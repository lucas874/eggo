#include "SMTP_event.h"

SMTP_event::SMTP_event(enum SMTP_event_enum e, std::string d) {
    event = e;
    data = d;
}

std::string SMTP_event::getData() {
  return data;
}

int SMTP_event::getEventNo() {
  return event;
}






  
