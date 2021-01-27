#include <iostream>
#include <string>
#include <zmq.hpp>
#include <ctime>
#define N 1000


int main() {
	zmq::context_t context{1};
	zmq::socket_t socket{context, zmq::socket_type::req};
    	socket.connect("tcp://localhost:50002");

	zmq::message_t reply{};
	
	std::time_t result = std::time(nullptr);
	
	for(int i = 0; i < N; i++) {
		socket.send(zmq::buffer(""));
		socket.recv(reply);

		std::string str = "ADDU ";
		str += std::to_string(i);
		str += " password";
		str += std::to_string(i);
		
		socket.send(zmq::buffer(str));	
		socket.recv(reply);
		
		socket.send(zmq::buffer("QUIT"));
		socket.recv(reply);
	}

	zmq::socket_t socket1{context, zmq::socket_type::req};
	socket1.connect("tcp://localhost:50000");
	std::string arr[12];
    	arr[0] = ""; // Establish connection
    	arr[1] = "Helo localhost"; // Helo
    	arr[2] = "mail "; // Sender email
    	arr[3] = "rcpt "; // Recipient
    	arr[4] = "SUBJ ";
    	arr[5] = "data"; // Transmit content of mail
    	arr[6] = "Dear fellow users,\n This is a piece of email. Timestamp: "; // Content of mail
	arr[7] = "\\n.\\n"; // End Data 
    	arr[8] = "quit"; // End session
  
 
	for(int i = 0; i < N; i++) {
		socket1.send(zmq::buffer(arr[0]));	
		socket1.recv(reply);
		socket1.send(zmq::buffer(arr[1]));	
		socket1.recv(reply);
		std::string mail = arr[2];
		mail += std::to_string(i);
		mail += "@localhost";
		socket1.send(zmq::buffer(mail));	
		socket1.recv(reply);


		for(int j = 0; j < N; j++) {
			if(i == j) 
				continue;
			std::string recipient = arr[3];
			recipient += std::to_string(j);
			recipient += "@localhost";
			socket1.send(zmq::buffer(recipient));	
			socket1.recv(reply);
				
		}
		
		std::string subject = arr[4];
		subject += "Subject ";
		subject += std::to_string(i);	
		socket1.send(zmq::buffer(subject));
		socket1.recv(reply);

		socket1.send(zmq::buffer(arr[5]));
		socket1.recv(reply);

		socket1.send(zmq::buffer(arr[6]));
		socket1.recv(reply);

		std::string timestamp = std::asctime(std::localtime(&result));

		socket1.send(zmq::buffer(timestamp));
		socket1.recv(reply);
	
		socket1.send(zmq::buffer(arr[7]));
		socket1.recv(reply);

		socket1.send(zmq::buffer(arr[8]));
		socket1.recv(reply);

	}

}


