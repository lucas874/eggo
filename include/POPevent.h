#ifndef _POPEVENT_H_
#define _POPEVENT_H_
#include "globals.h"
#include <iostream>

//	         0       1       2       3       4       5       6       7       8       9
enum POP_Events {P_USER, P_PASS, P_STAT, P_LIST, P_RETR, P_DELE, P_NOOP, P_RSET, P_QUIT, P_BAD};

class POPevent {
	public:
		POPevent(enum POP_Events e, std::string data);
		int getEventNo();
		int getStateNo();

		std::string getData();
	private:
		enum POP_Events _event;
		std::string _data;
		int stateNumber; 
};

#endif
