#ifndef SMTPEVENT_H
#define SMTPEVENT_H
#include <iostream>

// Commands that SMTP receiver understands represented as enums
enum SMTP_event_enum {INIT, HELO, MAIL, RCPT, DATA, RSET, QUIT, NOOP};

// Bytes received from socket parsed and converted to appropriate event
class SMTPevent{
	public:
		SMTPevent(enum SMTP_event_enum, std::string);
		
		// Return data contained in event. Sender domain, email content etc.
  		std::string getData();

  		// Return event number defined by enum
  		int getEventNo();
	
	private:
  	// First four characters of received string parsed and turned into 
  	// a variable of this type
  	enum SMTP_event_enum event;

  	// Stores things as sender domain, email content. 
  	std::string data;
};

#endif

