#ifndef BOWLER_EVENT_MESSAGE_HPP
#define BOWLER_EVENT_MESSAGE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/messages/message.hpp"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Variable;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_PARAMETER = 5;

enum BowlerEventType
{
	BOWLER_EVENT_TYPE_NONE,
	BOWLER_EVENT_TYPE_START_GO,
	BOWLER_EVENT_TYPE_LEVEL_START,
	BOWLER_EVENT_TYPE_LEVEL_FINISH,
    BOWLER_EVENT_TYPE_LEVEL_TIME_ATTACK_FAIL,
	BOWLER_EVENT_TYPE_NEXT_LEVEL,
	BOWLER_EVENT_TYPE_REPLAY,
	BOWLER_EVENT_TYPE_PAUSE,
	BOWLER_EVENT_TYPE_UNPAUSE,
	BOWLER_EVENT_TYPE_COLLECT,
    BOWLER_EVENT_TYPE_REMOVE_COLLECT,
	BOWLER_EVENT_TYPE_ADD_OBJECT_FROM_PHYSICS,
	BOWLER_EVENT_TYPE_REMOVE_OBJECT_FROM_PHYSICS,
    BOWLER_EVENT_TYPE_BACK_BUTTON_PRESS
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerEventMessage : public Message
{
public:

	BowlerEventMessage();
	~BowlerEventMessage();	

	void AddParameterInt( int index, int value );
	void AddParameterFloat( int index, float value );
	void AddParameterBool( int index, bool value );
	Variable* GetParameter( int index );

	BowlerEventType mBowlerEventType;

	void* mContext;

private:
		
	Variable* mParameters[MAX_PARAMETER];
};

#endif
