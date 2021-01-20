#ifndef SMTPSTATE_H_
#define SMTPSTATE_H_

#include <string>
#include "SMTP_event.h"

// Forward declaration of SMTPsession.
class SMTPsession;

// Interface for the states of the SMTPsession.
class SMTPState {
  
 public:
	 // Take care of the responsibilities of state 
	 virtual void Action(SMTPsession*, SMTP_event*) = 0;

	 // Change state
	 virtual void ChangeState(SMTPsession*, int) = 0;

	 // Make it possible to index states 
	 virtual int getStateNo() = 0;	
 };

#endif
