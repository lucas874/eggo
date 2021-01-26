#ifndef POPSESSION_H
#define POPSESSION_H

#include "POPstate.h"
#include "POPevent.h"
#include "globals.h"
#include "UserCollection.h"
#include "connection.h"
#include <unistd.h>
#include <set>


/*
 * Interface for the POPsession class
 */

class POPsession {
	public:
		POPsession(std::vector<POPstate*>, UserCollection*, struct connection*);
		~POPsession(){};
		void Run();
		void ChangeState(POPevent*);
		void StateAction();
		void Reply(int);
		void Reply(int, std::string);
		void Close();
		POPevent* ProcessRequest(std::string buffer);	
		void setCurrentUser(User*);
		User* getCurrentUser();

	private:
		void ChangeState(POPstate*);
		friend class POPState;
  		friend class POPauthorization;
  		friend class POPtransaction;
  		friend class POPupdate;
  		
		POPstate* currentState;
		std::vector<POPstate*> states;
		UserCollection* uc;
		zmq::context_t context;
		zmq::socket_t socket;
		POPevent* currentevent;
		User* currentUser;
		struct connection *_connection;

		bool run = true;

		std::set<int> markedAsDeleted; // Stores indices to the inbox of a user to be deleted 
					       //Set used because a piece of mail can on
};
#endif 
