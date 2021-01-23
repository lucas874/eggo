#ifndef SMTPCONNECTION_H_
#define SMTPCONNECTION_H_

#include "SMTPstate.h"
#include "globals.h"
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
 *
 * Instantiated when a connection on the socket for SMTP 
 * session is established.  
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

		// Set sender domain for current session
  		void setSenderDomain(std::string);

		// Send sender username for current session
  		void setSenderUsername(std::string);

		// Set domain for a recipient
  		void setRcptDomain(std::string);

		// Set username for a recipient 
  		void setRcptUsername(std::string);

		// Used to access data from events
  		std::string getCurData();
	
	private:
		/*
		 * State classes are friends of 
		 * SMTPsession so they can access private fields. 
		 */
		friend class SMTPState;
  		friend class SMTPInit;
  		friend class SMTPHelo;
  		friend class SMTPMail;
  		friend class SMTPRcpt;
  		friend class SMTPData;
  		friend class SMTPQuit;
  		friend class SMTPRset;

		// Current state of session	
  		SMTPState* currentState;

		// Pointer to collection of users
  		UserCollection* _uc;

		// Array of states
  		std::vector<SMTPState*> states;

		// Current event
  		SMTPevent* currentevent;

		// Sender domain of current session	
  		std::string senderDomain;

		// Sender username
  		std::string senderUsername;

		// It's possible to have multiple recipients
		// We can only deliver mails to the domain of 
		// server... currently
		std::vector<std::string> rcptDomain;
  		std::vector<std::string> rcptUsername;
  		std::vector<User*> rcpt;

		// Instantiated if MAIL command is received
  		PieceOfMail* curmail;
  		std::string currentData;

		// Contains socket and context
  		struct connection *_connection;

		// Main loop runs while this is true
		// false when QUIT command is received
  		bool run = true;
};

#endif
