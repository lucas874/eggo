#include <string>
#include <vector>
#include <iostream>
#include <zmq.hpp>
int main(void) {

	// initialize the zmq context with a single IO thread
    	zmq::context_t context{1};

    	// construct a REQ (request) socket and connect to interface
    	zmq::socket_t socket{context, zmq::socket_type::req};
    	socket.connect("tcp://localhost:50001");
	
	socket.send(zmq::buffer(""), zmq::send_flags::none);
	
	std::string CMD;

	while(true) {
		zmq::message_t reply{};
		socket.recv(reply, zmq::recv_flags::none);
		std::cout << reply.to_string();
		
		// std::cout << "Enter command: ";
		std::getline(std::cin, CMD);
		socket.send(zmq::buffer(CMD), zmq::send_flags::none);		
	}	
	socket.close();
	return 0;
}
