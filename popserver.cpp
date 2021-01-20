#include "POPsession.h"
#include <iostream>
#include "POPstates.h"
#include <bits/stdc++.h>

using namespace std;

vector<POPstate*> initializeV() {
	vector<POPstate*> v(3);
  	v[0] = new POPauthorization();
  	v[1] = new POPtransaction();
  	v[2] = new POPupdate;
  	return v;
}
  
int main() {
	User *user1 = new User("Lars", "Løkke");
	User *user2 = new User("Person", "adgangskode");
	UserCollection* uc = new UserCollection;
	uc->addUser(user1);
	uc->addUser(user2);	
	
	vector<POPstate*> v = initializeV();

  	POPsession PS(v, uc);  

  	PS.Run();
}

