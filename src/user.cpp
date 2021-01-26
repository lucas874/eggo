#include "user.h"

std::string User::getName() {
	return name;
}

std::string User::getPass() {
	return password;
}

std::string User::getPath() {
	return path;
}

std::string User::getInboxPath() {
	std::string str = path;
	str += "/";
	str += "Inbox";
	str += "/";
	return str;
}

void User::addMail(PieceOfMail* p) {
	inbox.push_back(p);
}

int User::getInboxSize() {
	return inbox.size();
}

int User::getInboxSizeOctets() {
	int sz = 0;
	for(auto &itr : inbox) {
		sz += sizeof(itr->getContent());
	}
	return sz;
}

int User::getMailSize(int index) {
	if(inbox.size() == 0)
		return -1;
	if(index < 0 || index >= inbox.size())
		return -1;
	return sizeof(inbox[index]->getContent());
}

std::vector<std::string> User::getMailContent(int index) {
	return inbox[index]->getContent();
}

void User::deleteMail(int index) {
	inbox.erase(inbox.begin()+index);
}

int User::getMailLines(int index) {
	if(inbox.empty())
		return -1;
	return inbox[index]->getLineNo();
}

bool User::getReadStatus(int index) {
        return inbox[index]->getReadStatus();
}

void User::markAsRead(int index) {
	inbox[index]->markAsRead();
}

void User::markAsUnread(int index) {
	inbox[index]->markAsUnread();
}
