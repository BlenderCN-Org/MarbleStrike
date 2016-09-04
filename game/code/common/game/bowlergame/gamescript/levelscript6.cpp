//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/gamescript/levelscript6.hpp"
#include "common/game/bowlergame/objects/bowlerobject.hpp"
#include "common/engine/render/trigger.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

LevelScript6::LevelScript6()
{    
	System::StringCopy( mMovingPlatformName1, 32, "Block.026" );

	mAllTriggered = false;
}

//===========================================================================

LevelScript6::~LevelScript6()
{    
}

//===========================================================================

void LevelScript6::Initialize( 
	const std::vector<BowlerObject*> &objects, 
	const std::vector<BowlerObject*> &triggerObjects 
	)
{
	size_t numObjects = triggerObjects.size();
	for ( int i = 0; i < numObjects; ++i )
	{
		BowlerObject* bowlerObject = triggerObjects[i];
		RenderObject* renderObject = bowlerObject->GetRenderObject();
		const char* gameType = renderObject->GetGameType();
		if ( strcmp( gameType, "toggle" ) == 0 )
		{
			mTriggerToggleObjects.push_back( bowlerObject );

			Trigger* trigger = reinterpret_cast<Trigger*>( bowlerObject->GetRenderObject()->GetRenderObjectData() );
			const char* data = trigger->GetData();

			size_t numObjects = objects.size();
			for ( int i = 0; i < numObjects; ++i )
			{
				if ( strcmp( objects[i]->GetRenderObject()->GetObjectName(), data ) == 0 )
				{
					mToggleObjects.push_back( objects[i] );
					break;
				}
			}
		}
	}
}

//===========================================================================

void LevelScript6::Update( 
	float /*elapsedTime*/,
	const std::vector<BowlerObject*> &/*objects*/,
	const std::vector<BowlerObject*> &/*triggerObjects*/
	)
{
	if ( mAllTriggered == false )
	{
		int countToggled = 0;
		size_t numObjects = mToggleObjects.size();
		for ( int i = 0; i < numObjects; ++i )
		{
			BowlerObject* bowlerObject = mToggleObjects[i];
			if ( bowlerObject->GetToggleState() == false )
			{
				break;
			}
			countToggled++;
		}

		if ( numObjects == countToggled )
		{
			TriggerMessage* triggerMessage = NEW_PTR( "Message" ) TriggerMessage;
			triggerMessage->mTriggerType = TRIGGER_TYPE_START_ANIMATION;
			triggerMessage->mData = (void*)mMovingPlatformName1;
			GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, triggerMessage );

			mAllTriggered = true;
		}
	}
	else
	{
		size_t numObjects = mTriggerToggleObjects.size();
		for ( int i = 0; i < numObjects; ++i )
		{
			BowlerObject* bowlerObject = mTriggerToggleObjects[i];
			Trigger* trigger = reinterpret_cast<Trigger*>( bowlerObject->GetRenderObject()->GetRenderObjectData() );
			trigger->DisableTrigger();
		}
	}
}

//===========================================================================

void LevelScript6::Reset()
{
	mAllTriggered = false;

	size_t numObjects = mTriggerToggleObjects.size();
	for ( int i = 0; i < numObjects; ++i )
	{
		BowlerObject* bowlerObject = mTriggerToggleObjects[i];
		Trigger* trigger = reinterpret_cast<Trigger*>( bowlerObject->GetRenderObject()->GetRenderObjectData() );
		trigger->EnableTrigger();
	}
}