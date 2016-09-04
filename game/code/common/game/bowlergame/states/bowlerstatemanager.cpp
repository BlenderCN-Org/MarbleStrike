//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/utilities.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/game/bowlergame/states/bowlerstate.hpp"
#include "common/game/bowlergame/states/bowlerstartstate.hpp"
#include "common/game/bowlergame/states/bowlerselectstate.hpp"
#include "common/game/bowlergame/states/bowlerrunstate.hpp"
#include "common/game/bowlergame/states/bowlerstatemanager.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"
#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerStateManager::BowlerStateManager( 
	BowlerInput* bowlerInput, 
	PhysicsManager* physicsManager,
    Audio::AudioManager* audioManager,
	BowlerDatabase* bowlerDatabase
	)
{
	GameApp::Get()->GetMessageHandler()->RegisterListener( PORT_GAME, &BowlerStateManager::OnMessageCallback, this );
	
	mBowlerInput = bowlerInput;
	mPhysicsManager = physicsManager;
    mAudioManager = audioManager;
	mBowlerDatabase = bowlerDatabase;

	mCurrentStateType = BOWLER_STATE_TYPE_START;
	mCurrentState = NULL;
	ChangeState( mCurrentStateType );
	mCurrentState->Initialize();

	mCurrentLevel = 0;
}

//===========================================================================

BowlerStateManager::~BowlerStateManager()
{
    if ( mCurrentState )
    {
        mCurrentState->Shutdown();
    }
	DELETE_PTR( mCurrentState );
	GameApp::Get()->GetMessageHandler()->UnRegisterListener( PORT_GAME, &BowlerStateManager::OnMessageCallback, this );	
}

//===========================================================================

void BowlerStateManager::OnMessageCallback( void* userData )
{
	Message* message = reinterpret_cast<Message*>( userData );
	switch ( message->GetType() )
	{
	case BOWLER_MESSAGE_STATE_CHANGE:
		{
			BowlerStateChangeMessage* msg = reinterpret_cast<BowlerStateChangeMessage*>( message );
			ChangeState( msg->mToState );

			if ( mCurrentStateType == BOWLER_STATE_TYPE_RUN )
			{
				BowlerRunState* runState = reinterpret_cast<BowlerRunState*>( mCurrentState );
				runState->SetLevel( msg->mLevel );

				mCurrentLevel = msg->mLevel;
			}

			mCurrentState->Initialize();
		}		
		break;
	case BOWLER_MESSAGE_EVENT:
		{
			BowlerEventMessage* msg = reinterpret_cast<BowlerEventMessage*>( message );
			if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_NEXT_LEVEL )
			{
				mCurrentLevel++;
				ChangeState( BOWLER_STATE_TYPE_RUN );
				BowlerRunState* runState = reinterpret_cast<BowlerRunState*>( mCurrentState );
				runState->SetLevel( mCurrentLevel );
				mCurrentState->Initialize();

	            BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
		        bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_LOAD;
			    bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CHANGE;
				GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );
			}
		}
		break;
	default:
		break;
	}
}

//===========================================================================

void BowlerStateManager::Update( float elapsedTime )
{
	Assert( mCurrentState != NULL, "" );
	mCurrentState->Update( elapsedTime );
}

//===========================================================================

void BowlerStateManager::RenderUpdate()
{
	Assert( mCurrentState != NULL, "" );
	mCurrentState->RenderUpdate();
}

//===========================================================================

void BowlerStateManager::ChangeState( BowlerStateType state )
{
    if ( mCurrentState )
    {
        mCurrentState->Shutdown();
    }
	DELETE_PTR( mCurrentState );

	mCurrentStateType = state;

	switch ( state )
	{
	case BOWLER_STATE_TYPE_START:		
		mCurrentState = NEW_PTR( "StartState" ) BowlerStartState( 
			mBowlerInput, 
			mPhysicsManager,
            mAudioManager,
			mBowlerDatabase
			);
		break;
	case BOWLER_STATE_TYPE_SELECT:
		mCurrentState = NEW_PTR( "SelectState" ) BowlerSelectState( 
			mBowlerInput, 
			mPhysicsManager,
            mAudioManager,
			mBowlerDatabase
			);
		break;
	case BOWLER_STATE_TYPE_RUN:
		mCurrentState = NEW_PTR( "RubState" ) BowlerRunState( 
			mBowlerInput, 
			mPhysicsManager,
            mAudioManager,
			mBowlerDatabase
			);
		break;
	}
}