//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerGuiEventMessage::BowlerGuiEventMessage() 	
	: Message( BOWLER_MESSAGE_GUI_EVENT )
{
	mToGuiType = BOWLER_GUI_START;
    mEventType = BOWLER_GUI_EVENT_TYPE_CHANGE;
}

//===========================================================================

BowlerGuiEventMessage::~BowlerGuiEventMessage()
{

}
