#ifndef POPSTATE_H
#define POPSTATE_H
#include "event.h"
class POPsession;

class POPstate {
	public:
		virtual void Action(POPsession*, Event*) = 0;
  		virtual void ChangeState(POPsession*, int) = 0;
  		virtual int getStateNo() = 0;
};



#endif 

