#ifndef POPSESSION_H
#define POPSESSION_H

#include "POPstate.h"
#include "event.h"
#include "Globals.h"
#include "UserCollection.h"
#include "connection.h"
#include <unistd.h>
#include <set>

class POPsession {
	public:
		POPsession(std::vector<POPstate*>, UserCollection*, struct connection*);
		~POPsession(){};
		void Run();
		void ChangeState(Event*);
		void StateAction();
		void Reply(int);
		void Reply(int, std::string);
		void Close();
		Event* ProcessRequest(std::string buffer);	
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
		Event* currentevent;
		User* currentUser;
		struct connection *_connection;

		bool run = true;

		std::set<int> markedAsDeleted; // Stores indices to the inbox of a user to be deleted 
					       //Set used because a piece of mail can on
};
#endif 
