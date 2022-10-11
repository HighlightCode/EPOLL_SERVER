#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

struct sqlite3;
struct sqlite3_stmt;

enum RESULT_TYPE
{
	RESULT_ERROR,
	RESULT_ROW,
	RESULT_DONE
};

class DBConnector
{
public:
	DBConnector(const char *sql);
	~DBConnector();

	static bool Initialize(const char *connInfoStr);
	static void Finalize();

	static bool Execute(const char *format, ...);

	/*------------- USER FUNCTION -------------*/
	bool BindParamInt(int param);
	bool BindParamDouble(double param);
	bool BindParamText(const char *text, int count);

	RESULT_TYPE FetchRow();

	int GetResultParamInt();
	double GetResultParamDouble();
	const unsigned char *GetResultParamText();

private:
	static sqlite3 *mSqlite;

	sqlite3_stmt *mResult;

	int mResultColCount;

	int mBindColCount;

	int mResultCurrentCol;
};

#endif