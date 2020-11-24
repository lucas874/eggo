
#include "SMTPState.h"

//class SMTPConnection;
// inject SMTPConnection in constructor!!
void SMTPState::Close(SMTPConnection*) { }
void SMTPState::Send(SMTPConnection*) { }
void SMTPState::Action() { }

/*void SMTPState::ChangeState(SMTPConnection* c, SMTPState* s) {
  c->ChangeState(s);
}
*/

SMTPState* SMTPState::_instance = 0;

SMTPState::SMTPState() {}

SMTPState* SMTPState::instance() {
  if(instance == 0)
    _instance = new SMTPState;
  return _instance;
}

