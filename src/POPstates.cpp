#include "POPstates.h"
#include "POPsession.h"


/*
 * Handle incoming commands when in the AUTHORIZATION state
 */
void POPauthorization::Action(POPsession* ps, POPevent* e) {

	switch(e->getEventNo()) {
		case POP_USER:
			existingUser = checkUser(ps, e->getData()); // check if the user exists
			if(existingUser) // if true, allow password input
				ps->Reply(USER_OK);
			else
				ps->Reply(USER_ERR);
			break;
		case POP_PASS:
			if(ps->getCurrentUser()) {
				 authorized = checkPass(ps, e->getData()); // check if password matches user
				 if(authorized) // if true, reply and set authorized to true
					 ps->Reply(PASS_OK);
				 else
					 ps->Reply(PASS_ERR);
			}
			else
				ps->Reply(BAD_CMD_SEQ);
			break;
		case POP_NOOP:
			ps->Reply(REPLY_OK); // Handle NOOP command (reply +OK)
			break;
	        case POP_QUIT:
			ps->Reply(QUIT_AUTH_OK);
			ps->run = false;
			break;
		default:
			ps->Reply(BAD_CMD_SEQ); // Reply for any unavailable command in AUTH state
			break;
	}

	if(existingUser && authorized) // go to TRANSACTION state when authorized
		ChangeState(ps, POP_STATE_TRAN);
}

// Public method for changing state
void POPauthorization::ChangeState(POPsession* ps, int n) {
	if(allowedTransitions[n])
		ps->currentState = ps->states[n];
}

// Public method for getting the integer representing the current state
int POPauthorization::getStateNo() {
	return stateNo;
}

// Public method for checking if the user is one that exists
bool POPauthorization::checkUser(POPsession* ps, std::string name) {
	User* tmp = ps->uc->LookUp(name);
	if(tmp != nullptr) {		
		ps->setCurrentUser(tmp);
		return true;
	}
	return false;
}

// Public method for authorizing the current user
bool POPauthorization::checkPass(POPsession* ps, std::string pass) {	
	if(pass.compare(ps->getCurrentUser()->getPass()) == 0)
		return true;
	else 
		return false;
}

// Private method for getting the cumulative size of deleted mails in current session
int POPtransaction::getDeletedSizeOctets(POPsession* ps) {
	int sz = 0;
	for(auto &itr : ps->markedAsDeleted) {
	       sz += ps->currentUser->getMailSize(itr); 
	}	
	return sz;
}


void POPtransaction::transmitContent(POPsession *ps, int index) {
	std::vector<std::string> content;
	content = ps->getCurrentUser()->getMailContent(index);
	for(auto &i : content) {
		zmq::message_t rawrequest;
    		ps->_connection->socket.recv(rawrequest, zmq::recv_flags::none);
    		ps->Reply(CONTENT_TRANSMIT, i);
	}
}
/*
 * Handle incoming commands when in the TRANSACTION state
 *
 * See https://tools.ietf.org/pdf/rfc1939.pdf for functionality
 */
void POPtransaction::Action(POPsession* ps, POPevent* e) {
	std::string buffer;
	int replycode;

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
					buffer = std::to_string(index);
					buffer += " ";
				        if(ps->getCurrentUser()->getReadStatus(index))
					        buffer += "T ";
					else
					        buffer += "F ";
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
				  buffer = std::to_string(sz);
				  buffer += " messages\n";
					for(int i = 0; i < sz + ps->markedAsDeleted.size(); ++i) {
						if(ps->markedAsDeleted.find(i) == ps->markedAsDeleted.end()) {
							buffer += std::to_string(i);
							buffer += " ";
							if(ps->getCurrentUser()->getReadStatus(i))
							        buffer += "T ";
							else
							        buffer += "F ";
							buffer += std::to_string(ps->getCurrentUser()->getMailSize(i));
							if(i == sz + ps->markedAsDeleted.size() - 1)
							{} else
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
				int tmp = ps->getCurrentUser()->getMailLines(index);
				if(tmp == -1 || ps->markedAsDeleted.find(index) != ps->markedAsDeleted.end()) {
					buffer = "No such mail exists.";
					replycode = REPLY_ERR;
					
				}
				else {
				        if(ps->getCurrentUser()->getReadStatus(index))
					        buffer = "T ";
					else
					        buffer = "F ";
					buffer += std::to_string(tmp);
					ps->Reply(REPLY_OK, buffer);
					transmitContent(ps, index);
					return;
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

		// Handle READ command
	        case POP_READ:
	                if(e->getData().length() != 0) {
				int index = std::stoi(e->getData());
				int tmp = ps->getCurrentUser()->getMailSize(index);
				if(tmp == -1 || ps->markedAsDeleted.find(index) != ps->markedAsDeleted.end()) {
					buffer = "No such mail exists.";
					replycode = REPLY_ERR;
				} else {
					buffer = "";
					replycode = REPLY_OK;
					ps->getCurrentUser()->markAsRead(index);
				}	                        	                        
			} else {
				buffer = "Please provide an argument.";
				replycode = REPLY_ERR;
			}
			break;

                // Handle UNRD command
	        case POP_UNRD:
	                if(e->getData().length() != 0) {
				int index = std::stoi(e->getData());
				int tmp = ps->getCurrentUser()->getMailSize(index);
				if(tmp == -1 || ps->markedAsDeleted.find(index) != ps->markedAsDeleted.end()) {
					buffer = "No such mail exists.";
					replycode = REPLY_ERR;
				} else {
					buffer = "";
					replycode = REPLY_OK;
					ps->getCurrentUser()->markAsUnread(index);
				}	                        	                        
			} else {
				buffer = "Please provide an argument.";
				replycode = REPLY_ERR;
			}
			break;

		// Handle QUIT command
	        /*case POP_QUIT:
			ChangeState(ps, POP_STATE_UPDATE);
			
			int size = ps->currentUser->getInboxSize();
			if(size > 0) {
				buffer = "POP3 server signing off (";
				buffer += std::to_string(size);
				buffer += " message(s) left)\n";
			}
			else {
				buffer = "POP3 server signing off (maildrop empty)";
			}
			replycode = REPLY_OK;
		        break;*/	  
	}
	
	ps->Reply(replycode, buffer);
}



// Public method for changing state
void POPtransaction::ChangeState(POPsession* ps, int n) {
	if(allowedTransitions[n])
		ps->currentState = ps->states[n];
}

// Public method for getting the integer representing the current state
int POPtransaction::getStateNo() {
	return stateNo;
}

/*
 * Update the state of the users inbox with the UPDATE state
 */
void POPupdate::Action(POPsession* ps, POPevent* ) {
	for (auto i = ps->markedAsDeleted.rbegin(); i != ps->markedAsDeleted.rend(); ++i) {
		ps->currentUser->deleteMail(*i);
	}
	
	std::string buffer;
	int size = ps->currentUser->getInboxSize();

	if(size > 0) {
		buffer = "POP3 server signing off (";
		buffer += std::to_string(size);
		buffer += " message(s) left)\n";
	}
	else {
		buffer = "POP3 server signing off (maildrop empty)";
	}
	ps->run = false;
	ps->Reply(REPLY_OK, buffer);

	// implement REPLY_ERR reply if not properly deleted (?)
}

// Public method for changing state
void POPupdate::ChangeState(POPsession* ps, int n) {
	if(allowedTransitions[n])
		ps->currentState = ps->states[n];
}

// Public method for getting the integer representing the current state
int POPupdate::getStateNo() {
	return stateNo;
}

