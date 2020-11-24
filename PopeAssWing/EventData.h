#ifndef _EVENTDATA_H_
#define _EVENTDATA_H_

//#include <string>
//#include <cstring>
#include <iostream>

enum Event {INIT, HELO, MAIL, RCPT, DATA, RSET, QUIT, NOOP};

class EventData {
 public:
  EventData(enum Event e, std::string str);
  
  std::string getData();
  int getEventNo();
  
  
 private:
  enum Event _event;
  std::string _data;

  // State that SMTP session transitions to upon event
};

#endif

