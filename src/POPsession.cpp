#include "POPsession.h"

// POPsession constructor with vector of states, collection of users and connection
POPsession::POPsession(std::vector<POPstate*> v, UserCollection* u, struct connection* c)
	: states(v)
	, currentState(v[0])
	, uc(u)
	, _connection(c)
	
	{
		//socket.bind("tcp://*:50001");
	}

// Run method that loops, receives commands on its socket and performs the correct actions
void POPsession::Run() {
	while(run) {
		zmq::message_t rawrequest;
    		_connection->socket.recv(rawrequest, zmq::recv_flags::none);
    		std::string request = rawrequest.to_string();
    		std::cout << "Received " << request << std::endl;

        	POPevent *e = ProcessRequest(request);

		if(e->getEventNo() == POP_NOOP)
			Reply(REPLY_OK);
		else if(e->getEventNo() == POP_BAD)
			Reply(BAD_CMD_SEQ);
		else {	
			currentevent = e;
	    		ChangeState(e);
	    		currentState->Action(this, e);	
		}
		
	}
	Close();
}

// Delete states 
void POPsession::Close() {
	for(auto& i : states)
	       delete i;
}

// Reply method only with reply code as parameter
void POPsession::Reply(int replycode) {
	std::string buffer;
	switch(replycode) {
		case USER_OK:
			buffer = "+OK user found";
			break;
		case USER_ERR:
			buffer = "-ERR user not found";
			break;
		case PASS_OK:
			buffer = "+OK password ok";
			break;
		case PASS_ERR:
			buffer = "-ERR password not ok";
		       	break;
		case QUIT_AUTH_OK:
			buffer = "+OK POP3 server signing off";
			break;
		case QUIT_TRAN_OK:
			buffer = "+OK POP3 server signing off (";
			buffer += std::to_string(currentUser->getInboxSize());
			buffer += " messages left)";
			break;
		case STAT_OK:
			buffer = "+OK ";
			buffer += std::to_string(currentUser->getInboxSize());
			buffer += " ";
			buffer += std::to_string(currentUser->getInboxSizeOctets());
			break;
		case BAD_CMD_SEQ:
			buffer = "-ERR bad sequence of commands, try another";
			break;
		case GREETING:
			buffer = "localhost POP3 service ready";
			break;
		case REPLY_OK:
			buffer = "+OK ";
			break;
	        default:
			buffer = "Command not understood";
			break;
	}	

  	buffer += "\n";
	// send reply message
	_connection->socket.send(zmq::buffer(buffer), zmq::send_flags::none);
}

// Reply method with both reply code and a string as parameters
void POPsession::Reply(int replycode, std::string reply) {
	std::string buffer;

	switch(replycode) {
		case REPLY_OK:
			buffer = "+OK ";
			buffer += reply;
			buffer += "\n";
			break;	
		case REPLY_ERR:
			buffer = "-ERR ";
			buffer += reply;
			buffer += "\n";
			break;
		case CONTENT_TRANSMIT:
			buffer = reply;
			break;
	}
		
	// send reply message
	_connection->socket.send(zmq::buffer(buffer), zmq::send_flags::none);

}

// Process the strings of incoming commands to decide event type
POPevent* POPsession::ProcessRequest(std::string buffer) {

	int e;

	std::string CMD = buffer.substr(0, 4);
	
	std::string data;
	if(buffer.size() > 4)
		data = buffer.substr(5); // assign the argument after the command to a string	
	if(CMD.compare("USER") == 0)
		e = POP_USER;
	else if(CMD.compare("PASS") == 0)
		e = POP_PASS;
	else if(CMD.compare("STAT") == 0)
		e = POP_STAT;
	else if(CMD.compare("LIST") == 0)
		e = POP_LIST;
	else if(CMD.compare("RETR") == 0)
		e = POP_RETR;
	else if(CMD.compare("DELE") == 0)
		e = POP_DELE;
	else if(CMD.compare("RSET") == 0)
		e = POP_RSET;
	else if(CMD.compare("QUIT") == 0)
		e = POP_QUIT;
	else if(CMD.compare("NOOP") == 0) 
		e = POP_NOOP;
	else 
		e = POP_BAD;

	if(currentState->getStateNo() == 0 && e == POP_QUIT) {
		//Reply(QUIT_AUTH_OK);
	       	// Handle this. Close properly, destroy current POPsession object.	
	}
	
	// create the POPevent with the specified event and data, return it
	POPevent* event = new POPevent(e, data);
	return event;	
}

// Method for changing state
void POPsession::ChangeState(POPevent* e)  {
       currentState->ChangeState(this, e->getStateNo());
}

// Method for setting the currently logged in user
void POPsession::setCurrentUser(User* user) {
	currentUser = user;
}

// Method for returning the currently logged in user
User* POPsession::getCurrentUser() {
	return currentUser;
}


