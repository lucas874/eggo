#include "globals.h"
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include "zhelpers.h"
#include <algorithm>


class SMTP_Receiver {
 public:

  SMTP_Receiver(){

    // initialize the zmq context with a single IO thread
    context = zmq_ctx_new ();

    
    /* construct a REP (reply) socket and bind to interface
     * port 25 is reserved for SMTP connections, but it seems
     * that this port is blocked by our internet service provider
     * so port 5560 is used insted...
     */
    responder = zmq_socket(context, ZMQ_REP);
    zmq_connect(responder, "tcp://localhost:5560");
    server_state = SMTP_STATE_INIT;
  };
  
  ~SMTP_Receiver() {
    //  We never get here, but clean up anyhow
    zmq_close (responder);
    zmq_ctx_destroy (context);
  };

  int processHELO(char* buffer);
  int processMAIL(char* buffer);
  int processRCPT(char* buffer);
  int processDATA(char* buffer);
  int processRSET(char* buffer);
  int processNOOP(char* buffer);
  int processQUIT(char* buffer);
  int processRequest(char* buffer);
  void run();
  int reply(char* buffer);

 private:
  
  int server_state;
  void *context;
  void *responder;
  
};


