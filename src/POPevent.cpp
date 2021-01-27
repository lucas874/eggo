#include "POPevent.h"

POPevent::POPevent(int event, std::string data) {
    _event = event;
    _data = data;
    if(_event < 2) // is either POP_USER or POP_PASS
	    stateNumber = POP_STATE_AUTH;
    if(_event > 1 && _event < 8) // is any of the TRANSACTION state commands 
	    stateNumber = POP_STATE_TRAN;
    else // go to UPDATE if QUIT command
	    stateNumber = POP_STATE_UPDATE;
}

/*
 * public methods for getting data, event and state number
 */
std::string POPevent::getData() {
  return _data;
}

int POPevent::getEventNo() {
  return _event;
}

int POPevent::getStateNo() {
	return stateNumber;
} 
