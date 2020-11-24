#include "SMTPState.h"
#include <iostream>

class SMTPInit : public SMTPState {
public:
  void Action() override;
};

void SMTPInit::Action() {
  std::cout << "hello from Init Action()" << std::endl;
}
  
class SMTPHelo : public SMTPState {
public:
  void Action() override;
};


void SMTPHelo::Action() {
  std::cout << "hello from Helo Action()" << std::endl;
}

class SMTPMail : public SMTPState {
public:
  void Action() override;
};


void SMTPMail::Action() {
  std::cout << "hello from Mail Action()" << std::endl;
}

class SMTPRcpt : public SMTPState {
public:
  void Action() override;
};


void SMTPRcpt::Action() {
  std::cout << "hello from Rcpt Action()" << std::endl;
}

class SMTPData : public SMTPState {
public:
  void Action() override;
};


void SMTPData::Action() {
  std::cout << "hello from Data Action()" << std::endl;
}

class SMTPRset : public SMTPState {
public:
  void Action() override;
};


void SMTPRset::Action() {
  std::cout << "hello from Rset Action()" << std::endl;
}

class SMTPQuit : public SMTPState {
public:
  void Action() override;
};


void SMTPQuit::Action() {
  std::cout << "hello from Quit Action()" << std::endl;
}
