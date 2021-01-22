#ifndef SMTPCONNECTION_H_
#define SMTPCONNECTION_H_

#include "SMTPstate.h"
#include "Globals.h"
#include "SMTPevent.h"
#include "UserCollection.h"
#include "PieceOfMail.h"
#include "connection.h"
#include <iostream>
#include <unistd.h> // sleep
#include <string>
#include <vector> 

/*
 * Class definition of class representing an SMTP session.
 * Built as a FSM. Has an array of states. Each state is 
 * implemented as its own class, that inherits from the 
 * same state interface. 
 */
class SMTPsession {
	public:
		// Constructor takes vector of states, collection of user and a connection.
		// The connection struct contains socket and context. 
		SMTPsession(std::vector<SMTPState*>, UserCollection*, struct connection*);
  		~SMTPsession();

		// Runs throughout session
  		void Run();

		// End session with client. 
  		void Close();

		// Send reply to client
  		void Reply(int replycode);

		// Parse string received from socket and generate 
		// an event that the session FSM understands. 
  		SMTPevent* ProcessRequest(std::string buffer);	
  		
		// Call state function
		void StateAction();

		// Change state, SMTPevent* contains
		// enum corresponding to next state and
		// other data. This way change of state
		// is changing index in an array
  		void ChangeState(SMTPevent*);
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
  		SMTPevent* currentevent; 
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
