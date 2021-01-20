#include "top.h"

Top::Top(UserCollection* u)
	: usercollection(u) 
	{
	connection1 = new struct connection;
	connection2 = new struct connection;
	connection1->context = zmq::context_t(1);
	connection1->socket = zmq::socket_t(connection1->context, zmq::socket_type::rep); 
	connection2->context = zmq::context_t(1);
	connection2->socket = zmq::socket_t(connection2->context, zmq::socket_type::rep); 

	connection1->socket.bind("tcp://*:50000");
   	connection2->socket.bind("tcp://*:50001");	
}

void Top::listen() {
	while(1) {
		bool rc;

		do {
			zmq::message_t task1;
			if((rc = connection1->socket.recv(&task1, ZMQ_DONTWAIT)) == true)
			{	
				SMTPsession* session = createSMTPsession();
				session->Reply(220);
				createSMTPsession()->Run();


			}
		} while (rc == true);

		do {
			zmq::message_t task2;
			if((rc = connection2->socket.recv(&task2, ZMQ_DONTWAIT)) == true)
			{
				POPsession* session = createPOPsession();
				session->Reply(0);
				session->Run();
			}
		} while (rc == true);
	}
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


		  
