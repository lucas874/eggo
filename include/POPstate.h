#ifndef _POPSTATE_H_
#define _POPSTATE_H_

#include "POPevent.h"


class POPsession;

class POPstate {
	public:
		virtual void Action(POPsession*, POPevent*) = 0;
  		virtual void ChangeState(POPsession*, int) = 0;
  		virtual int getStateNo() = 0;
};



#endif 

