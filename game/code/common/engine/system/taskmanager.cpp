//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/taskmanager.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/task.hpp"

#ifdef PLAT_PC
#include <windows.h>
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	void TaskManager::RunTask( ThreadData* threadData )
	{
        TaskManager* taskManager = threadData->mTaskManager;
		int threadID = threadData->mThreadID;

        bool infiniteLoop = true;
		while ( infiniteLoop && taskManager->mShouldShutdown )
		{
			if ( taskManager->mTaskList.size() == 0 )
			{
                std::unique_lock<std::mutex> lock( taskManager->mUpdateThreadMutex );
				taskManager->mThreadCondition.wait( lock );
			}
			else
			{		
                taskManager->mTaskListMutex.lock();
				Task* task = NULL;
				if ( taskManager->mTaskList.size() != 0 )
				{
					task = taskManager->mTaskList.front();
					taskManager->mTaskList.pop_front();
				}
				taskManager->mTaskListMutex.unlock();
				
				if ( task )
				{
					task->RunTask( threadID );
				}
			}
		}
	}

	//===========================================================================

    TaskManager::TaskManager()
    {
		mNumThreads = 0;
        mShouldShutdown = false;
		mThreads = NULL;    
	}

    //===========================================================================

    TaskManager::~TaskManager()
    {
        mShouldShutdown = true;
        mThreadCondition.notify_all();
        for ( int i = 0; i < mNumThreads; i++)
		{
#if defined USE_TASK_MANAGER
            mThreads[i].join();
#endif
        }

		DELETE_PTR_ARRAY( mThreads );
		DELETE_PTR_ARRAY( mThreadData );
		DELETE_PTR_ARRAY( mThreadRunState );
    }

	//===========================================================================

	void TaskManager::Initialize()
	{
		int numCPU = GetNumCPU();
		mNumThreads = numCPU - 1;

		mThreads = NEW_PTR( "threads" ) std::thread[mNumThreads];
		mThreadData = NEW_PTR( "Thread Data" ) ThreadData[mNumThreads];		
		mThreadRunState = NEW_PTR( "ThreadRunState" ) ThreadRunState[mNumThreads];

		for ( int i = 0; i < mNumThreads; i++)
		{
			mThreadData[i].mTaskManager = this;
			mThreadData[i].mThreadID = i;

#if defined USE_TASK_MANAGER
			mThreads[i] = std::thread( RunTask, &mThreadData[i] );
#endif

			mThreadRunState[i] = THREAD_RUN_STATE_WAITING;
		}
	}

	//===========================================================================

	void TaskManager::AddTask( Task* task )
	{
		mTaskListMutex.lock();
		mTaskList.push_back( task );
		mTaskListMutex.unlock();

		mUpdateThreadMutex.lock();
        mThreadCondition.notify_all();
		mUpdateThreadMutex.unlock();
	}

	//===========================================================================

	void TaskManager::WaitForAllTasks()
	{
	}

	//===========================================================================

	int TaskManager::GetNumCPU()
	{
#ifdef PLAT_PC
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );

		int numCPU = sysinfo.dwNumberOfProcessors;
		return numCPU;
#else
		return 1;
#endif
	}
}