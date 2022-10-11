#ifndef DBJOBCONTEXT_H
#define DBJOBCONTEXT_H

#include "types.h"
#include "string.h"
#include <string>

struct DatabaseJobContext
{
	DatabaseJobContext() : mSuccess(false) {}

	DatabaseJobContext(SOCKET socketKey) : mSockKey(socketKey), mSuccess(false) {}

	virtual ~DatabaseJobContext() {}

	virtual bool OnExecute() = 0;

	void SetSuccess(bool success) { mSuccess = success; }

	SOCKET mSockKey;

	bool mSuccess;
};

/*--------------- LOGIN ---------------*/
struct FindPlayerDataContext : public DatabaseJobContext
{
	FindPlayerDataContext(SOCKET socketKey) : DatabaseJobContext(socketKey)
	{
		memset(mPlayerName, 0, sizeof(mPlayerName));
	}
	virtual bool OnExecute();

	char mPlayerName[MAX_PLAYERID];
};

#endif