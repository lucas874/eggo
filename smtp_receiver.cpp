#include "smtp_receiver.h"

int SMTP_Receiver::processHELO(char* buffer) {
  return(reply("OK"));
  
}


int SMTP_Receiver::processMAIL(char* buffer) {
  return 2;
}


int SMTP_Receiver::processRCPT(char* buffer) {
  return 3;
}


int SMTP_Receiver::processDATA(char* buffer) {
  return 4;
}


int SMTP_Receiver::processRSET(char* buffer) {
  return 5;
}


int SMTP_Receiver::processNOOP(char* buffer) {
  return 6;
}


int SMTP_Receiver::processQUIT(char* buffer) {
  return 7;
}


int SMTP_Receiver::processRequest(char* buffer) {

  //std::string request = buffer.substr(0,4);
  /*char subbuffer[4];
  for(int i = 0; i < 4; i++) {
    subbuffer[i] = (char) tolower(buffer[i]);
    }*/
    //std::transform(request.begin(), request.end(), request.begin(), [](unsigned char c) {return std::tolower(c); });


  /*
   * Generally bad to test like this, but it seemed more complex to have 
   * hash function or switch with mapping between strings and integer values
   * since there are only 8 options
   */
  if(buffer == "helo" || buffer == "ehlo")
    return processHELO(buffer);
  else if(buffer == "mail")
    return processMAIL(buffer);
  else if(buffer == "rcpt")
    return processRCPT(buffer);
  else if(buffer == "data")
    return processDATA(buffer);
  else if(buffer == "rset")
    return processRSET(buffer);
  else if(buffer == "noop")
    return processNOOP(buffer);
  else if(buffer == "quit")
    return processQUIT(buffer);
}

void SMTP_Receiver::run() {
  std::cout << "220 mailserver.eggoenterprises.com ready" << std::endl;
  while(1) {
    std::cout << "hej";
    char* request = s_recv(responder);
    std::cout << request;

    processRequest(request);
  }
}

int SMTP_Receiver::reply(char* buffer) {
  s_send (responder, buffer);
  return 250;
}



