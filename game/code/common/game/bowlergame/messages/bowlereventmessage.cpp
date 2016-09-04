//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/variable.hpp"
#include "common/engine/system/variableint.hpp"
#include "common/engine/system/variablefloat.hpp"
#include "common/engine/system/variablebool.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerEventMessage::BowlerEventMessage() 	
	: Message( BOWLER_MESSAGE_EVENT )
{
	mBowlerEventType = BOWLER_EVENT_TYPE_NONE;

	for ( int i = 0; i < MAX_PARAMETER; ++i )
	{
		mParameters[i] = NULL;
	}
}

//===========================================================================

BowlerEventMessage::~BowlerEventMessage()
{
	for ( int i = 0; i < MAX_PARAMETER; ++i )
	{
		DELETE_PTR( mParameters[i] );
	}
}

//===========================================================================

void BowlerEventMessage::AddParameterInt( int index, int value )
{
	Assert( index >= 0 &&  index < MAX_PARAMETER, "Index out of range" );
	DELETE_PTR( mParameters[index] );
	Variable* newParameter = NEW_PTR( "Parameter" ) VariableInt( value );	
	mParameters[index] = newParameter;
}

//===========================================================================

void BowlerEventMessage::AddParameterFloat( int index, float value )
{
	Assert( index >= 0 &&  index < MAX_PARAMETER, "Index out of range" );
	DELETE_PTR( mParameters[index] );
	Variable* newParameter = NEW_PTR( "Parameter" ) VariableFloat( value );	
	mParameters[index] = newParameter;
}

//===========================================================================

void BowlerEventMessage::AddParameterBool( int index, bool value )
{
	Assert( index >= 0 &&  index < MAX_PARAMETER, "Index out of range" );
	DELETE_PTR( mParameters[index] );
	Variable* newParameter = NEW_PTR( "Parameter" ) VariableBool( value );	
	mParameters[index] = newParameter;
}

//===========================================================================

Variable* BowlerEventMessage::GetParameter( int index )
{
	Assert( index >= 0 &&  index < MAX_PARAMETER, "Index out of range" );
	return mParameters[index];
}