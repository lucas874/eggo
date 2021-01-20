#include <iostream>
#include "SMTPsession.h"
#include "SMTP_States.h"
#include "UserCollection.h"

using namespace std;

vector<SMTPState*> initializeV() {
  vector<SMTPState*> v(7);
  v[0] = new SMTPInit();
  v[1] = new SMTPHelo();
  v[2] = new SMTPMail;
  v[3] = new SMTPRcpt;
  v[4] = new SMTPData;
  v[5] = new SMTPRset;
  v[6] = new SMTPQuit;

  return v;
}
  
int main() {
  
  vector<SMTPState*> v = initializeV();

  
  UserCollection *uc = new UserCollection;
  //UserCollection uc();
  
  User *a_user = new User("Alice", "1234");
  User *b_user = new User("Bob", "Kodeord");

  uc->addUser(a_user);
  uc->addUser(b_user);
  

  
  SMTPsession SC(v, uc);

  //SC.StateAction();

  SC.Run();
  //enum Event e = MAIL;
  //string str = "bjarnebjarne";

  //EventData ed(e, str);
  //SC.ChangeState(&ed);
  //SC.StateAction();
}
