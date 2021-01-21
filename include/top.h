#include "SMTPsession.h"
#include "POPsession.h"
#include "POPstates.h"
#include "SMTP_states.h"
#include "editUserCollection.h"

class Top {
	public:
		Top(UserCollection*);
		void listen();
		std::vector<SMTPState*> initializeSMTPstates();
		std::vector<POPstate*> initializePOPstates();
		SMTPsession* createSMTPsession();
		POPsession* createPOPsession();

	private:
		UserCollection* usercollection;
		struct connection *connection1;
		struct connection *connection2;
		struct connection *connection3;
		EditUsers* editUsersObject;
};
