#include "SMTPstates.h"
#include "SMTPsession.h"

// Reply that service is ready
void SMTPInit::Action(SMTPsession* sc, SMTPevent* e) {	
	if(e->getEventNo() == SMTP_HELO) // 1 corresponds to Helo	
		sc->Reply(MAIL_ACTION_OK);
	else if(e->getEventNo() == SMTP_QUIT) // Quit
		sc->Reply(SERVICE_CLOSING);
	else
		sc->Reply(BAD_CMD_SEQUENCE); // Cannot do this in current state

}

void SMTPInit::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPInit::getStateNo() {
	return stateNo;
}

void SMTPHelo::Action(SMTPsession* sc, SMTPevent* e) {
	
		
	if(e->getData().size() == 0) {
		sc->Reply(BAD_CMD_SEQUENCE);
		return;
	}
	else 
		sc->setSenderDomain(e->getData());	

	/*
	 * At this point the SMTP session has changed its state according to the
	 * received command. It might seem redundant to check what command was received
	 * again, but this is necessary because the transition could have been faulty/
	 * not allowed. Necessary in this design at least...
	 */
	if(e->getEventNo() == SMTP_HELO) 
		sc->Reply(MAIL_ACTION_OK); // Received helo ok
	else if(e->getEventNo() == SMTP_QUIT)
		sc->Reply(SERVICE_CLOSING);
	else
		sc->Reply(BAD_CMD_SEQUENCE);	
}

void SMTPHelo::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPHelo::getStateNo() {
	return stateNo;
}


void SMTPMail::Action(SMTPsession* sc, SMTPevent* e) {	
	std::string str = sc->getCurData();
  	int i = 0;
  	while(str[i] != '@')
    		i++;

  	std::string name;
  	name = str.substr(0,i);

  	sc->setSenderUsername(name);
  	sc->curmail = new PieceOfMail();
  	sc->curmail->setsender(name);
  	sc->Reply(MAIL_ACTION_OK);

}

void SMTPMail::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPMail::getStateNo() {
	return stateNo;
}

void SMTPRcpt::Action(SMTPsession* sc, SMTPevent* e) { 
  std::string str = sc->getCurData();
  std::string domain;
  std::string name;

  int i = 0;
  while(str[i] != '@')
    i++;

  
  name = str.substr(0,i);
  domain = str.substr(i+1, str.size()-name.size());
 
  if(domain.compare(DOMAIN_NAME) != 0) {
    sc->Reply(551);
    return;
  }
  else {
	  User* u = sc->_uc->LookUp(name);
	  if (u != nullptr) {
		  sc->rcpt.push_back(u);
		  sc->curmail->setrcpt(name);
		  sc->Reply(MAIL_ACTION_OK);
	  }
	  else 
		  sc->Reply(USER_NOT_LOCAL);
  } 
}

void SMTPRcpt::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPRcpt::getStateNo() {
	return stateNo;
}

void SMTPData::Action(SMTPsession* sc, SMTPevent* e) {
	if(sc->curmail->getContent().empty()) {
		
		sc->curmail->setHeader();	
		sc->Reply(354);
	}
	else {
		sc->curmail->append(e->getData());
		sc->Reply(MAIL_ACTION_OK);
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
  for(auto &itr : sc->rcpt) {
	  sc->_uc->addMailToInbox(itr, sc->curmail); 
  }
  sc->Reply(221);
  sc->run = false;
}

void SMTPQuit::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPQuit::getStateNo() {
	return stateNo;
}
