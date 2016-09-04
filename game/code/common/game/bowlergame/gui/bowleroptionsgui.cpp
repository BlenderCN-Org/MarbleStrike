//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/animation/interpolators.hpp"
#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/gui/bowleroptionsgui.hpp"
#include "common/game/bowlergame/messages/bowlereventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguieventmessage.hpp"
#include "common/game/bowlergame/messages/bowlerguilevelenablemessage.hpp"
#include "common/game/bowlergame/messages/bowlermessage.hpp"
#include "common/game/bowlergame/messages/bowlerstatechangemessage.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerOptionsGui::BowlerOptionsGui( Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase )
	:	GuiScreen( BOWLER_GUI_OPTIONS )
{
	GameApp::Get()->GetMessageHandler()->RegisterListener( PORT_GAME, &BowlerOptionsGui::OnMessageCallback, this );
	mAudioManager = audioManager;
	mBowlerDatabase = bowlerDatabase;
}

//===========================================================================

BowlerOptionsGui::~BowlerOptionsGui()
{
	GameApp::Get()->GetMessageHandler()->UnRegisterListener( PORT_GAME, &BowlerOptionsGui::OnMessageCallback, this );
}

//===========================================================================

void BowlerOptionsGui::OnMessageCallback( void* userData )
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

				switch ( mOptionGuiState )
				{
				case OPTION_GUI_STATE_MAIN:
					OnEvent( EVENT_BUTTON_CLICKED, mBackButton->GetID(), NULL );
					break;
				case OPTION_GUI_STATE_RESET:
					CloseResetDialog();
					break;
				case OPTION_GUI_STATE_CONTROLS:
					CloseControlDialog();
					break;
				case OPTION_GUI_STATE_CREDITS:
					OnEvent( EVENT_BUTTON_CLICKED, mCreditBackButton->GetID(), NULL );					
					break;
				default:
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

//===========================================================================

void BowlerOptionsGui::Initialize()
{
	const char* resolutionString = GameApp::GetResolutionString();
	std::string filename = "game\\bowler\\gui\\bowleroptionsgui";
	filename += resolutionString;
	filename += ".xml";
	Load( filename.c_str() );

	mBackButton = GetButtonByName( "BackButton" );
	mBackButton->SetID( GUI_BUTTON_BACK );

	mOptionsTitle = GetStaticByName( "OptionsTitle" );

	mResetDataButton = GetButtonByName( "ResetDataButton" );
	mResetDataButton->SetID( GUI_BUTTON_RESET_DATA );

	mSwapControlButton = GetButtonByName( "SwapControlButton" );
	mSwapControlButton->SetID( GUI_BUTTON_SWAP_CONTROL );

	mCreditsButton = GetButtonByName( "CreditButton" );
	mCreditsButton->SetID( GUI_BUTTON_CREDITS );

	mResetTitle = GetStaticByName( "ResetTitle" );
	mResetTitle->SetVisible( false );

	mYesButton = GetButtonByName( "YesButton" );
	mYesButton->SetID( GUI_BUTTON_RESET_YES );
	mYesButton->SetVisible( false );

	mNoButton = GetButtonByName( "NoButton" );
	mNoButton->SetID( GUI_BUTTON_RESET_NO );
	mNoButton->SetVisible( false );

	mControlTitle = GetStaticByName( "ControlTitle" );
	mControlTitle->SetVisible( false );

	mLeftButton = GetButtonByName( "LeftButton" );
	mLeftButton->SetID( GUI_BUTTON_SWAP_LEFT );
	mLeftButton->SetVisible( false );

	mRightButton = GetButtonByName( "RightButton" );
	mRightButton->SetID( GUI_BUTTON_SWAP_RIGHT );
	mRightButton->SetVisible( false );

	mLeftButtonColor = mLeftButton->GetColor();
	mRightButtonColor = mRightButton->GetColor();
	UpdateControlVisuals();

	mCreditTitle = GetStaticByName( "CreditTitle" );

	mCreditBackButton = GetButtonByName( "CreditBackButton" );
	mCreditBackButton->SetID( GUI_BUTTON_CREDITS_BACK );

	mCredits[0].push_back( GetStaticByName( "CreditText01" ) );
	mCredits[0].push_back( GetStaticByName( "CreditText02" ) );
	mCredits[0].push_back( GetStaticByName( "CreditText03" ) );
	mCredits[0].push_back( GetStaticByName( "CreditText04" ) );

	mCredits[1].push_back( GetStaticByName( "CreditText05" ) );
	mCredits[1].push_back( GetStaticByName( "CreditText07" ) );

	CreditsControlDialog( false );

	mAlphaAnimationController.SetPlaybackSettings( 24.0f );
	mAlphaAnimationController.AddKeyFrame( 0, 0 );
	mAlphaAnimationController.AddKeyFrame( 1.0f, 24 );
	mAlphaAnimationController.AddKeyFrame( 1.0f, 96 );
	mAlphaAnimationController.AddKeyFrame( 0, 120 );

	mCreditIndex = 0;

	mOptionGuiState = OPTION_GUI_STATE_MAIN;
}

//===========================================================================

void BowlerOptionsGui::Update( float elapsedTime )
{
	if ( mOptionGuiState == OPTION_GUI_STATE_CREDITS  )
	{
		if ( mAlphaAnimationController.GetState() == ANIM_STATE_PLAYING )
		{
			mAlphaAnimationController.Update( elapsedTime,  Animation::LinearInterpolation );
			float alpha = mAlphaAnimationController.GetAnimationData();

			for ( size_t i = 0; i < mCredits[mCreditIndex].size(); ++i )
			{
				Math::Vector4 fontColor = mCredits[mCreditIndex][i]->GetFontColor();
				fontColor[3] = alpha;
				mCredits[mCreditIndex][i]->SetFontColor( fontColor );
			}
		}
		else if ( mAlphaAnimationController.GetState() == ANIM_STATE_STOPPED )
		{
			mCreditIndex++;
			mCreditIndex = mCreditIndex % 2;
			mAlphaAnimationController.Play();
		}
	}

	GuiScreen::Update( elapsedTime );
}

//===========================================================================

void BowlerOptionsGui::OnEvent( EventType event, int controlID, GuiControl* /*control*/ )
{
	if ( event == EVENT_BUTTON_CLICKED )
	{
		mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

		switch ( controlID )
		{
		case GUI_BUTTON_BACK:
			{
				BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
				bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CLOSE;
				GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );
			}
			break;
		case GUI_BUTTON_RESET_DATA:
			{
				OptionsGui( false );
				ResetDialogDialog( true );

				mOptionGuiState = OPTION_GUI_STATE_RESET;
			}
			break;
		case GUI_BUTTON_SWAP_CONTROL:
			{
				UpdateControlVisuals();
				OptionsGui( false );
				SwapControlDialog( true );

				mOptionGuiState = OPTION_GUI_STATE_CONTROLS;
			}
			break;
		case GUI_BUTTON_CREDITS:
			{
				OptionsGui( false );
				CreditsControlDialog( true );

				for ( int creditIndex = 0; creditIndex < 2; ++creditIndex )
				{
					for ( size_t i = 0; i < mCredits[creditIndex].size(); ++i )
					{
						Math::Vector4 fontColor = mCredits[creditIndex][i]->GetFontColor();
						fontColor[3] = 0.0f;
						mCredits[creditIndex][i]->SetFontColor( fontColor );
					}
				}

				mCreditIndex = 0;

				mAlphaAnimationController.Play();

				mOptionGuiState = OPTION_GUI_STATE_CREDITS;
			}
			break;
		case GUI_BUTTON_CREDITS_BACK:
			{
				CreditsControlDialog( false );
				OptionsGui( true );
				mAlphaAnimationController.Stop();
				mOptionGuiState = OPTION_GUI_STATE_MAIN;
			}
			break;
		case GUI_BUTTON_RESET_YES:
			{
				mBowlerDatabase->ResetData();
				BowlerGuiLevelEnableMessage* bowlerGuiLevelEnableMessage = NEW_PTR( "Message" ) BowlerGuiLevelEnableMessage;
				GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiLevelEnableMessage );

				CloseResetDialog();
			}
			break;
		case GUI_BUTTON_RESET_NO:
			{
				CloseResetDialog();
			}
			break;
		case GUI_BUTTON_SWAP_LEFT:
			{
				mBowlerDatabase->SetControlPosition( ControlPosition_Left );
				mBowlerDatabase->SaveUserData();

				CloseControlDialog();
			}
			break;
		case GUI_BUTTON_SWAP_RIGHT:
			{
				mBowlerDatabase->SetControlPosition( ControlPosition_Right );
				mBowlerDatabase->SaveUserData();

				CloseControlDialog();
			}
			break;
		default:
			break;
		}
	}
}

