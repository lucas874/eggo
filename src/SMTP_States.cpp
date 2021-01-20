#include "SMTP_states.h"
#include "SMTPsession.h"

void SMTPInit::Action(SMTPsession* sc, SMTP_event* e) {
	sc->Reply(220);
}

void SMTPInit::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->_state = sc->states[n];
}

int SMTPInit::getStateNo() {
	return stateNo;
}

void SMTPHelo::Action(SMTPsession* sc, SMTP_event* e) {
	
	
	std::string str = sc->getCurData().substr(5);
  	sc->setSenderDomain(str);
  
  	sc->Reply(250);
}

void SMTPHelo::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->_state = sc->states[n];
}

int SMTPHelo::getStateNo() {
	return stateNo;
}


void SMTPMail::Action(SMTPsession* sc, SMTP_event* e) {
  std::cout << "hello from Mail Action()" << std::endl;
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
		sc->_state = sc->states[n];
}

int SMTPMail::getStateNo() {
	return stateNo;
}

void SMTPRcpt::Action(SMTPsession* sc, SMTP_event* e) {
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
		sc->_state = sc->states[n];
}

int SMTPRcpt::getStateNo() {
	return stateNo;
}

void SMTPData::Action(SMTPsession* sc, SMTP_event* e) {
	if(sc->curmail->getContent().empty()) {
		std::cout << "hello from Data Action()" << std::endl;
		sc->curmail->append("Fra:\nTil:");	
		sc->Reply(354);
	}
	else {
		sc->curmail->append(e->getData());
		sc->Reply(250);
	}
}

void SMTPData::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->_state = sc->states[n];
}

int SMTPData::getStateNo() {
	return stateNo;
}

void SMTPRset::Action(SMTPsession* sc, SMTP_event* e) {
  std::cout << "hello from Rset Action()" << std::endl;
  sc->Reply(250);
}

void SMTPRset::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->_state = sc->states[n];
}
int SMTPRset::getStateNo() {
	return stateNo;
}

void SMTPQuit::Action(SMTPsession* sc, SMTP_event* e) {
  std::cout << "hello from Quit Action()" << std::endl;

  for(auto &itr : sc->rcpt) {
	  sc->_uc->addMailToInbox(itr, sc->curmail); 
  }
  sc->Reply(221);
  sc->run = false;
}

void SMTPQuit::ChangeState(SMTPsession* sc, int n) {
	if(allowedTransitions[n])
		sc->_state = sc->states[n];
}

int SMTPQuit::getStateNo() {
	return stateNo;
}
