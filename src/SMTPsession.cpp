#include "SMTPsession.h"


/* TODO:
 * Check length of received string
 * User class
 * Piece of mail class
 * User "database", with has table mapping to directories
 *
 * In ProcessRequest  NOOP event with event number assigned to current state,
 * so that ChangeState doesn't do anything?  Instead of if statement in run?
 */
SMTPsession::SMTPsession(std::vector<SMTPState*> v, UserCollection* u, struct connection* c) 
	: states(v) 
	, _state(v[0]) 
	, _uc(u) 
	, _connection(c) 	
{
		//socket.bind("tcp://*:50000");
   		currentData.clear();
   		curmail = nullptr;
}

SMTPsession::~SMTPsession() {
	for(int i=0; i < 7; i++)
		delete states[i];
	//delete this;
}
void SMTPsession::Run() {
	while(run) {
      
    	zmq::message_t rawrequest;
    	_connection->socket.recv(rawrequest, zmq::recv_flags::none);
    	std::string request = rawrequest.to_string();
    	std::cout << "Received " << request << std::endl;

    /*if(_state->getStateNo() == 4)
	    _state->Action(this, request);
	    */
    	SMTP_event *ed = ProcessRequest(request);
    	if(ed->getEventNo() == 7)
	    	Reply(220);
	else {
		currentevent = ed;
	    	ChangeState(ed);
	    	_state->Action(this, ed);
	}
	sleep(1);
	
	}
	delete this;
}
    
void SMTPsession::Close() {
  std::cout << "hej";
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
  	  case USER_NOT_LOCAL_FORWARDED:
    		  buffer = "User not local; will forward to <forward-path>";
    		  break;
	  case START_MAIL_INPUT:
		  buffer = "Start mail input end with <CRLF>.<CRLF>";
    		  break;
	  case SERVICE_NA_CLOSING:
		  buffer = " localhost Service not available, closing transmission channel";
		  break;
	  case FAIL_MAILBOX_UNAVAILABLE:
		  buffer = "Mailbox unavailable";
		  break;
	  case ERROR_IN_PROCESSING:
    		  buffer = " ";
		  break;
	  case FAIL_INSUFFICIENT_STORAGE:
		  buffer = " ";
		  break;
	  case SYNTAX_ERROR_COMMAND_NOT_RECOGNIZED:
    		  buffer = " ";
		  break;
  	  case SYNTAX_ERROR_IN_ARGUMENTS:
    		  buffer = " ";
    		  break;
  	  case COMMAND_NOT_IMPLEMENTED:
    		  buffer = " ";
    		  break;
  	  case BAD_COMMAND_SEQUENCE:
    		  buffer = " ";
    		  break;
	  case PARAMETER_NOT_IMPLEMENTED:
    		  buffer = " ";
    		  break;
  	  case MAILBOX_NOT_FOUND:
    		  buffer = " ";
    		  break;
  	  case USER_NOT_LOCAL:
    		  buffer = " ";
    		  break;
  	  case MAIL_ACTION_ABORT_STORAGE:
    		  buffer = " ";
    		  break;
	  case MAILBOX_SYNTAX_ERROR:
    		  buffer = " ";
    		  break;
  	  case TRANSACTION_FAILED:
    		  buffer = " ";
    		  break;
  	  default:
    		  buffer = "Command not recognized";
  } 
  
  // Send reply to client
  _connection->socket.send(zmq::buffer(buffer), zmq::send_flags::none);
}

SMTP_event* SMTPsession::ProcessRequest(std::string buffer) {
   std::string CMD(4, ' ');
   
   std::string data;
   if(buffer.size() > 4)
	   data = buffer.substr(5);

    
   for(int i = 0; i < 4; i++)
     CMD[i] = buffer[i];

   transform(CMD.begin(), CMD.end(), CMD.begin(), ::tolower);

   enum SMTP_event_enum e;

   /*
    * Not very good with long else if statements,
    * but on the other hand this part of the code
    * is unlikely to change (changes if RFC-821 changes, which it wont RFC aren't changed,
    * new ones are published). RFC-821 has been obsoleted... 
    * and with 8 cases it is kind of ok. 
    */
   if(CMD.compare("helo") == 0) {
     //do something
     e = HELO;
     
   }
   else if(CMD.compare("ehlo") == 0) {
     e = HELO;
     
   }
   else if(CMD.compare("mail") == 0) {
     e = MAIL;
     
   }
   else if(CMD.compare("rcpt") == 0) {
     e = RCPT;
     
   }
   else if(CMD.compare("data") == 0) {
     e = DATA;
     
   }
   else if(CMD.compare("rset") == 0) {
     e = RSET;
     
   }
   else if(CMD.compare("noop") == 0) {
     e = NOOP;
     //Reply(250);
     
     // NOOP doesn't cause a state transition
     // this value is never used
     
   }
   else if(CMD.compare("quit") == 0) {
     e = QUIT;
     
   }
   else if(_state->getStateNo() == 4) {
	   e = DATA;
	   data = buffer;
	   if(buffer.substr(0,5).compare("\r\f.\r\f") == 0)
		   e = HELO;
   }
   
   SMTP_event *event = new SMTP_event(e, data);

   return event;
}

void SMTPsession::ChangeState(SMTPState* s) {
  _state = s;
}

void SMTPsession::ChangeState(SMTP_event* e) {

  //_state = states[e->getEventNo()];
  _state->ChangeState(this, e->getEventNo());
}
  
/*void SMTPsession::StateAction() {
  _state->Action(this);
}*/

void SMTPsession::setSenderDomain(std::string s) {
  senderDomain = s;
}


void SMTPsession::setSenderUsername(std::string s) {
  senderUsername = s;
}

void SMTPsession::setRcptDomain(std::string s) {
  rcptDomain = s;
}

void SMTPsession::setRcptUsername(std::string s) {
  rcptUsername.push_back(s);
}

std::string SMTPsession::getCurData() {
  return currentevent->getData();
}

