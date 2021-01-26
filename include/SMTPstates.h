#ifndef _SMTP_STATES_H_
#define _SMTP_STATES_H_

#include "SMTPstate.h"
#include "globals.h"
#include <iostream>
#include <vector>
#include <string>

// Init state derived from the SMTPState interface
class SMTPInit : public SMTPState {
public:
	// State action
	void Action(SMTPsession* sc, SMTPevent* e) override;

	// State transitioning 
	void ChangeState(SMTPsession* sc, int n) override;

	// Get index
	int getStateNo() override; 
private: 
	// Vector to indicate allowed transitions. Indices corresponds to enum in SMTPevent.h
	std::vector<bool> allowedTransitions = 
	{false, true, false, false, false, false, true, false};
	// Notion of index
	int stateNo = SMTP_INIT;
  
};

// Helo state derived from SMTPState interface
class SMTPHelo : public SMTPState {
public:
	// State action
	void Action(SMTPsession* sc, SMTPevent* e) override; 
	
	// State transitioning 
	void ChangeState(SMTPsession* sc, int n) override;
	
	// Get index
	int getStateNo() override; 
private:
	// Vector of allowed transitions
	std::vector<bool> allowedTransitions = 
	{false, false, true, false, false, false, true, false};
	
	// Index state
	int stateNo = SMTP_HELO;
};

// Mail state derived from SMTPState interface 
class SMTPMail : public SMTPState {
public:
	// Do stuff
	void Action(SMTPsession* sc, SMTPevent* e) override;

	// State transitioning 
	void ChangeState(SMTPsession* sc, int n) override;  

	// Get index
	int getStateNo() override; 
private:
	// Allowed transitions
	std::vector<bool> allowedTransitions = 
	{false, false, false, true, false, true, true, false};
	
       	// Index state	
	int stateNo = SMTP_MAIL;

};

// Rcpt state derived from SMTPState interface
class SMTPRcpt : public SMTPState {
public:
	// Do stuff
	void Action(SMTPsession* sc, SMTPevent* e) override;

	// State transitioning
  	void ChangeState(SMTPsession* sc, int n) override;

	// Get index
  	int getStateNo() override; 
private:
	// Allowed transitions
  	std::vector<bool> allowedTransitions = 
	{false, false, false, true, true, true, true, false};
  	
	// Index state
	int stateNo = SMTP_RCPT;
};

// Data state derived from SMTPState interface
class SMTPData : public SMTPState {
public:
	// Do stuff
	void Action(SMTPsession* sc, SMTPevent* e) override;
  	
	// Change state
	void ChangeState(SMTPsession* sc, int n) override;
  	
	// Get index
	int getStateNo() override;
private:
	// Allowed transitions
  	std::vector<bool> allowedTransitions = 
	{false, true, true, false, true, true, true, false};
  	
	// Index state
	int stateNo = SMTP_DATA;
  	
};

// Rset state derived from SMTPState interface
class SMTPRset : public SMTPState {
public:
	// Do stuff 
	void Action(SMTPsession* sc, SMTPevent* e) override;
  	
	// Change state
	void ChangeState(SMTPsession* sc, int n) override;
  	
	// Get index
	int getStateNo() override; 
private:
	// Allowed transitions
  	std::vector<bool> allowedTransitions = {false, true, true, false, false, true, true, false};
  	
	// Notion of index
	int stateNo = SMTP_RSET; 
};

// Quit state derived from SMTPState interface
class SMTPQuit : public SMTPState {
public:
	// Do stuff
	void Action(SMTPsession* sc, SMTPevent* e) override;
  	
	// Change state
	void ChangeState(SMTPsession* sc, int n) override;
  	
	// Get index
	int getStateNo() override; 
private:
	// Allowed transitions
	std::vector<bool> allowedTransitions = 
	{false, false, false, false, false, false, false, false};
	
	// Notion of index
	int stateNo = SMTP_QUIT; 
};

#endif
