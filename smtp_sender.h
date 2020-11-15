#include "globals.h"
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include "zhelpers.h"
#include <cctype>


class SMTP_Sender {
 public:

  SMTP_Sender(){
    context = zmq_ctx_new ();
    requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5559");

    

    
    /* construct a REQ (request) socket and bind to interface
     * port 25 is reserved for SMTP connections, but it seems
     * that this port is blocked by our internet service provider
     * so port 5560 is used insted...
     */
  };

  ~SMTP_Sender() {
    zmq_close (requester);
    zmq_ctx_destroy (context);
  }
    
  int SendRequest(char* buffer);
 private:
  void *context;
  void *requester;
  
};

