#include <string>
#include <vector>
#include <iostream>
#include <zmq.hpp>


/*
 * Unit test of SMTP component
 * POP3 component
 * Add user component
 */

int main()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};
    socket.connect("tcp://localhost:50000");
    
    
    // Array containing commands sent to server
    std::vector<std::string> arr(8);
    arr[0] = ""; // Establish connection
    arr[1] = "Helo localhost"; // Helo
    arr[2] = "mail Alice@localhost"; // Sender email
    arr[3] = "rcpt Bob@localhost"; // Recipient
    arr[4] = "data"; // Transmit content of mail
    arr[5] = "Dear Bob,\n This is a piece of email"; // Content of mail
    arr[6] = "\r\f.\r\f"; // End Data 
    arr[7] = "quit"; // End session
    
    std::cout << "Testing SMTP component" << std::endl << std::endl;

    for (int i = 0; i < 8; i++) 
    {
        // send the request message
        std::cout << "Sending " << arr[i] << "..." << std::endl;
        socket.send(zmq::buffer(arr[i]), zmq::send_flags::none);	
        
        // wait for reply from server
        zmq::message_t reply{};
        socket.recv(reply, zmq::recv_flags::none);

        std::cout << "Received " << reply.to_string();  
        std::cout << std::endl;
	}
    socket.close();

    std::cout << std::endl;
    
    // initialize the zmq context with a single IO thread
    zmq::context_t context1{1};
    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket1{context1, zmq::socket_type::req};
    socket1.connect("tcp://localhost:50001");
    
    arr.resize(7);
    arr[0] = "";	
    arr[1] = "USER Bob";
    arr[2] = "PASS Kodeord";
    arr[3] = "STAT";
    arr[4] = "LIST 5";
    arr[5] = "RETR 0";
    arr[6] = "QUIT";	
    
    std::cout << "Testing POP component" << std::endl << "Logging in as user Bob" << std::endl;
    std::cout << "Asking for information about a non-existing piece of mail" << std::endl;
    std::cout << "Retrieving newly received piece of mail sent by Alice" << std::endl << std::endl;		
    
    for (int i = 0; i < 7; i++) {
	    // send the request message
	    std::cout << "Sending " << arr[i] << "..." << std::endl;
	    socket1.send(zmq::buffer(arr[i]), zmq::send_flags::none);
	    
	    // wait for reply from server
	    zmq::message_t reply{};
	    socket1.recv(reply, zmq::recv_flags::none);
	    std::cout << "Received " << reply.to_string(); 	
	    std::cout << std::endl;
	}

    // initialize the zmq context with a single IO thread
    zmq::context_t context2{1};
    
    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket2{context2, zmq::socket_type::req};
    socket2.connect("tcp://localhost:50002");
	
    arr.resize(3);
    arr[0] = "";
    arr[1] = "ADDU LarsPoulsen heste";
    arr[2] = "QUIT";
    

    std::cout << "Testing add user component" << std::endl;
    std::cout << "Adding User LarsPoulsen" << std::endl;
    for (int i = 0; i < 3; i++) {
	    // send the request message
	    std::cout << "Sending " << arr[i] << "..." << std::endl;
	    socket2.send(zmq::buffer(arr[i]), zmq::send_flags::none);
	    
	    // wait for reply from server
	    zmq::message_t reply{};
	    socket2.recv(reply, zmq::recv_flags::none);
	    
	    std::cout << "Received " << reply.to_string(); 
	    std::cout << std::endl;
	}





    	

	
    
    return 0;
}
