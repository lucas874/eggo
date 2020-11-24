#ifndef _SMTPCONNECTION_H_
#define _SMTPCONNECTION_H_

#include "SMTP_States.h"
#include "zhelpers.h"
#include "globals.h"
#include "Connection.h"
#include "EventData.h"

#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <string>
#include <cstring>

class SMTPState;

class SMTPConnection {
 public:
  SMTPConnection(struct Connection*);
  

  void Run();
  void Close();
  void Reply(int replycode);
  EventData* ProcessRequest(char *buffer);
  std::string getTextLine(char *buffer);
  void StateAction();
  

 private:
  friend class SMTPState;
  void ChangeState(SMTPState*);
  void ChangeState(EventData*);
  SMTPState* _state;
  SMTPState **states;
  struct Connection* _connection;
  /*std::string senderDomain;
  std::string senderUserName;
  std::string rcptDomain;
  std::string rcptUsername;*/
  
};

#endif
