#include "POPsession.h"


POPsession::POPsession(std::vector<POPstate*> v, UserCollection* u, struct connection* c)
	: states(v)
	, currentState(v[0])
	, uc(u)
	, _connection(c)
	
	{
		//socket.bind("tcp://*:50001");
	}

void POPsession::Run() {
	while(run) {
		zmq::message_t rawrequest;
    		_connection->socket.recv(rawrequest, zmq::recv_flags::none);
    		std::string request = rawrequest.to_string();
    		std::cout << "Received " << request << std::endl;

        	POPevent *e = ProcessRequest(request);

		if(e->getEventNo() == POP_NOOP)
		       Reply(REPLY_OK);
		else {	
			currentevent = e;
	    		ChangeState(e);
	    		currentState->Action(this, e);	
		}
		
	}
	delete this;
}
 
void POPsession::Close() {
	std::cout << "hej";
}

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
  	_connection->socket.send(zmq::buffer(buffer), zmq::send_flags::none);
}

void POPsession::Reply(int replycode, std::string reply) {
	std::string buffer;

	switch(replycode) {
		case REPLY_OK:
			buffer = "+OK ";
			buffer += reply;
			break;	
		case REPLY_ERR:
			buffer = "-ERR ";
			buffer += reply;
			break;
	}
	buffer += "\n";
	_connection->socket.send(zmq::buffer(buffer), zmq::send_flags::none);

}

POPevent* POPsession::ProcessRequest(std::string buffer) {

	enum POP_Events e;

	std::string CMD = buffer.substr(0, 4);
	
	std::string data;
	if(buffer.size() > 4)
		data = buffer.substr(5);
	
	if(CMD.compare("USER") == 0)
		e = P_USER;
	else if(CMD.compare("PASS") == 0)
		e = P_PASS;
	else if(CMD.compare("STAT") == 0)
		e = P_STAT;
	else if(CMD.compare("LIST") == 0)
		e = P_LIST;
	else if(CMD.compare("RETR") == 0)
		e = P_RETR;
	else if(CMD.compare("DELE") == 0)
		e = P_DELE;
	else if(CMD.compare("RSET") == 0)
		e = P_RSET;
	else if(CMD.compare("QUIT") == 0)
		e = P_QUIT;
	else if(CMD.compare("NOOP") == 0) 
		e = P_NOOP;
	else 
		e = P_NOOP;

	if(currentState->getStateNo() == 0 && e == P_QUIT) {
		Reply(QUIT_AUTH_OK);
	       	// Handle this. Close properly, destroy current POPsession object.	
	}

	POPevent* event = new POPevent(e, data);
	return event;	
}

void POPsession::ChangeState(POPevent* e)  {
       currentState->ChangeState(this, e->getStateNo());
}

void POPsession::setCurrentUser(User* user) {
	currentUser = user;
}

User* POPsession::getCurrentUser() {
	return currentUser;
}



