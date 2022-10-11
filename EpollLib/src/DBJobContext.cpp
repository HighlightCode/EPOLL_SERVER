#include "../include/DBJobContext.h"
#include "../include/DBConnector.h"

const char* FindPlayer = "SELECT NAME FROM UserInfo WHERE NAME=?";

bool FindPlayerDataContext::OnExecute()
{
    DBConnector dbConnector(FindPlayer);
    dbConnector.BindParamText(mPlayerName, strlen(mPlayerName));

    if ( RESULT_ERROR == dbConnector.FetchRow())
		return false;

	return true;
}