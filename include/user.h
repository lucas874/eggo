#ifndef USER_H
#define USER_H

#include <string> 
#include "PieceOfMail.h"
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

		int getInboxSize();
		
		int getInboxSizeOctets();
		int getMailSize(int);
		std::string getMailContent(int);
		void deleteMail(int);
	private:
		std::string name;
		std::string password;
		std::string path;	
		std::vector<PieceOfMail*> inbox;
};
#endif
