#include "POPstates.h"
#include "POPsession.h"

void POPauthorization::Action(POPsession* ps, POPevent* e) {
	

	if(e->getEventNo() == 0) {
		existingUser = checkUser(ps, e->getData());
		if(existingUser)
			ps->Reply(USER_OK);
		else
			ps->Reply(USER_ERR);
	}
	else if(e->getEventNo() == 1) {
		if(existingUser) {
			authorized = checkPass(ps, e->getData());
			if(authorized)
				ps->Reply(PASS_OK);
			else
				ps->Reply(PASS_ERR);
		}
		else 
			ps->Reply(PASS_ERR);
	}
	else if(e->getEventNo() == P_NOOP) 
		ps->Reply(REPLY_OK);
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

int POPtransaction::getDeletedSizeOctets(POPsession* ps) {
	int sz = 0;
	for(auto &itr : ps->markedAsDeleted) {
	       sz += ps->currentUser->getMailSize(itr); 
	}	
	return sz;
}
//	       0     1     2     3     4     5     6     7     8
//enum POPevents {USER, PASS, STAT, LIST, RETR, DELE, NOOP, RSET, QUIT};

void POPtransaction::Action(POPsession* ps, POPevent* e) {
	std::string buffer;
	int replycode;

	/*
	 * Handles different commands
	 */

	switch(e->getEventNo()) {

		// Handle STAT command
		case POP_STAT:
	    		replycode = REPLY_OK;	
			buffer = std::to_string(ps->currentUser->getInboxSize() - ps->markedAsDeleted.size());
			buffer += " ";
			buffer += std::to_string(ps->currentUser->getInboxSizeOctets() - getDeletedSizeOctets(ps));	
			break;

		// Handle LIST command
		case POP_LIST:		
			if(e->getData().length() != 0) {
				int index = std::stoi(e->getData());
				int tmp = ps->getCurrentUser()->getMailSize(index);
				if(tmp == -1 || ps->markedAsDeleted.find(index) != ps->markedAsDeleted.end()) {
					buffer = "No such mail exists.";
					replycode = REPLY_ERR;
					
				}
				else {
					buffer = index;
					buffer += " ";
					buffer += std::to_string(tmp);
					replycode = REPLY_OK;
				}	
			}
			else {
				int sz = ps->getCurrentUser()->getInboxSize() - ps->markedAsDeleted.size();
				if(sz == 0) {
					buffer = "Inbox empty";
				}
				else {
					for(int i = 0; sz + ps->markedAsDeleted.size(); ++i) {
						if(ps->markedAsDeleted.find(i) == ps->markedAsDeleted.end()) {
							buffer += std::to_string(i);
							buffer += " ";
							buffer += std::to_string(ps->getCurrentUser()->getMailSize(i));
							buffer += "\n";
						}

					}	
				}
				replycode = REPLY_OK;
			}
			break;

		// Handle RETR command
		case POP_RETR:
			if(e->getData().length() == 0) {
				buffer = "Please provide an argument.";
				replycode = RETR_ERR;
			}
			else {
				int index = std::stoi(e->getData());
				int tmp = ps->getCurrentUser()->getMailSize(index);
				if(tmp == -1 || ps->markedAsDeleted.find(index) != ps->markedAsDeleted.end()) {
					buffer = "No such mail exists.";
					replycode = REPLY_ERR;
					
				}
				else {
					buffer = std::to_string(tmp);
					buffer += " octets.";
					buffer += ps->getCurrentUser()->getMailContent(index);
					buffer += "\n.\n";
					replycode = REPLY_OK;
				}
			}
			break;
		
		// Handle DELE command
		case POP_DELE:
			if(e->getData().length() == 0) {
				buffer = "Please provide an argument.";
				replycode = REPLY_ERR;
			}
			else {
				int index = std::stoi(e->getData());
				int tmp = ps->getCurrentUser()->getMailSize(index);
				if(tmp == -1) {
					buffer = "No such mail exists.";
					replycode = REPLY_ERR;
					
				}
				else {
					if(ps->markedAsDeleted.insert(index).second) {
						buffer = "Message ";
						buffer += std::to_string(index);
						buffer += " deleted.";
						replycode = REPLY_OK;
					}
					else {
						buffer = "Message ";
						buffer += std::to_string(index);
					       	buffer += " already deleted";
						replycode = REPLY_ERR;
					}
				}
			}
			break;

		// Handle RSET command
		case POP_RSET:
			ps->markedAsDeleted.clear();
			replycode = REPLY_OK;	
			buffer = std::to_string(ps->currentUser->getInboxSize());
			buffer += " ";
			buffer += std::to_string(ps->currentUser->getInboxSizeOctets());
			break;
		
		// Handle NOOP command
		case POP_NOOP:
			buffer = "";
			replycode = REPLY_OK;
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

void POPupdate::Action(POPsession* ps, POPevent* ) {
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
	ps->Reply(REPLY_OK, buffer);
}

void POPupdate::ChangeState(POPsession* ps, int n) {
	if(allowedTransitions[n])
		ps->currentState = ps->states[n];
}

int POPupdate::getStateNo() {
	return stateNo;
}


