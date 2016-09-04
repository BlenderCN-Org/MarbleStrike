//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/timetask.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

TimeTask::TimeTask( float startTime, float endTime, void (*OnTrigger)( void* ), void* context )
{
	mStartTimeSeconds = startTime;
	mEndTimeSeconds = endTime;
	mElapsedTimeSeconds = 0;
	mContext = context;
	OnTriggerCallback = OnTrigger;
}

//===========================================================================

TimeTask::~TimeTask()
{
}

//===========================================================================

void TimeTask::Update( float elapsedTime )
{
	mElapsedTimeSeconds += elapsedTime;
}

//===========================================================================

float TimeTask::GetElapsedTimeSeconds()
{
	return mElapsedTimeSeconds;
}

//===========================================================================

float TimeTask::GetStartTimeSeconds()
{
	return mStartTimeSeconds;
}

//===========================================================================

float TimeTask::GetEndTimeSeconds()
{
	return mEndTimeSeconds;
}

//===========================================================================

void TimeTask::OnTrigger()
{
	OnTriggerCallback( mContext );
}