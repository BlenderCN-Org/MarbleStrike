#ifndef TIME_SCHEDULER_HPP
#define TIME_SCHEDULER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <list>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class TimeTask;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class TimeScheduler  
{
public:
	TimeScheduler();
	~TimeScheduler();

	void AddTimeTask( const TimeTask &timeTask );
	void Initialize();
	void Update( float elapsedTime );
    void Clear();

private:

	std::list<TimeTask> mTimeTaskList;
};

#endif 
