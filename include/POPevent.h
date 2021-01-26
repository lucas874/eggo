#ifndef _POPEVENT_H_
#define _POPEVENT_H_
#include "globals.h"
#include <iostream>


/*
 * Pop event class definition
 */ 
class POPevent {
	public:
		POPevent(int e, std::string data);
		int getEventNo();
		int getStateNo();

		std::string getData();
	private:
		int _event;
		std::string _data;
		int stateNumber; 
};

#endif
