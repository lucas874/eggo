#include "UserCollection.h"

/*UserCollection::UserCollection() 
        : context(1)
	, socket(context, zmq::socket_type::rep)
{
	socket.bind("tcp://*:50002");
}
*/

void UserCollection::addUser(User* u) {
	umap.insert({u->getName(), u});
}

void UserCollection::removeUser(User* u) {
	umap.erase(u->getName());

}

User* UserCollection::LookUp(std::string username) {
	// Computed twice???	
	if(umap.find(username) != umap.end())
		return umap.find(username)->second;
	else 
		return nullptr;
}


void UserCollection::addMailToInbox(User* u, PieceOfMail* p) {
	
	u->addMail(p);
	std::string filename = u->getInboxPath();
	filename += std::to_string(u->getInboxSize()); 
	filename += ".txt";	
	std::ofstream letter;
	letter.open(filename);

	letter << p->getContent();

	
} 
