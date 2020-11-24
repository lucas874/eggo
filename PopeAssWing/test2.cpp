#include "SMTP_States.h"

int main() {
  //SMTPState*s = new SMTPState;
  SMTPInit* s = SMTPInit::instance();
  s->Action();
}

  
