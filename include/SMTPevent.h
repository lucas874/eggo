#ifndef _SMTPEVENT_H_
#define _SMTPEVENT_H_
#include <iostream>

// Commands that SMTP receiver understands represented as enums
//                    0	    1     2     3     4     5     6     7     8
enum SMTP_event_enum {INIT, HELO, MAIL, RCPT, DATA, RSET, QUIT, NOOP, BAD_CMD};

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

