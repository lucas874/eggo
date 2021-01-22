#ifndef _TOP_H_
#define _TOP_H_

#include "SMTPsession.h"
#include "POPsession.h"
#include "POPstates.h"
#include "SMTPstates.h"
#include "editUserCollection.h"


/*
 * Top server with three sockets.
 * Listens for connections on these without blocking flow
 * When a connection is established, requested service is instantiated
 *
 */

class Top {
	public:
		// Inject collection of users in constructor
		Top(UserCollection*);

		// Runs indefinitely 
		void listen();

		// Initialize states for SMTP object
		std::vector<SMTPState*> initializeSMTPstates();

		// Initialize states for POP object
		std::vector<POPstate*> initializePOPstates();

		// Instantiate Objects
		SMTPsession* createSMTPsession();
		POPsession* createPOPsession();

	private:

		// Collection of users
		UserCollection* usercollection;

		// Structs containing context and socket
		struct connection *connection1;
		struct connection *connection2;
		struct connection *connection3;	
};

#endif
