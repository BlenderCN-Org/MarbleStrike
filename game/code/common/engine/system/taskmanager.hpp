#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <list>

#include <thread>
#include <mutex>
#include <condition_variable>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//#define USE_TASK_MANAGER

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace System
{
	class Task;
}

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	class TaskManager  
	{
	public:

		struct ThreadData
		{
			TaskManager* mTaskManager;
			int mThreadID;
		};

		enum ThreadRunState
		{
			THREAD_RUN_STATE_RUNNING,
			THREAD_RUN_STATE_WAITING
		};

	public:

		static void RunTask( ThreadData* threadData );

		TaskManager();
		~TaskManager();

		void Initialize();
		void AddTask( Task* task );
		void WaitForAllTasks();

		int GetNumThreads()
		{
			return mNumThreads;
		}

	private:

		int GetNumCPU();

        std::thread* mThreads;
        std::condition_variable mThreadCondition;
        std::mutex mTaskListMutex;
        std::mutex mUpdateThreadMutex;

		int mNumThreads;
		std::list<Task*> mTaskList;
		ThreadData* mThreadData;		
		ThreadRunState* mThreadRunState;
        volatile bool mShouldShutdown;
	};
}

#endif