//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/render/blenderasset.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/light.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendernode.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/game/bowlergame/bowlerinput.hpp"
#include "common/game/bowlergame/states/bowlerstartstate.hpp"
#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static const char* gMusicFileNames[MUSIC_TYPE_MAX] =
{
#define MUSIC_TYPE_ENTRY( ENUM, FILENAME ) FILENAME,
	MUSIC_TYPE_TUPLE
#undef MUSIC_TYPE_ENTRY
};

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerStartState::BowlerStartState( BowlerInput* bowlerInput, PhysicsManager* physicsManager, Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase )
	: BowlerState( bowlerInput, physicsManager, audioManager, bowlerDatabase )
{    
    mTotalElapsedTime = 0;
    mMusicStarted = false;
}

//===========================================================================

BowlerStartState::~BowlerStartState()
{
}

//===========================================================================

void BowlerStartState::Initialize()
{
    Audio::MusicList musicList;
	musicList.AddFile(gMusicFileNames[MUSIC_TYPE_GUI]);
	musicList.SetLoop(true);
    mAudioManager->SetMusicList(musicList);    
}

//===========================================================================

void BowlerStartState::Update( float elapsedTime )
{
    mTotalElapsedTime += elapsedTime;
    if ( mTotalElapsedTime > 0.5f && mMusicStarted == false )
    {
        mAudioManager->PlayMusic();
        mMusicStarted = true;
    }

	Assert( mBowlerInput != NULL, "" );	
    std::vector<int> upResults = mBowlerInput->GetTouchInput( BOWLER_INPUT_TYPE_UP );
    if ( upResults.size() > 0 )
    {		
        mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

		BowlerStateChangeMessage* msg = NEW_PTR( "Message" ) BowlerStateChangeMessage;
		msg->mToState = BOWLER_STATE_TYPE_SELECT;
		GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, msg );

		BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
		bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_SELECT;
        bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CHANGE;
		GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );
	}
}

//===========================================================================

void BowlerStartState::RenderUpdate()
{	
    Renderer::Get()->SetRenderTarget( NULL, NULL );
    Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.0f );
    Renderer::Get()->ClearRenderer( NULL );
	GameApp::Get()->GetRenderQueue()->ClearStates();
}