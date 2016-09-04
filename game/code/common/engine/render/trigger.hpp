#ifndef TRIGGER_HPP
#define TRIGGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "common/engine/render/renderobjectdata.hpp"
#include "common/engine/messages/message.hpp"
#include "tinyxml2.h"

//////////////////////////////////////////////////////
// FORWARD DECLARTIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum TriggerType
{
	TRIGGER_TYPE_NONE,
    TRIGGER_TYPE_START_ANIMATION,
	TRIGGER_TYPE_TOGGLE,
	TRIGGER_TYPE_REMOVE_OBJECT,
};

enum TriggerState
{
    TRIGGER_STATE_ON,
    TRIGGER_STATE_OFF
};

class TriggerMessage : public Message
{
public:

	TriggerMessage();
	~TriggerMessage();	

    TriggerType mTriggerType;
	void* mData;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Trigger : public RenderObjectData
{
public:
	
	Trigger();
	~Trigger();

	void Update( float elapsedTime );
	void Load( tinyxml2::XMLNode* node );

    void OnTrigger();

    bool IsEnabled()
    {
        return ( mTriggerState == TRIGGER_STATE_ON );
    }

    void EnableTrigger()
    {
        mTriggerState = TRIGGER_STATE_ON;
    }

    void DisableTrigger()
    {
        mTriggerState = TRIGGER_STATE_OFF;
    }

	const char* GetData()
	{
		return mData;
	}
	
private:
    
    TriggerType mTriggerType;
    char mData[32];
    TriggerState mTriggerState;
    
};

#endif
