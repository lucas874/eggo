#ifndef USER_H
#define USER_H

#include <string> 
#include "PieceOfMail.h"


/*
 * Interface for the User class
 */

class User {
	public:
		User(std::string _name, std::string _pass)
			: name(_name)
			, password(_pass) 
			{
				std::string str = "Users";
				str += "/";
				str += name;
				path = str;
			};
		std::string getName();
		std::string getPass();
		std::string getPath();
		std::string getInboxPath();
		
		void addMail(PieceOfMail* p);
		void addSent(PieceOfMail* p);
		
		int getInboxSize();
		
		int getInboxSizeOctets();
		int getMailSize(int);
		int getMailLines(int);
		std::vector<std::string> getMailContent(int);
		void deleteMail(int);
	        void markAsRead(int);
	        void markAsUnread(int);
	        bool getReadStatus(int);
	private:
		std::string name;
		std::string password;
		std::string path;	
		std::vector<PieceOfMail*> inbox;
		std::vector<PieceOfMail*> sent;
};
#endif
