#ifndef _PIECEOFMAIL_H_
#define _PIECEOFMAIL_H_

#include <vector>
#include <string>

// Code representation of a pice of email
class PieceOfMail {
 private:
	 // Consider whether this is necessary  
	 std::string filename;

	 // Possibly multiple recipents for a single piece of mail
	 std::vector<std::string> rcpt;

	 // Sender of email
	 std::string sender;

	 // Subject of email
	 std::string subject;

	 // Content of piece of mail is stored here
	 std::vector<std::string> content;
 public:
	 PieceOfMail(){};

	 // Set To, From, Subject
	 void setHeader();

	 // Add content to email
	 void append(std::string);

	 // Access email content
	 std::vector<std::string> getContent();

	 // Set recepient of email
	 void setrcpt(std::string);

	 // Set sender of email
	 void setsender(std::string);

	 // Set subject
	 void setSubject(std::string);

	 int getLineNo();
  
};


#endif
