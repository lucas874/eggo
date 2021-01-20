#include "SMTPsession.h"
#include "POPsession.h"
#include "POPstates.h"
#include "SMTP_states.h"

#include <vector>
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
};
