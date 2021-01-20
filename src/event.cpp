#include "event.h"

Event::Event(enum POP_Events event, std::string data) {
    _event = event;
    _data = data;
    for(int i = 0; i < 8; i++) {
	    if(i < 2)
		    stateNumbers[i] = POP_STATE_AUTH;
	    else
		    stateNumbers[i] = POP_STATE_TRAN;
    }
    stateNumbers[8] = POP_STATE_UPDATE;

}

std::string Event::getData() {
  return _data;
}

int Event::getEventNo() {
  return _event;
}

int Event::getStateNo() {
	return stateNumbers[getEventNo()];
}




 
