#include "PieceOfMail.h"

// Called in data state of SMTPsession. 
// Strings received over network appended to email content.
void PieceOfMail::append(std::string data) {
  content.push_back(data);
}

// Returns content. Used when writing email as file 
// or on screen.
std::vector<std::string> PieceOfMail::getContent() {
  return content;
}

// Add recipient to email
void PieceOfMail::setrcpt(std::string _rcpt) {
	rcpt.push_back(_rcpt);
}

// Add sender to email
void PieceOfMail::setsender(std::string _sender) {
	sender = _sender;	
}

// Add subject to email
void PieceOfMail::setSubject(std::string _subject) {
	subject = _subject;	
}
void PieceOfMail::setHeader() {
	content.push_back(sender);
	content.push_back(subject);
	
	
	std::string str = "";
	for(auto &i : rcpt) {
		str += i;
		str += " ";
	}	
	content.push_back(str);
}


int PieceOfMail::getLineNo() {
	return content.size();
}
