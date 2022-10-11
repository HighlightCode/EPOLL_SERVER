#include <thread>
#include <chrono>

#include "../include/DBJobContext.h"
#include "../include/DBJobManager.h"
#include "../include/DBConnector.h"

DatabaseJobManager *GDatabaseJobManager = nullptr;

void DatabaseJobManager::ExecuteDatabaseJobs()
{

	DatabaseJobContext *jobContext = nullptr;
	while (true)
	{
		if (mDbJobRequestQueue.PopFront(jobContext))
		{
			jobContext->mSuccess = jobContext->OnExecute();
			mDbJobResultQueue.PushBack(jobContext);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(3));
	}
}

void DatabaseJobManager::PushDatabaseJobRequest(DatabaseJobContext *jobContext)
{
	mDbJobRequestQueue.PushBack(jobContext);
}

bool DatabaseJobManager::PopDatabaseJobResult(DatabaseJobContext *&jobContext)
{
	return mDbJobResultQueue.PopFront(jobContext);
}