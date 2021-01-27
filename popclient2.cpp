#include <string>
#include <vector>
#include <iostream>
#include <zmq.hpp>

int main() {
	
	// initialize the zmq context with a single IO thread
    	zmq::context_t context{1};

    	// construct a REQ (request) socket and connect to interface
    	zmq::socket_t socket{context, zmq::socket_type::req};
    	socket.connect("tcp://localhost:50001");
	
	std::string arr[8];
	arr[0] = "";
	arr[1] = "USER Alice";
	arr[2] = "PASS 1234";
	arr[3] = "STAT";
	arr[4] = "RETR 0";
	arr[5] = "DELE 0";
	arr[6] = "RETR 0";
	arr[7] = "QUIT";


	for (int i = 0; i < 8; i++) {
		// send the request message
        	std::cout << "Sending " << arr[i] << "..." << std::endl;
        	socket.send(zmq::buffer(arr[i]), zmq::send_flags::none);
        
        	// wait for reply from server
        	zmq::message_t reply{};
        	socket.recv(reply, zmq::recv_flags::none);

        	std::cout << "Received " << reply.to_string(); 	
        	std::cout << std::endl;

		if(i == 4) {
			std::string str = reply.to_string();

			int lines = stoi(str.substr(6, str.size()-7));

        		std::cout << "Received " << reply.to_string(); 	
        		std::cout << std::endl;

			for(int j = 0; j < lines; j++) {
				socket.send(zmq::buffer(arr[i]), zmq::send_flags::none);
				zmq::message_t reply{};
        			socket.recv(reply, zmq::recv_flags::none);
				std::cout << reply.to_string() << std::endl;
			}
		}
	}
	return 0;


}