//===========================================================================

void BowlerOptionsGui::ResetDialogDialog( bool show )
{
	mYesButton->SetVisible( show );
	mNoButton->SetVisible( show );
	mResetTitle->SetVisible( show );
}

//===========================================================================

void BowlerOptionsGui::SwapControlDialog( bool show )
{
	mLeftButton->SetVisible( show );
	mRightButton->SetVisible( show );
	mControlTitle->SetVisible( show );
}

//===========================================================================

void BowlerOptionsGui::CreditsControlDialog( bool show )
{
	mCreditTitle->SetVisible( show );
	mCreditBackButton->SetVisible( show );

	for ( int creditIndex = 0; creditIndex < 2; ++creditIndex )
	{
		for ( size_t i = 0; i < mCredits[creditIndex].size(); ++i )
		{
			mCredits[creditIndex][i]->SetVisible( show );
		}
	}
}

//===========================================================================

void BowlerOptionsGui::OptionsGui( bool show )
{
	mBackButton->SetVisible( show );
	mResetDataButton->SetVisible( show );
	mSwapControlButton->SetVisible( show );
	mCreditsButton->SetVisible( show );
	mOptionsTitle->SetVisible( show );
}

//===========================================================================

void BowlerOptionsGui::UpdateControlVisuals()
{
	ControlPosition controlPosition = mBowlerDatabase->GetControlPosition();
	switch ( controlPosition )
	{
	case ControlPosition_Left:
		{
			Math::Vector4 color = mLeftButtonColor;
			color[0] += 0.5f;
			color[1] += 0.5f;
			color[2] += 0.5f;
			mLeftButton->SetColor( color );
			mRightButton->SetColor( mRightButtonColor );
		}
		break;
	case ControlPosition_Right:
		{
			Math::Vector4 color = mRightButtonColor;
			color[0] += 0.5f;
			color[1] += 0.5f;
			color[2] += 0.5f;
			mRightButton->SetColor( color );
			mLeftButton->SetColor( mLeftButtonColor );
		}
		break;
	}
}

//===========================================================================

void BowlerOptionsGui::CloseResetDialog()
{
	OptionsGui( true );
	ResetDialogDialog( false );
	mOptionGuiState = OPTION_GUI_STATE_MAIN;
}

//===========================================================================

void BowlerOptionsGui::CloseControlDialog()
{
	OptionsGui( true );
	SwapControlDialog( false );
	mOptionGuiState = OPTION_GUI_STATE_MAIN;
}