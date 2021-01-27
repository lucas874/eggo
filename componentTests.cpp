#include <iostream>
#include <string>
#include <zmq.hpp>
#include <ctime>
#define N 1000


int main() {

	std::cout << "Creating "<< N << " users" << std::endl;
	std::cout << "From each user a message is sent to the " << N-1 << " other users" << std::endl;
	std::cout << "We then log in as each user, check if each user has " << N-1 << " messages in the inbox, delete the messages and remove the user from the server" << std::endl;
	std::cout << "No more output means success" << std::endl;
	zmq::message_t reply{};
	
	std::time_t result = std::time(nullptr);
	
	for(int i = 0; i < N; i++) {
		zmq::context_t context{1};
		zmq::socket_t socket{context, zmq::socket_type::req};
    		socket.connect("tcp://localhost:50002");


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

		zmq_ctx_destroy(&context);
		socket.close();

	}
	
	
	std::string arr[9];
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
		zmq::context_t context1{1};
		zmq::socket_t socket1{context1, zmq::socket_type::req};
		socket1.connect("tcp://localhost:50000");

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
		
		zmq_ctx_destroy(&context1);
		socket1.close();
	}
	arr[1] = "USER ";
	arr[2] = "PASS password";
	arr[3] = "STAT ";
	arr[4] = "DELE ";
	arr[5] = "QUIT";
	for(int i = 0; i < N; i++) {
		zmq::context_t context2{1};
		zmq::socket_t socket2{context2, zmq::socket_type::req};
    		socket2.connect("tcp://localhost:50001");


		socket2.send(zmq::buffer(arr[0]));
		socket2.recv(reply);

		std::string user = arr[1];
		user += std::to_string(i);
		socket2.send(zmq::buffer(user));
		
		socket2.recv(reply);

		std::string pass = arr[2];
		pass += std::to_string(i);

		socket2.send(zmq::buffer(pass));
		socket2.recv(reply);

		socket2.send(zmq::buffer(arr[3]));
		socket2.recv(reply);

		std::string statreply = reply.to_string();

		int n = stoi(statreply.substr(4,3));

		if(n != N-1)
			std::cout << "FAIL!" << std::endl;
		
		else {
			std::string delemsg;
			for(int j = 0; j < N-1; j++) {
				delemsg = arr[4];
				delemsg += std::to_string(j);
				socket2.send(zmq::buffer(delemsg));
				socket2.recv(reply);
			}
		}
		socket2.send(zmq::buffer(arr[5]));
		socket2.recv(reply);
	
		zmq_ctx_destroy(&context2);	
		socket2.close();

		zmq::context_t context{1};
		zmq::socket_t socket{context, zmq::socket_type::req};
    		socket.connect("tcp://localhost:50002");

		socket.send(zmq::buffer(""));
		socket.recv(reply);

		std::string str = "REMU ";
		str += std::to_string(i);
		str += " password";
		str += std::to_string(i);
		
		socket.send(zmq::buffer(str));	
		socket.recv(reply);
		
		socket.send(zmq::buffer("QUIT"));
		socket.recv(reply);

		zmq_ctx_destroy(&context);
		socket.close();

	}
}

			


















