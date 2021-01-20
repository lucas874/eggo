#ifndef PIECEOFMAIL_H_
#define PIECEOFMAIL_H_

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

	 // Content of piece of mail is stored here
	 std::string content;
 public:
	 PieceOfMail(){};

	 // Add content to email
	 void append(std::string);

	 // Access email content
	 std::string getContent();

	 // Set recepient of email
	 void setrcpt(std::string);

	 // Set sender of email
	 void setsender(std::string);
  
};


#endif
