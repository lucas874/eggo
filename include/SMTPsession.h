#ifndef SMTPCONNECTION_H_
#define SMTPCONNECTION_H_

#include "SMTPstate.h"
#include "Globals.h"
#include "SMTP_event.h"
#include "UserCollection.h"
#include "PieceOfMail.h"
#include "connection.h"
#include <iostream>
#include <unistd.h> // sleep
#include <string>
//#include <zmq.hpp>
#include <vector> 

class SMTPState;

class SMTPsession {
 public:
  SMTPsession(std::vector<SMTPState*> v, UserCollection* u, struct connection*);
  ~SMTPsession();
  void Run();
  void Close();
  void Reply(int replycode);
  SMTP_event* ProcessRequest(std::string buffer);
  void processTextLine(std::string buffer);
  void StateAction();
  void ChangeState(SMTP_event*);
  void setSenderDomain(std::string);
  void setSenderUsername(std::string);
  void setRcptDomain(std::string);
  void setRcptUsername(std::string);
  std::string getCurData();
  

 private:
  friend class SMTPState;
  friend class SMTPInit;
  friend class SMTPHelo;
  friend class SMTPMail;
  friend class SMTPRcpt;
  friend class SMTPData;
  friend class SMTPQuit;
  friend class SMTPRset;

  void ChangeState(SMTPState*); 
  SMTPState* _state;
  UserCollection* _uc;
  std::vector<SMTPState*> states;
  SMTP_event* currentevent; 
  zmq::context_t context;
  zmq::socket_t socket;
  std::string senderDomain;
  std::string senderUsername;
  std::string rcptDomain;
  std::vector<std::string> rcptUsername;
  std::vector<User*> rcpt;
  PieceOfMail* curmail;
  std::string currentData;
  struct connection *_connection;
  bool run = true;
  
};

#endif
