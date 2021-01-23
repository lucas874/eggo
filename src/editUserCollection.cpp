#include "editUserCollection.h"


void EditUsers::Run() {
	while(run) {
		zmq::message_t rawrequest;
    		c->socket.recv(rawrequest, zmq::recv_flags::none);
    		std::string request = rawrequest.to_string();
    		std::cout << "Received " << request << std::endl;

		std::string CMD = request.substr(0,4);
		if(CMD.compare(EDIT_USER_QUIT) == 0) {
			Reply(0);
			run = false;
		}
		else {
			std::string name;
			std::string pass;
			int i = 5;
			while(request[i] != ' ') {
				i++;
			}
			name = request.substr(5, i-5);
			pass = request.substr(i+1, request.size()-i);	

			if(CMD.compare(EDIT_USER_ADDU) == 0) {
				Reply(uc->addUser(name, pass));
			}
			else if(CMD.compare(EDIT_USER_REMU) == 0)
				Reply(uc->removeUser(name, pass));
			else
				Reply(-3);
		}
	}
	delete this;
}

void EditUsers::Reply(int replycode) {
	std::string buffer;
	switch(replycode) {
		case 0:
			buffer = "+OK\n";
			break;
		case -1:
			buffer = "-ERR\n";
			break;
		case -2:
			buffer = "-Incorrect password, user not removed\n";
			break;
		case -3:
			buffer = "-Request not understood\n";
	}
	c->socket.send(zmq::buffer(buffer), zmq::send_flags::none);
}




