#include <string>
#include <iostream>
#include <vector>

#include <zmq.hpp>


int main()
{
    	// initialize the zmq context with a single IO thread
    	zmq::context_t context{1};

    	// construct a REQ (request) socket and connect to interface
    	zmq::socket_t socket{context, zmq::socket_type::req};
    	socket.connect("tcp://localhost:50000");
    
	std::string CRLF("");
   	char a = '\r';
    	char b = '\f';
    	char c = '.';
	CRLF = a + b + c + a + b;

	std::string CMD;
	bool run = true;

	while(run) {
		// std::cout << "Enter command: ";
		std::getline(std::cin, CMD);
		
		socket.send(zmq::buffer(CMD), zmq::send_flags::none);

		zmq::message_t reply{};
		socket.recv(reply, zmq::recv_flags::none);

		std::cout << reply.to_string() << std::endl;
	}
	socket.close();
	return 0;
}
