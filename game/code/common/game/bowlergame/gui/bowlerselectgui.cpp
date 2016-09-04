//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/gui/bowlerselectgui.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguilevelenablemessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"
#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"

//#define DISABLE_LEVEL_LOCKS

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//===========================================================================

BowlerSelectGui::BowlerSelectGui(Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase )    
	:   GuiScreen( BOWLER_GUI_SELECT )
{	
	mAudioManager = audioManager;
	mBowlerDatabase = bowlerDatabase;

	GameApp::Get()->GetMessageHandler()->RegisterListener( PORT_GAME, &BowlerSelectGui::OnMessageCallback, this );	
	mCurrentWorld = 0;

	for ( int i = 0; i < MAX_LEVELS; ++i )
	{
		mIsUnlocked[i] = false;
	}
}

//===========================================================================

BowlerSelectGui::~BowlerSelectGui()
{	
	GameApp::Get()->GetMessageHandler()->UnRegisterListener( PORT_GAME, &BowlerSelectGui::OnMessageCallback, this );		
}

//===========================================================================

void BowlerSelectGui::OnMessageCallback( void* userData )
{
	Message* message = reinterpret_cast<Message*>( userData );
	switch ( message->GetType() )
	{
	case BOWLER_MESSAGE_EVENT:
		{
			BowlerEventMessage* msg = reinterpret_cast<BowlerEventMessage*>( message );

			if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_BACK_BUTTON_PRESS )
			{
				switch ( mGuiSelectState )
				{
				case GUI_SELECT_STATE_LEVEL_SELECT:
					{
						BowlerStateChangeMessage* msg = NEW_PTR( "Message" ) BowlerStateChangeMessage;
						msg->mToState = BOWLER_STATE_TYPE_START;
						GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, msg );

						BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
						bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_START;
						bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CHANGE;
						GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );
					}
					break;
				default:
					break;
				}
			}
		}
		break;
	case BOWLER_MESSAGE_GUI_EVENT:
		{            
			BowlerGuiEventMessage* msg = reinterpret_cast<BowlerGuiEventMessage*>( message );

            switch ( msg->mEventType )
            {
            case BOWLER_GUI_EVENT_TYPE_CLOSE:
				
				mGuiSelectState = GUI_SELECT_STATE_LEVEL_SELECT;
                break;
            default:
                break;
            }
		}		
		break;	
	case BOWLER_MESSAGE_GUI_LEVEL_ENABLE:
		{
			CheckLevelLocks();

			int lastEnabledLevel = 0;
			for ( int i = 0; i < MAX_LEVELS; ++i )
			{
				if ( mIsUnlocked[i] )
				{
					lastEnabledLevel++;
				}
				else
				{
					break;
				}
			}

			mLastEnabledLevel = lastEnabledLevel;

			for ( int i = 0; i < MAX_LEVELS; ++i )
			{
				GuiButton* levelButton = mLevelButtons[i];
				if ( i < lastEnabledLevel )
				{   
					levelButton->SetEnabled( true );
				}
				else
				{
					levelButton->SetEnabled( false );
				}
			}

			char buffer[8];        
			snprintf( buffer, 8, "x%i", mTotalCollected );
			mCollectedText->SetText( buffer );

			UpdateCurrentWorldCountItem( mCurrentWorld, lastEnabledLevel );
		}
		break;	
	default:
		break;
	}
}

//===========================================================================

void BowlerSelectGui::Initialize()
{	
	mGuiSelectState = GUI_SELECT_STATE_LEVEL_SELECT;

	const char* resolutionString = GameApp::GetResolutionString();
	std::string filename = "game\\bowler\\gui\\bowlerselectgui";  
	filename += resolutionString;
	filename += ".xml";
	Load( filename.c_str() );

	GuiButton* optionsButton = GetButtonByName( "OptionsButton" );
	optionsButton->SetID( GUI_BUTTON_OPTIONS );

	GuiButton* statsButton = GetButtonByName( "StatsButton" );
	statsButton->SetID( GUI_BUTTON_STATS );

	GuiButton* levelButton = NULL;
	char buffer[64];
	for ( int i = 0; i < MAX_LEVELS_PER_WORLD; ++i )
	{
		snprintf( buffer, 64, "Button%02i", i+1 );
		levelButton = GetButtonByName( buffer );
		mLevelButtons.push_back( levelButton );
		levelButton->SetID( BUTTON_LEVEL_ID + i );        
		levelButton->SetDisabledAlpha( 0.5f );
		levelButton->SetVisible( false );
	}

	GuiStatic* countItem = NULL;
	for ( int i = 0; i < MAX_LEVELS_PER_WORLD; ++i )
	{
		snprintf( buffer, 64, "Count%02i", i+1 );
		countItem = GetStaticByName( buffer );
		mCountItems.push_back( countItem );
		countItem->SetText( "0" );
	}

	mCollectedText = GetStaticByName( "CollectText" );

	SetCurrentWorldVisible( mCurrentWorld );

	BowlerGuiLevelEnableMessage* bowlerGuiLevelEnableMessage = NEW_PTR( "Message" ) BowlerGuiLevelEnableMessage;    
	GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiLevelEnableMessage );
}

