//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/task.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
    Task::Task()
    {
		mTaskState = TASK_STATE_CREATED;
    }

    //===========================================================================

    Task::~Task()
    {
    }

	//===========================================================================

	void Task::SetEntryPoint( ThreadEntryPoint entryPoint )
	{
		mTaskState = TASK_STATE_CREATED;
		mEntryPoint = entryPoint;
	}

	//===========================================================================

	void Task::SetArguments( void* arg )
	{
		mArguments = arg;
	}

	//===========================================================================

	void Task::RunTask( int threadID )
	{
		mTaskState = TASK_STATE_RUNNING;
		mEntryPoint( mArguments, threadID );
		mTaskState = TASK_STATE_COMPLETED;
	}

	//===========================================================================

	void Task::WaitOn()
	{
 		while ( mTaskState != TASK_STATE_COMPLETED );
	}
}
