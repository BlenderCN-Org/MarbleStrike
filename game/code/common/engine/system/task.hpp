#ifndef TASK_HPP
#define TASK_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// TYPEDEF
//////////////////////////////////////////////////////

typedef void* (*ThreadEntryPoint) ( void*, int );

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	class Task  
	{
	public:
		enum TaskState
		{
			TASK_STATE_CREATED,
			TASK_STATE_RUNNING,
			TASK_STATE_COMPLETED
		};

	public:
		Task();
		~Task();

		void SetEntryPoint( ThreadEntryPoint entryPoint );
		void SetArguments( void* arg );

		void RunTask( int threadID );

		void WaitOn();

		bool IsComplete()
		{
			if ( mTaskState == TASK_STATE_COMPLETED )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		TaskState GetState()
		{
			return mTaskState;
		}

	private:

		volatile TaskState mTaskState;
		ThreadEntryPoint mEntryPoint;
		void* mArguments;

	};
}

#endif 