//===========================================================================

void BowlerSelectGui::OnEvent( EventType event, int controlID, GuiControl* /*control*/ )
{
	if ( event == EVENT_BUTTON_CLICKED )
	{
		if ( controlID >= BUTTON_LEVEL_ID && controlID <= BUTTON_LEVEL_ID + MAX_LEVELS )
		{
			mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

			int level = controlID - BUTTON_LEVEL_ID;

			BowlerStateChangeMessage* bowlerStateChangeMessage = NEW_PTR( "Message" ) BowlerStateChangeMessage;
			bowlerStateChangeMessage->mToState = BOWLER_STATE_TYPE_RUN;
			bowlerStateChangeMessage->mLevel = level + 1;
			GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerStateChangeMessage );

			BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
			bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_LOAD;
			bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CHANGE;
			GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );
		}

		switch ( controlID )
		{
		case GUI_BUTTON_OPTIONS:
			{
				mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

				BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
				bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_OPTIONS;
				bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_OPEN;
				GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );

				mGuiSelectState = GUI_SELECT_STATE_OPTIONS;
			}
			break;    
		case GUI_BUTTON_NEXT:
			{
				mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

				mCurrentWorld++;
				if ( mCurrentWorld == 2 )
				{
				}
				mCurrentWorld = mCurrentWorld % 3;

				SetCurrentWorldVisible( mCurrentWorld );

				UpdateCurrentWorldCountItem( mCurrentWorld, mLastEnabledLevel );
			}
			break;
		case GUI_BUTTON_BACK:
			{
				mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

				mCurrentWorld--;
				if ( mCurrentWorld == 0 )
				{
				}
				mCurrentWorld = mCurrentWorld % 3;

				SetCurrentWorldVisible( mCurrentWorld );

				UpdateCurrentWorldCountItem( mCurrentWorld, mLastEnabledLevel );

				mGuiSelectState = GUI_SELECT_STATE_LEVEL_SELECT;
			}
			break;
		case GUI_BUTTON_STATS:
			{
				mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

				BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
				bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_STATS;
				bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_OPEN;
				GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );

				mGuiSelectState = GUI_SELECT_STATE_STATS;
			}
			break;
		default:
			break;
		}
	}
}

//===========================================================================

void BowlerSelectGui::SetCurrentWorldVisible( int currentWorld )
{
	for ( int i = 0; i < MAX_LEVELS; ++i )
	{
		GuiButton* levelButton = mLevelButtons[i];
		levelButton->SetVisible( false );        
	}

	int levelIndex = MAX_LEVELS_PER_WORLD * currentWorld;
	for ( int i = levelIndex; i < levelIndex + MAX_LEVELS_PER_WORLD; ++i )
	{
		GuiButton* levelButton = mLevelButtons[i];
		levelButton->SetVisible( true );        
	}
}

//===========================================================================

void BowlerSelectGui::UpdateCurrentWorldCountItem( int currentWorld, int lastEnabledLevel )
{
	char buffer[8];
	for ( int i = 0; i < MAX_LEVELS_PER_WORLD; ++i )
	{
		int index = i + currentWorld * MAX_LEVELS_PER_WORLD;
		if ( index < lastEnabledLevel )
		{
			mCountItems[i]->SetVisible( false );
		}
		else
		{
			snprintf( buffer, 8, "%i", mCountPerLevel[index] );
			mCountItems[i]->SetText( buffer );
			mCountItems[i]->SetVisible( true );
		}
	}
}

//===========================================================================

void BowlerSelectGui::CheckLevelLocks()
{
	int totalCollected = 0;

	for ( int level = 1; level <= MAX_LEVELS; ++level )	
	{
		DatabaseDataUser data = mBowlerDatabase->GetUserData( level );
		totalCollected += data.mNumCollected;
	}

	mTotalCollected = totalCollected;

	for ( int level = 1; level <= MAX_LEVELS; ++level )	
	{
		DatabaseDataReadOnly readOnlyData = mBowlerDatabase->GetReadOnlyData( level );

#if defined( DISABLE_LEVEL_LOCKS )
		mIsUnlocked[level-1] = true;
#else
		mIsUnlocked[level-1] = (readOnlyData.mNumToUnlock <= totalCollected);
#endif
		mCountPerLevel[level-1] = readOnlyData.mNumToUnlock;
	}
}