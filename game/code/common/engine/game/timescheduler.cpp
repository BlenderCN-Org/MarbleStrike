//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/timescheduler.hpp"
#include "common/engine/game/timetask.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////


TimeScheduler::TimeScheduler()
{
}

//===========================================================================

TimeScheduler::~TimeScheduler()
{
}

//===========================================================================

void TimeScheduler::AddTimeTask( const TimeTask &timeTask )
{
	mTimeTaskList.push_back( timeTask );
}

//===========================================================================

void TimeScheduler::Initialize()
{
}

//===========================================================================

void TimeScheduler::Update( float elapsedTime )
{		
	std::list<TimeTask>::iterator timeTaskIter = mTimeTaskList.begin();
	while( timeTaskIter != mTimeTaskList.end() )
	{
		TimeTask* timeTask = &(*timeTaskIter);
		timeTask->Update( elapsedTime );

		float taskElapsedTimeSeconds = timeTask->GetElapsedTimeSeconds();
		float startTimeSeconds = timeTask->GetStartTimeSeconds();
		float endTimeSeconds = timeTask->GetEndTimeSeconds();

		if ( taskElapsedTimeSeconds > ( endTimeSeconds - startTimeSeconds ) )
		{
			timeTask->OnTrigger();
			timeTaskIter = mTimeTaskList.erase( timeTaskIter );
		}
		else
		{
			timeTaskIter++;
		}
	}
}

//===========================================================================

void TimeScheduler::Clear()
{
    mTimeTaskList.clear();
}
