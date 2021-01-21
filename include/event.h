#ifndef EVENT_H
#define EVENT_H
#include "Globals.h"
#include <iostream>

//	         0       1       2       3       4       5       6       7       8
enum POP_Events {P_USER, P_PASS, P_STAT, P_LIST, P_RETR, P_DELE, P_NOOP, P_RSET, P_QUIT};

class Event {
	public:
		Event(enum POP_Events e, std::string data);
		int getEventNo();
		int getStateNo();

		std::string getData();
	private:
		enum POP_Events _event;
		std::string _data;
		int stateNumber; 
};

#endif
