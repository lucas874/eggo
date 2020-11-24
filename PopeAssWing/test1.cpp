#include "EventData.h"
#include <iostream>

using namespace std;

int main() {
  enum Event e;
  e = NOOP;
  string str = "hej";
  EventData obj(e, str);

  cout << obj.getData() << endl;
  cout << obj.getEventNo() << endl;
}
