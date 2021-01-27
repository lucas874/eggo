#include "SMTPsession.h"

// Initialiaze state vector, initial state, collection of user and connection
SMTPsession::SMTPsession(std::vector<SMTPState*> v, UserCollection* u, struct connection* c) 
	: states(v) 
	, currentState(v[0]) 
	, _uc(u) 
	, _connection(c) 	
{	
   		currentData.clear();
   		curmail = nullptr;
}

// Delete states, allocated on heap
SMTPsession::~SMTPsession() {
	for(auto &itr : states)
		delete itr;	
}

// Runs until QUIT command is issued
void SMTPsession::Run() {
	while(run) {
		zmq::message_t rawrequest;
    		_connection->socket.recv(rawrequest, zmq::recv_flags::none);
    		std::string request = rawrequest.to_string();
    		std::cout << "Received " << request << std::endl;
	
		SMTPevent *ed = ProcessRequest(request);	
	
		if(ed->getEventNo() == SMTP_NOOP)
			Reply(220);
		if(ed->getEventNo() == SMTP_BAD_CMD)
			Reply(CMD_NOT_RECOGNIZED);
		else {
			//currentevent = ed;
	    		ChangeState(ed);
	    		currentState->Action(this, ed);
		}
	}
	Close();
}

// Call destructor
void SMTPsession::Close() {
	this->~SMTPsession();
}


// Send reply over socket
void SMTPsession::Reply(int replycode) {
  std::string buffer;
  

  // Switch on reply code. Reply with appropriate answer.
  switch(replycode) {
	  case SERVICE_READY:
		  buffer = "220 localhost Service Ready";
		  break;
	  case SERVICE_CLOSING:
		  buffer = "221 localhost Service closing transmission channel";
    		  break;
  	  case MAIL_ACTION_OK:
    		  buffer = "250 Requested mail action okay, completed";
    		  break;
  	  case USER_NOT_LOCAL:
    		  buffer = "551 User not local; will not be forwarded";
    		  break;
	  case START_MAIL_INPUT:
		  buffer = "354 Start mail input end with <CRLF>.<CRLF>";
    		  break;
	  case SERVICE_NA_CLOSING:
		  buffer = "421 localhost Service not available, closing transmission channel";
		  break;
	  case FAIL_MAILBOX_UNAVAILABLE:
		  buffer = "552 Mailbox unavailable";
		  break;
	  case CMD_NOT_RECOGNIZED:
		  buffer = "500 Command not recognized";
		  break;
	  case BAD_CMD_SEQUENCE:
		 buffer = "503 Bad sequence of commands";
		 break;
	  case END_OF_DATA:
		 buffer = "355 Mail data transmission over";
		 break;
	  case SYNTAX_ERROR:
		 buffer = "501 syntax error";
		 break;	 
	  default:
    		  buffer = "Error. Command not recognized.";
  } 

  buffer += "\n";
  
  // Send reply to client
  _connection->socket.send(zmq::buffer(buffer), zmq::send_flags::none);
}


// Read string received from network and turn it into 
// a type that the FSM understands
SMTPevent* SMTPsession::ProcessRequest(std::string buffer) {
	std::string CMD;
   
   	std::string data;

   	if(buffer.size() > 4)
		data = buffer.substr(5, buffer.size()-4);
	

	CMD = buffer.substr(0,4);

	transform(CMD.begin(), CMD.end(), CMD.begin(), ::tolower);

   	enum SMTP_event_enum e;

   	/*
    	 * Not very good with long else if statements,
    	 * but on the other hand this part of the code
    	 * is unlikely to change (changes if RFC-821 changes, which it wont RFC aren't changed,
    	 * new ones are published). RFC-821 has been obsoleted... 
    	 * and with 8 cases it is kind of ok. 
    	 */
   	if(CMD.compare("helo") == 0)
     		e = HELO;
	else if(CMD.compare("ehlo") == 0)
     		e = HELO;
   	else if(CMD.compare("mail") == 0)
     		e = MAIL;
   	else if(CMD.compare("rcpt") == 0)
     		e = RCPT;
   	else if(CMD.compare("data") == 0)
     		e = DATA;
	else if(CMD.compare("rset") == 0)
     		e = RSET; 
   	else if(CMD.compare("noop") == 0) 
     		e = NOOP;
	else if(CMD.compare("quit") == 0)
	       e = QUIT;
	else if(CMD.compare("subj") == 0)
		e = SUBJ;
   	else if(currentState->getStateNo() == 4) {
		e = DATA;
	   	data = buffer;
	}
	else
		e = BAD_CMD;
	
	// SMTPevent with relevant enum and data 
	SMTPevent *event = new SMTPevent(e, data);

   	return event;
}

void SMTPsession::ChangeState(SMTPevent* e) {
	currentState->ChangeState(this, e->getEventNo());
}
  
void SMTPsession::setSenderDomain(std::string s) {
  senderDomain = s;
}


void SMTPsession::setSenderUsername(std::string s) {
  senderUsername = s;
}

void SMTPsession::setRcptDomain(std::string s) {
  rcptDomain.push_back(s);
}

void SMTPsession::setRcptUsername(std::string s) {
  rcptUsername.push_back(s);
}

/*std::string SMTPsession::getCurData() {
  return currentevent->getData();
}*/

