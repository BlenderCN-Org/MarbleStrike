#ifndef TIME_TASK_HPP
#define TIME_TASK_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS
//////////////////////////////////////////////////////

class TimeTask
{
public:
	TimeTask( float startTime, float endTime, void (*OnTrigger)( void* ), void* context );
	~TimeTask();

	void Update( float elapsedTime );
	float GetElapsedTimeSeconds();
	float GetStartTimeSeconds();
	float GetEndTimeSeconds();
	void OnTrigger();

private:

	float mStartTimeSeconds;
	float mEndTimeSeconds;
	float mElapsedTimeSeconds;
	void (*OnTriggerCallback)( void* );
	void* mContext;
};

#endif 
