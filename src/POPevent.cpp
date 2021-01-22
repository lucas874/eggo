#include "POPevent.h"

POPevent::POPevent(enum POP_Events event, std::string data) {
    _event = event;
    _data = data;
    if(_event < 2)
	    stateNumber = POP_STATE_AUTH;
    if(_event > 1 && _event < 8)
	    stateNumber = POP_STATE_TRAN;
    else 
	    stateNumber = POP_STATE_UPDATE;
}

std::string POPevent::getData() {
  return _data;
}

int POPevent::getEventNo() {
  return _event;
}

int POPevent::getStateNo() {
	return stateNumber;
}




 
