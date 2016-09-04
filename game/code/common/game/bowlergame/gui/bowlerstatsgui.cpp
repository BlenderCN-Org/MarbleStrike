//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guiinput.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/game/bowlergame/bowlerutilities.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/gui/bowlerstatsgui.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"
#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerStatsGui::BowlerStatsGui( Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase )
	:	mAudioManager( audioManager )
	,	mBowlerDatabase( bowlerDatabase )
	,	GuiScreen( BOWLER_GUI_STATS )
{
	GameApp::Get()->GetMessageHandler()->RegisterListener( PORT_GAME, &BowlerStatsGui::OnMessageCallback, this );
}

//===========================================================================

BowlerStatsGui::~BowlerStatsGui()
{	
	GameApp::Get()->GetMessageHandler()->UnRegisterListener( PORT_GAME, &BowlerStatsGui::OnMessageCallback, this );
}

//===========================================================================

void BowlerStatsGui::OnMessageCallback( void* userData )
{
	Message* message = reinterpret_cast<Message*>( userData );
	switch ( message->GetType() )
	{
	case BOWLER_MESSAGE_EVENT:
		{
			BowlerEventMessage* msg = reinterpret_cast<BowlerEventMessage*>( message );

			if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_BACK_BUTTON_PRESS )
			{
				mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);
				OnEvent( EVENT_BUTTON_CLICKED, GUI_BUTTON_BACK, NULL );				
			}
		}
		break;
	default:
		break;
	}
}

//===========================================================================

void BowlerStatsGui::Initialize()
{
	const char* resolutionString = GameApp::GetResolutionString();
    std::string filename = "game\\bowler\\gui\\bowlerstatsgui";  
    filename += resolutionString;
    filename += ".xml";
    Load( filename.c_str() );

	GuiButton* backButton = GetButtonByName( "BackButton" );
	backButton->SetID( GUI_BUTTON_BACK );

	mLeftButton = GetButtonByName( "LeftButton" );
	mLeftButton->SetID( GUI_BUTTON_PREV );
	mLeftButton->SetEnabled( false );

	mRightButton = GetButtonByName( "RightButton" );
	mRightButton->SetID( GUI_BUTTON_NEXT );

	mToggleViewButton = GetButtonByName( "ToggleView" );
	mToggleViewButton->SetID( GUI_BUTTON_TOGGLE_VIEW );

	mBestTimeLabel = GetStaticByName( "timebest" );

	mBestAttackTimeLabel = GetStaticByName( "attacktimebest" );
	mBestAttackTimeLabel->SetVisible( false );

	mCollectImage = GetStaticByName( "collect" );

	mPageIndex = 0;

	char buffer[64];
	for ( int i = 0; i < MAX_ITEMS_PER_PAGE; ++i )
	{
		int level = i + 1;

		snprintf( buffer, 64, "level%02ititle", level );
		mLevelTitle[i] = GetStaticByName( buffer );

		snprintf( buffer, 64, "level%02icollect", level );
		mLevelCollect[i] = GetStaticByName( buffer );

		snprintf( buffer, 64, "level%02itime", level );
		mLevelTime[i] = GetStaticByName( buffer );
	}

	mViewState = VIEW_STATE_MAIN;
	ShowSummary( mPageIndex, mViewState );
}

//===========================================================================

void BowlerStatsGui::OnEvent( EventType event, int controlID, GuiControl* /*control*/ )
{
	if ( event == EVENT_BUTTON_CLICKED )
	{
		mAudioManager->PlayAudio( SOUND_TYPE_BUTTON_CLICK );

		switch ( controlID )
		{
		case GUI_BUTTON_BACK:
			{
				BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
				bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CLOSE;
				GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );
			}
			break;
		case GUI_BUTTON_PREV:
			{
				mLeftButton->SetEnabled( true );
				mRightButton->SetEnabled( true );

				mPageIndex--;
				if ( mPageIndex <= 0 )
				{
					mPageIndex = 0;
					mLeftButton->SetEnabled( false );
				}

				ShowSummary( mPageIndex, mViewState );
			}
			break;
		case GUI_BUTTON_NEXT:
			{
				mLeftButton->SetEnabled( true );
				mRightButton->SetEnabled( true );

				mPageIndex++;
				if ( mPageIndex >= 2 )
				{
					mPageIndex = 2;
					mRightButton->SetEnabled( false );
				}

				ShowSummary( mPageIndex, mViewState );
			}
			break;
		case GUI_BUTTON_TOGGLE_VIEW:
			{
				mBestTimeLabel->SetVisible( !( mBestTimeLabel->GetVisible() ) );
				mBestAttackTimeLabel->SetVisible( !( mBestAttackTimeLabel->GetVisible() ) );
				mCollectImage->SetVisible( !( mCollectImage->GetVisible() ) );

				if ( mViewState == VIEW_STATE_MAIN )
				{
					mViewState = VIEW_STATE_TIME_ATTACK;
				}
				else if ( mViewState == VIEW_STATE_TIME_ATTACK )
				{
					mViewState = VIEW_STATE_MAIN;
				}

				ShowSummary( mPageIndex, mViewState );
			}
			break;
		}
	}
}

//===========================================================================

void BowlerStatsGui::ShowSummary( int page, ViewState viewState )
{
	char buffer[64];
	for ( int i = 0; i < MAX_ITEMS_PER_PAGE; ++i )
	{
		int level = ( page * MAX_ITEMS_PER_PAGE ) + i + 1;
		DatabaseDataUser data = mBowlerDatabase->GetUserData( level );

		snprintf( buffer, 64, "%i",level );
		mLevelTitle[i]->SetText( buffer );

		if ( viewState == VIEW_STATE_MAIN )
		{		
			snprintf( buffer, 64, "%i/5", data.mNumCollected );
			mLevelCollect[i]->SetText( buffer );		
			mLevelCollect[i]->SetVisible( true );

			int minutes = 0;
			int seconds = 0;
			int fraction = 0;
			BowlerUtilities::ExtractTime( data.mBestTime, minutes, seconds, fraction );

			snprintf( buffer, 64, "%02i'%02i\"%02i", minutes, seconds, fraction  );
			mLevelTime[i]->SetText( buffer );
		}
		else if ( viewState == VIEW_STATE_TIME_ATTACK )
		{
			mLevelCollect[i]->SetVisible( false );

			int minutes = 0;
			int seconds = 0;
			int fraction = 0;
			BowlerUtilities::ExtractTime( data.mBestTimeAttack, minutes, seconds, fraction );

			snprintf( buffer, 64, "%02i'%02i\"%02i", minutes, seconds, fraction  );
			mLevelTime[i]->SetText( buffer );
		}
	}
}