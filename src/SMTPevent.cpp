#include "SMTPevent.h"

SMTPevent::SMTPevent(enum SMTP_event_enum e, std::string d) {
    event = e;
    data = d;
}

std::string SMTPevent::getData() {
  return data;
}

int SMTPevent::getEventNo() {
  return event;
}






  
