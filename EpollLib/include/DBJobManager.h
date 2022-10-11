#ifndef DBJOBMANAGER_H
#define DBJOBMANAGER_H

#include "types.h"
#include "SPSCQueue.h"

struct DatabaseJobContext ;

class DatabaseJobManager
{
public:
	DatabaseJobManager() {}

	void ExecuteDatabaseJobs() ;
	
	void PushDatabaseJobRequest(DatabaseJobContext* jobContext) ;
	bool PopDatabaseJobResult(DatabaseJobContext*& jobContext) ;

private:

	SPSCQueue<DatabaseJobContext*, MAX_DBJOB>	mDbJobRequestQueue;
	SPSCQueue<DatabaseJobContext*, MAX_DBJOB>	mDbJobResultQueue;
    
} ;

extern DatabaseJobManager* GDatabaseJobManager;

#endif