#include <iostream>
#include "SMTPConnection.h"
//#include "SMTP_States.h"
using namespace std;

int main() {
  struct Connection *C = new Connection;
  C->context = zmq_ctx_new();
  C->socket = zmq_socket(C->context, ZMQ_REP);
  zmq_connect(C->context, "tcp://localhost5560");
  SMTPConnection SC(C);

  SC.Close();

  cout << endl;

  SC.StateAction();

  

}
