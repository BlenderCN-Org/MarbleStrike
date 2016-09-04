//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/trigger.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/quaternion.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

TriggerMessage::TriggerMessage() 	
	: Message( BOWLER_MESSAGE_TRIGGER )
{
    mTriggerType = TRIGGER_TYPE_START_ANIMATION;
	mData = NULL;    
}

//===========================================================================

TriggerMessage::~TriggerMessage()
{
}

Trigger::Trigger() 
	:	RenderObjectData( RENDER_OBJECT_DATA_TYPE_TRIGGER )
{    
	mTriggerType = TRIGGER_TYPE_NONE;
    mTriggerState = TRIGGER_STATE_ON;
}

//===========================================================================

Trigger::~Trigger()
{
}

//===========================================================================

void Trigger::Update( float /*elapsedTime*/ )
{	
}

//===========================================================================

void Trigger::Load( tinyxml2::XMLNode* node )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "trigger" ) == 0 )
			{			
				const char* triggerType = element->Attribute( "on_trigger" );
				Assert( triggerType != NULL, "" );

				if ( strcmp( triggerType, "START_ANIMATION" ) == 0 )
                {
                    mTriggerType = TRIGGER_TYPE_START_ANIMATION;
                }
				else if ( strcmp( triggerType, "TOGGLE" ) == 0 )
				{
					mTriggerType = TRIGGER_TYPE_TOGGLE;
				}

                const char* data = element->Attribute( "data" );
                System::StringCopy( mData, 32, data );
			}
		}
	}
}

//===========================================================================

void Trigger::OnTrigger()
{
    TriggerMessage* triggerMessage = NEW_PTR( "Message" ) TriggerMessage;
    triggerMessage->mTriggerType = mTriggerType;
    triggerMessage->mData = (void*)mData;
    GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, triggerMessage );
}