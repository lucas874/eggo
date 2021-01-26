#include "UserCollection.h"

void UserCollection::addUser(User* u) {
	umap.insert({u->getName(), u});
}

void UserCollection::removeUser(User* u) {
	umap.erase(u->getName());

}

int UserCollection::addUser(std::string name, std::string password) {
	if(LookUp(name) != nullptr)
		return -1;
	else {
		User *newuser = new User(name, password);

		umap.insert({name, newuser});
	}
	return 0;
}

int UserCollection::removeUser(std::string name, std::string password) {
	User *toBeRemoved = LookUp(name);
	if(toBeRemoved == nullptr)
		return -1;
	else {
		if(password.compare(toBeRemoved->getPass()) != 0)
			return -2;
		else
			umap.erase(name);
	}
	return 0;
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
	//std::string filename = u->getInboxPath();
	//filename += std::to_string(u->getInboxSize()); 
	//filename += ".txt";	
	//std::ofstream letter;
	//letter.open(filename);

	//letter << p->getContent();

	
} 
