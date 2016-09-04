//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/messages/message.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/gui/bowlerstartgui.hpp"
#include "common/game/bowlergame/gui/bowlerselectgui.hpp"
#include "common/game/bowlergame/gui/bowleroptionsgui.hpp"
#include "common/game/bowlergame/gui/bowlerrungui.hpp"
#include "common/game/bowlergame/gui/bowlerloadgui.hpp"
#include "common/game/bowlergame/gui/bowlerstatsgui.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerGuiManager::BowlerGuiManager( Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase ) 
    :   mAudioManager(audioManager)
    ,   mBowlerDatabase( bowlerDatabase )
{
}

//===========================================================================

BowlerGuiManager::~BowlerGuiManager()
{
}

//===========================================================================

void BowlerGuiManager::Initialize()
{
	GameApp::Get()->GetMessageHandler()->RegisterListener( 
		PORT_GAME, 
		&BowlerGuiManager::OnMessageCallback, 
		this 
		);	
	OpenScreen( BOWLER_GUI_START );	
}

//===========================================================================

void BowlerGuiManager::Shutdown()
{
	for ( int i = 0; i < mNumOpenedScreens; ++i )
	{
		DELETE_PTR( mOpenedScreens[i] );
	}

	GameApp::Get()->GetMessageHandler()->UnRegisterListener( 
		PORT_GAME, 
		&BowlerGuiManager::OnMessageCallback, 
		this 
		);
}

//===========================================================================

void BowlerGuiManager::OnMessageCallback( void* userData )
{
	Message* message = reinterpret_cast<Message*>( userData );
	switch ( message->GetType() )
	{
	case BOWLER_MESSAGE_GUI_EVENT:
		{            
			BowlerGuiEventMessage* msg = reinterpret_cast<BowlerGuiEventMessage*>( message );

            switch ( msg->mEventType )
            {
            case BOWLER_GUI_EVENT_TYPE_CHANGE:
                CloseScreen();
			    OpenScreen( msg->mToGuiType );
                break;
            case BOWLER_GUI_EVENT_TYPE_OPEN:
                OpenScreen( msg->mToGuiType );
                break;
            case BOWLER_GUI_EVENT_TYPE_CLOSE:
                CloseScreen();
                break;
            default:
                break;
            }
		}		
		break;	
	default:
		break;
	}
}

//===========================================================================

void BowlerGuiManager::OpenScreen( BowlerGuiStateType state )
{
	GuiScreen* newGui = NULL;
	switch ( state )
	{
	case BOWLER_GUI_START:
		newGui = NEW_PTR( "Start Gui" ) BowlerStartGui();
		break;	
	case BOWLER_GUI_SELECT:
		newGui = NEW_PTR( "Select Gui" ) BowlerSelectGui(mAudioManager, mBowlerDatabase );
		break;	
	case BOWLER_GUI_OPTIONS:
		newGui = NEW_PTR( "Options Gui" ) BowlerOptionsGui(mAudioManager, mBowlerDatabase);
		break;	
	case BOWLER_GUI_RUN:
		newGui = NEW_PTR( "Run Gui" ) BowlerRunGui(mAudioManager, mBowlerDatabase);
		break;	
    case BOWLER_GUI_LOAD:
		newGui = NEW_PTR( "Load Gui" ) BowlerLoadGui();
		break;	
	case BOWLER_GUI_STATS:
		newGui = NEW_PTR( "Stats Gui" ) BowlerStatsGui(mAudioManager, mBowlerDatabase);
		break;
    default:
        break;
	}

	Assert( newGui != NULL, "" );
	newGui->Initialize();

	mOpenedScreens[mNumOpenedScreens] = newGui;
	mNumOpenedScreens++;
	Assert( mNumOpenedScreens < MAX_OPENED_SCREENS, "" );
}

//===========================================================================

void BowlerGuiManager::CloseScreen()
{
	DELETE_PTR( mOpenedScreens[mNumOpenedScreens - 1] );
	mNumOpenedScreens--;
}

//===========================================================================

int BowlerGuiManager::GetCurrentScreenType()
{
	if ( mNumOpenedScreens > 0 )
	{
		return mOpenedScreens[mNumOpenedScreens - 1]->GetScreenType();
	}

	return -1;
}