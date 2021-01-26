#include <string>
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
    std::string arr[12];
    arr[0] = ""; // Establish connection
    arr[1] = "Helo localhost"; // Helo
    arr[2] = "mail ExampleUser@localhost"; // Sender email
    arr[3] = "rcpt Bob@localhost"; // Recipient
    arr[4] = "rcpt Alice@localhost";
    arr[5] = "rcpt UserName@gmail.com"; // will not work
    arr[6] = "SUBJ testsubject";
    arr[7] = "data"; // Transmit content of mail
    arr[8] = "Dear Bob & Alice,\n This is a piece of email"; // Content of mail
    arr[9] = "secadasdsa";
    arr[10] = "\\n.\\n"; // End Data 
    arr[11] = "quit"; // End session
    int i = 0;
    
    std::cout << "Starting SMTP session" << std::endl;

    for (int i = 0; i < 12; i++) 
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
    
    
    return 0;
}
