//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/physics/physicsmanager.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/audio/audiomanager.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/bowlergame.hpp"
#include "common/game/bowlergame/states/bowlerstatemanager.hpp"
#include "common/game/bowlergame/bowlerinput.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/messages/messagehandler.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static const char* gSoundFileNames[SOUND_TYPE_MAX] =
{
#define SOUND_TYPE_ENTRY( ENUM, FILENAME ) FILENAME,
	SOUND_TYPE_TUPLE
#undef SOUND_TYPE_ENTRY
};

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerGame::BowlerGame()
{
}

//===========================================================================

BowlerGame::~BowlerGame()
{
	mBowlerGuiManager->Shutdown();
	DELETE_PTR( mBowlerGuiManager );

	DELETE_PTR( mBowlerStateManager );	

	DELETE_PTR( mBowlerInput );	

	mPhysicsManager->Shutdown();
	DELETE_PTR( mPhysicsManager );

	mAudioManager->Shutdown();
	Audio::AudioManager::Destroy();

	mBowlerDatabase->SaveUserData();
	DELETE_PTR( mBowlerDatabase );
}

//===========================================================================

void BowlerGame::Initialize()
{
	mBowlerDatabase = NEW_PTR( "BowlerDatabase" ) BowlerDatabase;
	mBowlerDatabase->Load();

	mAudioManager = Audio::AudioManager::Create();
	mAudioManager->Initialize();

	mPhysicsManager = NEW_PTR( "PhysicsManager" ) PhysicsManager;
	mPhysicsManager->Initialize();

	mBowlerInput = NEW_PTR( "BowlerInput" ) BowlerInput;

	mBowlerStateManager = NEW_PTR( "BowlerStateManager" ) BowlerStateManager( mBowlerInput, mPhysicsManager, mAudioManager, mBowlerDatabase );

	mBowlerGuiManager = NEW_PTR( "BowlerGuiManager" ) BowlerGuiManager( mAudioManager, mBowlerDatabase );
	mBowlerGuiManager->Initialize();

	Audio::SoundBank soundBank;
	for ( int i = 0; i < SOUND_TYPE_MAX; ++i )
	{
		soundBank.AddFile( gSoundFileNames[i] );
	}
	mAudioManager->SetSoundBank(soundBank);
}

//===========================================================================

void BowlerGame::Update( float elapsedTime )
{
	mBowlerStateManager->Update( elapsedTime );
	mBowlerGuiManager->Update( elapsedTime );
	mAudioManager->Update();
}

//===========================================================================

void BowlerGame::UpdateVariable( float elapsedTime )
{
	if ( mPhysicsManager )
	{
		mPhysicsManager->Update( elapsedTime );
	}
}

//===========================================================================

void BowlerGame::RenderUpdate()
{	
	mBowlerStateManager->RenderUpdate();
	mBowlerGuiManager->RenderUpdate();
}

//===========================================================================

void BowlerGame::Show()
{
	mAudioManager->UnpauseAllAudio();
}

//===========================================================================

void BowlerGame::Hide()
{
	mAudioManager->PauseAllAudio();    
}

//===========================================================================

void BowlerGame::SetVolume()
{
	float volume = Database::Get()->GetVolume();
	mAudioManager->SetMusicVolume( volume );
	mAudioManager->SetSoundVolume( volume );
}

//===========================================================================

void BowlerGame::OnBackButton()
{
	BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
    bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_BACK_BUTTON_PRESS;		
	GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
}

//===========================================================================

bool BowlerGame::IsExitable()
{
	if ( mBowlerStateManager )
	{
		int screenType = mBowlerGuiManager->GetCurrentScreenType();
		if ( screenType  == BOWLER_GUI_START )
		{
			return true;
		}
	}

	return false;
}