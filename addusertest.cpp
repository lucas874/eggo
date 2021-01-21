#include <string>
#include <vector>
#include <iostream>
#include <zmq.hpp>

int main() {
	
	// initialize the zmq context with a single IO thread
    	zmq::context_t context{1};

    	// construct a REQ (request) socket and connect to interface
    	zmq::socket_t socket{context, zmq::socket_type::req};
    	socket.connect("tcp://localhost:50002");
	
	const std::string d("ADDU LarsPoulsen heste");
    	const std::string D("QUIT");
	const std::string streng("");

	std::string arr[3];
	arr[0] = streng;
	arr[1] = d;
	arr[2] = D;
	int i = 0;
	for (auto request_num = 0; request_num < 3; ++request_num) {
		// send the request message
        	std::cout << "Sending " << arr[i] << "..." << std::endl;
        	socket.send(zmq::buffer(arr[i]), zmq::send_flags::none);
		i++;
        
        	// wait for reply from server
        	zmq::message_t reply{};
        	socket.recv(reply, zmq::recv_flags::none);

        	std::cout << "Received " << reply.to_string(); 
        	std::cout << " (" << request_num << ")";
        	std::cout << std::endl;
	}
	return 0;


}
