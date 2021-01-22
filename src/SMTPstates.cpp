#include "SMTPstates.h"
#include "SMTPsession.h"

// Reply that service is ready
void SMTPInit::Action(SMTPsession* sc, SMTPevent* e) {
	sc->Reply(220);
}

void SMTPInit::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPInit::getStateNo() {
	return stateNo;
}

void SMTPHelo::Action(SMTPsession* sc, SMTPevent* e) {
	
	
	std::string str = sc->getCurData().substr(5);
  	sc->setSenderDomain(str);
  
  	sc->Reply(250);
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
  sc->Reply(250);

}

void SMTPMail::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->currentState = sc->states[n];
}

int SMTPMail::getStateNo() {
	return stateNo;
}

void SMTPRcpt::Action(SMTPsession* sc, SMTPevent* e) {
  std::cout << "hello from Rcpt Action()" << std::endl;
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
    sc->rcpt.push_back(sc->_uc->LookUp(name));
  } 
  sc->curmail->setrcpt(name);
  sc->Reply(250);
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
		sc->Reply(250);
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
		
	sc->Reply(250);
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
