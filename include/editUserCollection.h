#ifndef _EDITUSERCOLLECTION_H_
#define _EDITUSERCOLLECTION_H_

#include "UserCollection.h"
#include "connection.h"
#include "globals.h"
#include <iostream>

class EditUsers {
	public:
	EditUsers(UserCollection* _uc, struct connection* _c)
		: uc(_uc)
		, c(_c) {};

	void Run();
	void Reply(int);

	private:
	UserCollection *uc;
	struct connection* c;
	bool run = true;
};

#endif
