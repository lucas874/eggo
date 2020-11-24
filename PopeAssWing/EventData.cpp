#include "EventData.h"

EventData::EventData(enum Event e, std::string str) {
    _event = e;
    _data = str;
}

std::string EventData::getData() {
  return this->_data;
}

int EventData::getEventNo() {
  return this->_event;
}






  
