#include "SMTPstates.h"
#include "SMTPsession.h"

// Reply that service is ready
void SMTPInit::Action(SMTPsession* sc, SMTPevent* e) {	
	if(e->getEventNo() == SMTP_HELO) // 1 corresponds to Helo	
		sc->Reply(MAIL_ACTION_OK);
	else if(e->getEventNo() == SMTP_QUIT) {
		sc->Reply(SERVICE_CLOSING);
		ChangeState(sc, SMTP_QUIT);
	}
	else
		sc->Reply(BAD_CMD_SEQUENCE); // Cannot do this in current state

}

void SMTPInit::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n]) // If transition is allowed 
		sc->currentState = sc->states[n]; // Change state. If transition not allowed
						  // no transition occurs.
}
/*
 * This method is used to give the states a notion of their index in the 
 * state vector of the SMTP session object.
 */
int SMTPInit::getStateNo() {
	return stateNo;
}

void SMTPHelo::Action(SMTPsession* sc, SMTPevent* e) {	
	if(e->getEventNo() == SMTP_QUIT) {
		sc->Reply(SERVICE_CLOSING);
		ChangeState(sc, SMTP_QUIT);
	}
	else if(e->getData().size() == 0) {
		sc->Reply(BAD_CMD_SEQUENCE);
	}
	else if(e->getEventNo() == SMTP_HELO) 
		sc->Reply(MAIL_ACTION_OK); // Received helo
	else
		sc->Reply(BAD_CMD_SEQUENCE); // Command can be issued in current state
}

void SMTPHelo::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n]) // Change state if transition allowed
		sc->currentState = sc->states[n];
}

int SMTPHelo::getStateNo() {
	return stateNo;
}


void SMTPMail::Action(SMTPsession* sc, SMTPevent* e) {
	
	if(e->getEventNo() == SMTP_MAIL) {	
		std::string str = e->getData();

		size_t pos = str.find("@");
		if(pos == std::string::npos) {
			sc->Reply(SYNTAX_ERROR);
			return;
		}
		/*
  		int i = 0;
  		while(str[i] != '@')
    			i++;
		*/
		sc->setSenderUsername(str.substr(0,pos));
		sc->curmail = new PieceOfMail();
  		sc->curmail->setsender(str);
		sc->setSenderDomain(str.substr(pos, str.size()-pos));
  		sc->Reply(MAIL_ACTION_OK);
	}
	else
		sc->Reply(BAD_CMD_SEQUENCE);
}

void SMTPMail::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])  // Change state if allowed, else do not change state
		sc->currentState = sc->states[n];
}

int SMTPMail::getStateNo() {
	return stateNo;
}


/*
 * Receive a recipent. 
 * If design was better, we could do without some of the if statements.
 */
void SMTPRcpt::Action(SMTPsession* sc, SMTPevent* e) {	
	
	if(e->getEventNo() == SMTP_SUBJ) {
		sc->curmail->setSubject(e->getData());
 		sc->Reply(MAIL_ACTION_OK);
		return;
	}		
	else if(e->getEventNo() == SMTP_RCPT) {
		// Throws error if not formatted correcly fix....
		std::string str = e->getData();	
  		std::string domain;
  		std::string name;

  		int i;
		for(i = 0; i < str.size(); i++) {
			if(str[i] == '@')
				break;
		}

		if(i == str.size()) {
			sc->Reply(SYNTAX_ERROR);
			return;
		}  
  		name = str.substr(0,i);
  		domain = str.substr(i+1, str.size()-name.size());	
  		if(domain.compare(DOMAIN_NAME) != 0) {
			sc->Reply(551); // We can only deliver mail to local users
			return;
		}
		else {
			User* u = sc->_uc->LookUp(name); 
			if (u != nullptr) { // If user exists
		  	sc->rcpt.push_back(u);
		  	sc->curmail->setrcpt(name);
		  	sc->Reply(MAIL_ACTION_OK);
			}
			else 
				sc->Reply(USER_NOT_LOCAL); // If user doesn't exist
		}	
	}
	else  {  // If we tried to change state, but it wasn't allowed
		sc->Reply(BAD_CMD_SEQUENCE); // control goes here. This is how we process the
		return;                     // command in such a case. 
	}
}

void SMTPRcpt::ChangeState(SMTPsession* sc, int n) { // Change state
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPRcpt::getStateNo() {
	return stateNo;
}

void SMTPData::Action(SMTPsession* sc, SMTPevent* e) {
	if(e->getEventNo() != SMTP_DATA) {  // If we tried to change state, but it wasn't allowed
		sc->Reply(BAD_CMD_SEQUENCE); // control goes here. This is how we process the
		return;
	}
	else {
		if(sc->curmail->getContent().empty()) {
			sc->curmail->setHeader(); // Add lines From: Subject: To:	
			sc->Reply(354);
		}
		else {
			if(e->getData().compare("\\n.\\n") == 0) {
				sc->curmail->append(e->getData());
				sc->Reply(END_OF_DATA);
				ChangeState(sc, 1);
			}
			else {
				sc->curmail->append(e->getData()); // Append if piece of mail is not empty
				sc->Reply(MAIL_ACTION_OK);
			}
		}
	}
}
	
void SMTPData::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPData::getStateNo() {
	return stateNo;
}

void SMTPRset::Action(SMTPsession* sc, SMTPevent* e) {

	// Reset data fields of session.	
	sc->senderDomain.clear();
	sc->senderUsername.clear();
	sc->rcpt.clear();
	sc->rcptDomain.clear();
	sc->rcptUsername.clear();
	delete sc->curmail;
	sc->currentState = sc->states[1];
		
	sc->Reply(MAIL_ACTION_OK);
}

void SMTPRset::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}
int SMTPRset::getStateNo() {
	return stateNo;
}

void SMTPQuit::Action(SMTPsession* sc, SMTPevent* e) {
	
	/*
	 * This is where the piece of mail gets delivered to the inbox of users.
	 * It would have been a better solution to store these in a queue and 
	 * have a separate component observing this queue deliver it to the recepient(s).
	 *
	 */
	for(auto &itr : sc->rcpt)
		sc->_uc->addMailToInbox(itr, sc->curmail); 

	// Tell client that server acknowledges that transmission is over.	
	sc->Reply(SERVICE_CLOSING);
  	sc->run = false; // Stop running Run() loop of session
}

void SMTPQuit::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPQuit::getStateNo() {
	return stateNo;
}
