#include "PieceOfMail.h"

// Called in data state of SMTPsession. 
// Strings received over network appended to email content.
void PieceOfMail::append(std::string data) {
  content.append(data);
}

// Returns content. Used when writing email as file 
// or on screen.
std::string PieceOfMail::getContent() {
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

void PieceOfMail::setHeader() {
	content.append("From: ");
	content.append(sender);
	content.append("\nSubject: \n");
	content.append("To: ");
	for(auto &i : rcpt) {
		content.append(i);
		content.append(" ");
	}
	content.append("\n");
}

