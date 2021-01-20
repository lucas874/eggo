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
    
    char a = '\r';
    char b = '\f';
    char c = '.';
    // set up some static data to send
    //const std::string data(4);
    const std::string d("");
    const std::string data{"Helo localhost"};
    const std::string D("mail Alice@localhost");
    const std::string rcpt("rcpt Bob@localhost");
    const std::string DATA("data");
    const std::string DaTA("HEj den store bagedyst er et tv program");
    //const std::string DaTA1("<CRLF>.<CRLF>");
    std::string DaTA1;
    DaTA1 += a;
    DaTA1 += b;
    DaTA1 += c;
    DaTA1 += a;
    DaTA1 += b;
    const std::string DaTA2 = DaTA1;
    const std::string Da("Quit");

    std::string arr[8];

    std::cout << DaTA1;
    arr[0] = d;
    arr[1] = data;
    arr[2] = D;
    arr[3] = rcpt;
    arr[4] = DATA;
    arr[5] = DaTA;
    arr[6] = DaTA1;
    arr[7] = Da;
    int i = 0;
    
    
    for (auto request_num = 0; request_num < 8; ++request_num) 
    {
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
    socket.close();
        return 0;
}
