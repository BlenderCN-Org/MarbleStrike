//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/game/bowlergame/bowlerinput.hpp"
#include "common/game/bowlergame/messages/bowlerguilevelenablemessage.hpp"
#include "common/game/bowlergame/states/bowlerselectstate.hpp"

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

BowlerSelectState::BowlerSelectState( BowlerInput* bowlerInput, PhysicsManager* physicsManager, Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase )
	: BowlerState( bowlerInput, physicsManager, audioManager, bowlerDatabase )
{
}

//===========================================================================

BowlerSelectState::~BowlerSelectState()
{
    mAudioManager->StopMusic();
    mAudioManager->ClearMusicList();
}

//===========================================================================

void BowlerSelectState::Initialize()
{
    if ( !mAudioManager->IsMusicPlaying() )
    {
        Audio::MusicList musicList;
	    musicList.AddFile(gMusicFileNames[MUSIC_TYPE_GUI]);
	    musicList.SetLoop(true);
        mAudioManager->SetMusicList(musicList);
        mAudioManager->PlayMusic();
    }
}

//===========================================================================

void BowlerSelectState::Update( float /*elapsedTime*/ )
{	
}

//===========================================================================

void BowlerSelectState::RenderUpdate()
{	
    Renderer::Get()->SetRenderTarget( NULL, NULL );
    Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.0f );
    Renderer::Get()->ClearRenderer( NULL );
	GameApp::Get()->GetRenderQueue()->ClearStates();
}
