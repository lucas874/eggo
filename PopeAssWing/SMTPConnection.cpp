#include "SMTPConnection.h"

SMTPConnection::SMTPConnection(struct Connection* c) {
  _connection = c;
  states = new SMTPState*[7];
  states[0] = SMTPInit::instance();
  states[1] = SMTPHelo::instance();
  states[2] = SMTPMail::instance();
  states[3] = SMTPRcpt::instance();
  states[4] = SMTPData::instance();
  states[5] = SMTPRset::instance();
  states[6] = SMTPQuit::instance();
  
  _state = states[0];
}

void SMTPConnection::Run() {
  while(1) {
    char* buffer = s_recv(this->_connection->socket);
    if(strlen(buffer) > MAX_CMD)
      Reply(500);

    EventData* request = ProcessRequest(buffer);

    std::cout << "Received request %s";

    ChangeState(request);
    
  }
}

void SMTPConnection::Close() {
  std::cout << "hej";
}

void SMTPConnection::Reply(int replycode) {
  char* buffer; 
  //do something
  switch(replycode) {
  case SERVICE_READY:
    buffer = "220 localhost Service Ready";
    break;
  case SERVICE_CLOSING:
    buffer = "221 <domain> Service closing transmission channel";
    break;
  case MAIL_ACTION_OK:
    buffer = "250 Requested mail action okay, completed";
    break;
    
  }
}

EventData* SMTPConnection::ProcessRequest(char* buffer) {
   std::string CMD(4, ' ');
   std::string data = buffer;
   
   for(int i = 0; i < 4; i++)
     CMD[i] = buffer[i];

   transform(CMD.begin(), CMD.end(), CMD.begin(), ::tolower);

   enum Event e;

   /*
    * Not very good with long else if statements,
    * but on the other hand this part of the code
    * is unlikely to change (changes if RFC-821 changes) 
    * and with 8 cases is ok. 
    */
   if(CMD.compare("helo") == 0) {
     //do something
     e = HELO;
     
   }
   else if(CMD.compare("ehlo") == 0) {
     e = HELO;
     
   }
   else if(CMD.compare("mail") == 0) {
     e = MAIL;
     
   }
   else if(CMD.compare("rcpt") == 0) {
     e = RCPT;
     
   }
   else if(CMD.compare("data") == 0) {
     e = DATA;
     
   }
   else if(CMD.compare("rset") == 0) {
     e = RSET;
     
   }
   else if(CMD.compare("noop") == 0) {
     e = NOOP;
     
     // NOOP doesn't cause a state transition
     // this value is never used
     
   }
   else if(CMD.compare("quit") == 0) {
     e = QUIT;
     
   }

   EventData *event = new EventData(e, data);

   return event;
 }

   

void SMTPConnection::ChangeState(SMTPState* s) {
  _state = s;
}

void SMTPConnection::ChangeState(EventData* e) {
  _state = states[e->getEventNo()];
}
  
void SMTPConnection::StateAction() {
  _state->Action();
}
