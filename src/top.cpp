#include "top.h"

Top::Top(UserCollection* u)
	: usercollection(u) 
	{
		// Set up connection to network
		connection1 = new struct connection;
		connection2 = new struct connection;
		connection3 = new struct connection;
		connection1->context = zmq::context_t(1);
		connection1->socket = zmq::socket_t(connection1->context, zmq::socket_type::rep); 
	
		connection2->context = zmq::context_t(1);
		connection2->socket = zmq::socket_t(connection2->context, zmq::socket_type::rep); 
	
		connection3->context = zmq::context_t(1);
		connection3->socket = zmq::socket_t(connection3->context, zmq::socket_type::rep); 
	
		connection1->socket.bind("tcp://*:50000");
   		connection2->socket.bind("tcp://*:50001");
		connection3->socket.bind("tcp://*:50002");	
}

void Top::listen() {
	while(1) {
		char msg[256];

		// For listening on all sockets without blocking control flow, while listening
		// on one of them. 
		zmq_pollitem_t items [] = {
			{ connection1->socket, 0, ZMQ_POLLIN, 0 },
			{ connection2->socket, 0, ZMQ_POLLIN, 0 },
			{ connection3->socket, 0, ZMQ_POLLIN, 0 }
		};
		zmq_poll (items, 3, -1);

		// Ask if anything has been received on connection1
		// If so instantiate SMTPsession and handle further
		// communication with this client through new object.
		if(items[0].revents & ZMQ_POLLIN) {
			int size = connection1->socket.recv(msg, 255, 0);
			if(size != -1) {
				SMTPsession* session = createSMTPsession();
				session->Reply(220);
				session->Run();
			}
		}

		// Ask if anything has been received on connection2
		// If so instantiate POPsession.
		if(items[1].revents & ZMQ_POLLIN) {
			int size = connection2->socket.recv(msg, 255, 0);
			if(size != -1) {
				POPsession* session = createPOPsession();
				session->Reply(GREETING);
				session->Run();
			}
		}
		// Ask if anything has been received on connection2
		// If so instantiate POPsession.
		if(items[2].revents & ZMQ_POLLIN) {
			int size = connection3->socket.recv(msg, 255, 0);
			if(size != -1) {
				EditUsers* session = new EditUsers(usercollection, connection3);
				session->Reply(0);
				session->Run();
			}
		}

	}
	connection1->socket.close();
	connection2->socket.close();
	connection3->socket.close();

	connection1->context.close();
	connection2->context.close();
	connection3->context.close();
}

std::vector<POPstate*> Top::initializePOPstates() {
	std::vector<POPstate*> v(3);
  	v[0] = new POPauthorization();
  	v[1] = new POPtransaction();
  	v[2] = new POPupdate;
  	return v;
}
 

std::vector<SMTPState*> Top::initializeSMTPstates() {
	std::vector<SMTPState*> v(7);
  	v[0] = new SMTPInit();
  	v[1] = new SMTPHelo();
  	v[2] = new SMTPMail;
  	v[3] = new SMTPRcpt;
  	v[4] = new SMTPData;
  	v[5] = new SMTPRset;
  	v[6] = new SMTPQuit;

  return v;
}

SMTPsession* Top::createSMTPsession() {
	return new SMTPsession(initializeSMTPstates(), usercollection, connection1);
}

POPsession* Top::createPOPsession() {
	return new POPsession(initializePOPstates(), usercollection, connection2);
}


		  
