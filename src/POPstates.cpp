#include "POPstates.h"
#include "POPsession.h"

/*             0     	      1     	   2     
 * enum Event {AUTHORIZATION, TRANSACTION, UPDATE};
 *
 */


void POPauthorization::Action(POPsession* ps, Event* e) {
	std::cout << "Hello from AUTHORIZATION state" << std::endl;

	if(e->getEventNo() == 0) {
		existingUser = checkUser(ps, e->getData());
		if(existingUser)
			ps->Reply(USER_OK);
		else
			ps->Reply(USER_ERR);
	}
	else if(e->getEventNo() == 1) {
		authorized = checkPass(ps, e->getData());
		if(authorized)
			ps->Reply(PASS_OK);
		else
			ps->Reply(PASS_ERR);
	}
	else 
		ps->Reply(BAD_CMD_SEQ);


	if(existingUser && authorized)
		ps->currentState = ps->states[1];
}

void POPauthorization::ChangeState(POPsession* ps, int n) {
	if(allowedTransitions[n])
		ps->currentState = ps->states[n];
}

int POPauthorization::getStateNo() {
	return stateNo;
}

bool POPauthorization::checkUser(POPsession* ps, std::string name) {
	User* tmp = ps->uc->LookUp(name);
	if(tmp != nullptr) {		
		ps->setCurrentUser(tmp);
		return true;
	}
	return false;
}

bool POPauthorization::checkPass(POPsession* ps, std::string pass) {	
	if(pass.compare(ps->getCurrentUser()->getPass()) == 0)
		return true;
	else 
		return false;
}

//	       0     1     2     3     4     5     6     7     8
//enum Events {USER, PASS, STAT, LIST, RETR, DELE, NOOP, RSET, QUIT};

void POPtransaction::Action(POPsession* ps, Event* e) {
	std::string buffer;
	int replycode;

	/*
	 * Handles different commands
	 */

	switch(e->getEventNo()) {

		// Handle STAT command
		case POP_STAT:
	    		replycode = STAT_OK;	
			buffer = std::to_string(ps->currentUser->getInboxSize());
			buffer += " ";
			buffer += std::to_string(ps->currentUser->getInboxSizeOctets());
			buffer += "\n";
			
			//ps->Reply(STAT_OK);
			break;

		// Handle LIST command
		case POP_LIST:		
			if(e->getData().length() != 0) {
				int tmp = ps->getCurrentUser()->getMailSize(std::stoi(e->getData()));
				if(tmp == -1) {
					buffer = "No such mail exists.\n";
					replycode = LIST_ERR;
					
				}
				else {
					buffer = e->getData();
					buffer += " ";
					buffer += std::to_string(tmp);
					replycode = LIST_OK;
				}	
			}
			else {
				int sz = ps->getCurrentUser()->getInboxSize();
				if(sz == 0) {
					buffer = "Inbox empty\n";
				}
				else {
					for(int i = 0; i < sz; i++) {
						buffer += std::to_string(i);
						buffer += " ";
						buffer += std::to_string(ps->getCurrentUser()->getMailSize(i));
						buffer += "\n";
					} 
				}
				replycode = LIST_OK;
			}

			//ps->Reply(replycode, buffer);
			break;

		// Handle RETR command
		case POP_RETR:
			if(e->getData().length() == 0) {
				buffer = "Please provide an argument.\n";
				replycode = RETR_ERR;
			}
			else {
				int index = std::stoi(e->getData());
				int tmp = ps->getCurrentUser()->getMailSize(index);
				if(tmp == -1) {
					buffer = "No such mail exists.\n";
					replycode = RETR_ERR;
					
				}
				else {
					buffer = std::to_string(tmp);
					buffer += " octets. \n";
					buffer += ps->getCurrentUser()->getMailContent(index);
					buffer += "\n.\n";
					replycode = RETR_OK;
				}
			}
			break;
		case POP_DELE:
			if(e->getData().length() == 0) {
				buffer = "Please provide an argument.\n";
				replycode = DELE_ERR;
			}
			else {
				int index = std::stoi(e->getData());
				int tmp = ps->getCurrentUser()->getMailSize(index);
				if(tmp == -1) {
					buffer = "No such mail exists.\n";
					replycode = DELE_ERR;
					
				}
				else {
					if(ps->markedAsDeleted.insert(index).second) {
						buffer = "Message ";
						buffer += std::to_string(index);
						buffer += " deleted.\n";
						replycode = DELE_OK;
					}
					else {
						buffer = "Message ";
						buffer += std::to_string(index);
					       	buffer += " already deleted\n";
						replycode = DELE_ERR;
					}
				}
			}
		case POP_RSET:
			ps->markedAsDeleted.clear();
			replycode = STAT_OK;	
			buffer = std::to_string(ps->currentUser->getInboxSize());
			buffer += " ";
			buffer += std::to_string(ps->currentUser->getInboxSizeOctets());
			buffer += "\n";
			break;
		case POP_NOOP:
			buffer = "";
			replycode = STAT_OK;
			break;
	}
	ps->Reply(replycode, buffer);
}

void POPtransaction::ChangeState(POPsession* ps, int n) {
	if(allowedTransitions[n])
		ps->currentState = ps->states[n];
}

int POPtransaction::getStateNo() {
	return stateNo;
}

void POPupdate::Action(POPsession* ps, Event* ) {
	for(auto &i : ps->markedAsDeleted) 
		ps->currentUser->deleteMail(i);
	
	std::string buffer;
	int size = ps->currentUser->getInboxSize();

	if(size > 0) {
		buffer = "POP3 server signing off (";
		buffer += std::to_string(size);
		buffer += " message(s) left)\n";
	}
	else {
		buffer = "POP3 server signing off (maildrop empty)\n";
	}
	ps->run = false;
	ps->Reply(STAT_OK, buffer);
}

void POPupdate::ChangeState(POPsession* ps, int n) {
	if(allowedTransitions[n])
		ps->currentState = ps->states[n];
}

int POPupdate::getStateNo() {
	return stateNo;
}


