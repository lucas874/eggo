#include "UserCollection.h"
#include "connection.h"
#include "Globals.h"
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
	bool run = false;
};

