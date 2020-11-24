#ifndef _SMTPSTATE_H_
#define _SMTPSTATE_H_
//#include "Singleton.h"
//#include "SMTPConnection.h"
class SMTPConnection;

// Doesn't inherit from a Singleton class but is an implementation of a Singleton directly
class SMTPState {
  
 public:
  static SMTPState* instance();
  virtual void Close(SMTPConnection*);
  virtual void Send(SMTPConnection*);
  virtual void Action();
  SMTPState();

 protected:
  //void ChangeState(SMTPConnection*, SMTPState*);
  //SMTPState();

 private:
  static SMTPState* _instance;
  
};

#endif
