#ifndef SMTP_STATES_H
#define SMTP_STATES_H

#include "SMTPstate.h"
#include "Globals.h"
#include <iostream>
#include <vector>
#include <string>

/*             0     1     2     3     4     5     6     7		
 * enum Event {INIT, HELO, MAIL, RCPT, DATA, RSET, QUIT, NOOP};
 *
 */

// Init state derived from the SMTPState interface
class SMTPInit : public SMTPState {
public:
  void Action(SMTPsession* sc, SMTP_event* e) override;
  void ChangeState(SMTPsession* sc, int n) override;
  int getStateNo() override; 
private: // vector to indicate allowed transitions. Indices corresponds to enum in Event.h.
  std::vector<bool> allowedTransitions = 
  {false, true, false, false, false, false, true, false};
  int stateNo = 0;
  
};

class SMTPHelo : public SMTPState {
public:
  void Action(SMTPsession* sc, SMTP_event* e) override; 
  void ChangeState(SMTPsession* sc, int n) override;
  int getStateNo() override; 
private:
  std::vector<bool> allowedTransitions = 
  {false, false, true, false, false, false, true, false};
  int stateNo = 1;
};

class SMTPMail : public SMTPState {
public:
  void Action(SMTPsession* sc, SMTP_event* e) override;
  void ChangeState(SMTPsession* sc, int n) override;  
  int getStateNo() override; 
private:
  std::vector<bool> allowedTransitions = 
  {false, false, false, true, false, true, true, false};
  int stateNo = 2;

};

class SMTPRcpt : public SMTPState {
public:
  void Action(SMTPsession* sc, SMTP_event* e) override;
  void ChangeState(SMTPsession* sc, int n) override;
  int getStateNo() override; 
private:
  std::vector<bool> allowedTransitions = 
  {false, false, false, true, true, true, true, false};
  int stateNo = 3;
};


class SMTPData : public SMTPState {
public:
  void Action(SMTPsession* sc, SMTP_event* e) override;
  void ChangeState(SMTPsession* sc, int n) override;
  int getStateNo() override;
  void append(SMTPsession* sc, SMTP_event* e); 
private:
  std::vector<bool> allowedTransitions = 
  {false, true, true, false, true, true, true, false};
  int stateNo = 4;
  bool ready4data; 
};

class SMTPRset : public SMTPState {
public:
  void Action(SMTPsession* sc, SMTP_event* e) override;
  void ChangeState(SMTPsession* sc, int n) override;
  int getStateNo() override; 
private:
  std::vector<bool> allowedTransitions = 
  {false, true, true, false, false, true, true, false};
  int stateNo = 5; 
};

class SMTPQuit : public SMTPState {
public:
  void Action(SMTPsession* sc, SMTP_event* e) override;
  void ChangeState(SMTPsession* sc, int n) override;
  int getStateNo() override; 
private:
  std::vector<bool> allowedTransitions = 
  {false, false, false, false, false, false, false, false};
  int stateNo = 6; 
};

#endif
