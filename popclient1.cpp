#include <string>
#include <vector>
#include <iostream>
#include <zmq.hpp>

/*
int main() {
	
	// initialize the zmq context with a single IO thread
    	zmq::context_t context{1};

    	// construct a REQ (request) socket and connect to interface
    	zmq::socket_t socket{context, zmq::socket_type::req};
    	socket.connect("tcp://localhost:50001");
	
	const std::string d("USER Bob");
	const std::string d1("PASS Kodeord");
    	const std::string data{"STAT "};
	const std::string data1{"LIST 5"};
	const std::string data2{"RETR 0"};
	const std::string data3{"DELE 0"};
    	const std::string data4{"RETR 0"};
	const std::string D("QUIT");
	const std::string streng("");

	std::string arr[9];
	arr[0] = streng;
	arr[1] = d;
	arr[2] = d1;
	arr[3] = data;
	arr[4] = data1;
	arr[5] = data2;
	arr[6] = data3;
	arr[7] = data4;
	arr[8] = D;
	int i = 0;
	for (auto request_num = 0; request_num < 9; ++request_num) {
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


}*/

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
