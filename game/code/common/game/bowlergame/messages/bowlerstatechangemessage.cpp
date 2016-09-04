//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerStateChangeMessage::BowlerStateChangeMessage() 	
	: Message( BOWLER_MESSAGE_STATE_CHANGE )
{
	mToState = BOWLER_STATE_TYPE_START;
	mLevel = 0;
}

//===========================================================================

BowlerStateChangeMessage::~BowlerStateChangeMessage()
{

}
