#ifndef POPSTATES_H
#define POPSTATES_H

#include <iostream>
#include <vector>
#include "POPstate.h"
#include "globals.h"

/*
 * Interface for the states of the POP3 server
 */

// AUTHORIZATION state class
class POPauthorization : public POPstate {
	public:
		void Action(POPsession* ps, POPevent* e) override;
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

// TRANSITION state class
class POPtransaction : public POPstate {
	public:
		void Action(POPsession* ps, POPevent* e) override;
  		void ChangeState(POPsession* ps, int n) override;
  		int getStateNo() override; 
		int getDeletedSizeOctets(POPsession* ps);
		void transmitContent(POPsession*, int);


	private:
		std::vector<bool> allowedTransitions = 
		{true, true, true};

		int stateNo = 1;
};

// UPDATE state class
class POPupdate : public POPstate {
	public:
		void Action(POPsession* ps, POPevent* e) override;
  		void ChangeState(POPsession* ps, int n) override;
  		int getStateNo() override; 



	private:
		std::vector<bool> allowedTransitions = 
		{true, true, false};

		int stateNo = 2;
};


#endif  
