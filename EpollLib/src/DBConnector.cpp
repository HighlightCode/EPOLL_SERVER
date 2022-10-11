#include "../include/DBConnector.h"
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

sqlite3 *DBConnector::mSqlite = nullptr;

DBConnector::DBConnector(const char *sql)
	: mResult(nullptr), mResultColCount(0), mBindColCount(0), mResultCurrentCol(0)
{
	char *errMsg = nullptr;

	if (sqlite3_prepare_v2(mSqlite, sql, -1, &mResult, nullptr) != SQLITE_OK)
	{
		printf("DbHelper Query [%s] Prepare failed: %s\n", sql, sqlite3_errmsg(mSqlite));
	}
}

DBConnector::~DBConnector()
{
	sqlite3_finalize(mResult);
}

bool DBConnector::Initialize(const char *connInfoStr)
{
	int result = sqlite3_open(connInfoStr, &mSqlite);

	if (mSqlite == nullptr || result != SQLITE_OK)
	{
		printf("DbHelper::Initialize Error\n");
		return false;
	}
	
	return true;
}

void DBConnector::Finalize()
{
	if (mSqlite)
		sqlite3_close(mSqlite);
}

bool DBConnector::Execute(const char *format, ...)
{
	if (!format)
		return false;

	if (!mSqlite)
		return false;

	va_list ap;
	char sqlQuery[1024] = {
		0,
	};
	va_start(ap, format);
	int res = vsnprintf(sqlQuery, 1024, format, ap);
	va_end(ap);

	if (-1 == res)
		return false;

	char *errMsg = nullptr;

	if (sqlite3_exec(mSqlite, sqlQuery, NULL, NULL, &errMsg) != SQLITE_OK)
	{
		printf("SQL [%s] ERROR [%s] \n", sqlQuery, errMsg);
		return false;
	}

	return true;
}

bool DBConnector::BindParamInt(int param)
{

	if (sqlite3_bind_int(mResult, ++mBindColCount, param) != SQLITE_OK)
	{
		printf("DbHelper Bind Int failed: %s\n", sqlite3_errmsg(mSqlite));
		return false;
	}

	return true;
}

bool DBConnector::BindParamDouble(double param)
{
	if (sqlite3_bind_double(mResult, ++mBindColCount, param) != SQLITE_OK)
	{
		printf("DbHelper Bind Double failed: %s\n", sqlite3_errmsg(mSqlite));
		return false;
	}

	return true;
}

bool DBConnector::BindParamText(const char *text, int count)
{
	if (sqlite3_bind_text(mResult, ++mBindColCount, text, strlen(text), nullptr) != SQLITE_OK)
	{
		printf("DbHelper Bind Text failed: %s\n", sqlite3_errmsg(mSqlite));
		return false;
	}

	return true;
}

RESULT_TYPE DBConnector::FetchRow()
{
	int result = sqlite3_step(mResult);
	if (result != SQLITE_ROW && result != SQLITE_DONE)
	{
		printf("DbHelper FetchRow failed: %s\n", sqlite3_errmsg(mSqlite));
		return RESULT_ERROR;
	}

	if (result == SQLITE_DONE)
		return RESULT_DONE;

	mResultColCount = sqlite3_column_count(mResult);
	mResultCurrentCol = 0;

	return RESULT_ROW;
}

int DBConnector::GetResultParamInt()
{
	return sqlite3_column_int(mResult, mResultCurrentCol++);
}

double DBConnector::GetResultParamDouble()
{
	return sqlite3_column_double(mResult, mResultCurrentCol++);
}

const unsigned char *DBConnector::GetResultParamText()
{
	return sqlite3_column_text(mResult, mResultCurrentCol++);
}