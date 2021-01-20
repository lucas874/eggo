#include "top.h"

int main() {

	UserCollection *uc = new UserCollection; 
  
  	User *a_user = new User("Alice", "1234");
  	User *b_user = new User("Bob", "Kodeord");

  	uc->addUser(a_user);
  	uc->addUser(b_user);
  
	Top t(uc);

	t.listen();
}
