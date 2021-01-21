#ifndef POPSTATES_H
#define POPSTATES_H

#include <iostream>
#include <vector>
#include "POPstate.h"
#include "Globals.h"
/*             0     	      1     	   2     
 * enum Event {AUTHORIZATION, TRANSACTION, UPDATE};
 *
 */

class POPauthorization : public POPstate {
	public:
		void Action(POPsession* ps, Event* e) override;
  		void ChangeState(POPsession* ps, int n) override;
  		int getStateNo() override; 
		bool checkUser(POPsession*, std::string);
		bool checkPass(POPsession*, std::string);


	private:
		std::vector<bool> allowedTransitions = 
		{true, true, false};
		bool existingUser;
		bool authorized;

		int stateNo = 0;
		
};

class POPtransaction : public POPstate {
	public:
		void Action(POPsession* ps, Event* e) override;
  		void ChangeState(POPsession* ps, int n) override;
  		int getStateNo() override; 
		int getDeletedSizeOctets(POPsession* ps);


	private:
		std::vector<bool> allowedTransitions = 
		{true, true, true};

		int stateNo = 1;
};


class POPupdate : public POPstate {
	public:
		void Action(POPsession* ps, Event* e) override;
  		void ChangeState(POPsession* ps, int n) override;
  		int getStateNo() override; 



	private:
		std::vector<bool> allowedTransitions = 
		{true, true, false};

		int stateNo = 2;
};


#endif  
