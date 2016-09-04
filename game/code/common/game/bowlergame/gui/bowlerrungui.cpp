//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guiinput.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/system/timer.hpp"
#include "common/engine/system/variable.hpp"
#include "common/engine/system/variablefloat.hpp"
#include "common/engine/system/variableint.hpp"
#include "common/game/bowlergame/bowleraudio.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/game/bowlergame/bowlerutilities.hpp"
#include "common/game/bowlergame/gui/bowlerguimanager.hpp"
#include "common/game/bowlergame/gui/bowlerrungui.hpp"
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

BowlerRunGui::BowlerRunGui( Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase )
    :	mAudioManager( audioManager )
    ,	mBowlerDatabase( bowlerDatabase )
    ,	GuiScreen( BOWLER_GUI_RUN )
{
    GameApp::Get()->GetMessageHandler()->RegisterListener( PORT_GAME, &BowlerRunGui::OnMessageCallback, this );
    mTimer = NEW_PTR( "Timer" ) System::Timer;
    mTimer->Start();
    mTotalTime = 0;
    mCurrentLevel = 0;
    mCurrentCollect = 0;
    mGuiRunState = GUI_RUN_STATE_NONE;
    mIsDialogOpened = false;
}

//===========================================================================

BowlerRunGui::~BowlerRunGui()
{
    DELETE_PTR( mTimer );
    GameApp::Get()->GetMessageHandler()->UnRegisterListener( PORT_GAME, &BowlerRunGui::OnMessageCallback, this );
}

//===========================================================================

void BowlerRunGui::OnMessageCallback( void* userData )
{
    Message* message = reinterpret_cast<Message*>( userData );
    switch ( message->GetType() )
    {
    case BOWLER_MESSAGE_EVENT:
        {
            BowlerEventMessage* msg = reinterpret_cast<BowlerEventMessage*>( message );

            if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_LEVEL_FINISH )
            {
                GameMode gameMode = mBowlerDatabase->GetGameMode();
                if ( gameMode == GameMode_TimeAttack )
                {
                    Variable* parameter = msg->GetParameter( 0 );
                    VariableFloat* parameterFloat = reinterpret_cast<VariableFloat*>( parameter );
                    float timeAttackTime = parameterFloat->GetValue();
                    char timeAttackTimeBuffer[16];
                    int minutes = 0;
                    int seconds = 0;
                    int fraction = 0;
                    BowlerUtilities::ExtractTime( timeAttackTime, minutes, seconds, fraction );
                    snprintf( timeAttackTimeBuffer, 16, "%02i'%02i\"%02i", minutes, seconds, fraction  );
                    mCompleteTime->SetText( timeAttackTimeBuffer );
                }

                mGuiRunState = GUI_RUN_STATE_COMPLETED;
                mLevelCompletedButton->SetDirty( true );

                mPauseButton->SetVisible( false );

                ShowLevelCompleteDialog( true );
            }
            else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_BACK_BUTTON_PRESS )
            {
				mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

                switch ( mGuiRunState )
                {
                case GUI_RUN_STATE_START:
                    if ( mIsDialogOpened == false )
                    {
                        ShowGoDialog( false );
                        ShowPauseDialog( true );
                    }
                    else
                    {
                        ShowPauseDialog( false );
                        ShowGoDialog( true );                        
                    }
                    break;
                case GUI_RUN_STATE_RUNNING:
                    {
                        if ( mIsDialogOpened == false )
                        {
                            OnEvent( EVENT_BUTTON_CLICKED, mPauseButton->GetID(), NULL );
                        }
                    }
                    break;
                case GUI_RUN_STATE_PAUSED:
                    {
                        if ( mIsDialogOpened )
                        {
                            OnEvent( EVENT_BUTTON_CLICKED, mCloseButton->GetID(), NULL );
                        }
                    }
                    break;
                case GUI_RUN_STATE_COMPLETED:
                    {
                        OnEvent( EVENT_BUTTON_CLICKED, mMenuButton->GetID(), NULL );
                    }
                    break;
                default:
                    break;
                }
            }
            else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_LEVEL_TIME_ATTACK_FAIL )
            {
                mGuiRunState = GUI_RUN_STATE_COMPLETED;
                mLevelCompletedButton->SetDirty( true );

                mPauseButton->SetVisible( false );

                ShowPauseDialog( true, false, true );
            }
            else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_LEVEL_START )
            {
                Variable* parameter = msg->GetParameter( 0 );
                VariableInt* parameterInt = reinterpret_cast<VariableInt*>( parameter );
                mCurrentLevel = parameterInt->GetValue();

                mGuiRunState = GUI_RUN_STATE_START;

                mPauseButton->SetVisible( false );

                ShowGoDialog( true );

                mCurrentCollect = 0;
            }
            else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_COLLECT )
            {
                mCurrentCollect++;

                char currentScore[8];
                snprintf( currentScore, 8, "x%i", mCurrentCollect );
                mCollectScore->SetText( currentScore );
            }
            else if ( msg->mBowlerEventType == BOWLER_EVENT_TYPE_REMOVE_COLLECT )
            {
                if ( mCurrentCollect > 0 )
                {
                    mCurrentCollect--;
                }

                char currentScore[8];
                snprintf( currentScore, 8, "x%i", mCurrentCollect );
                mCollectScore->SetText( currentScore );
            }
        }
        break;
    default:
        break;
    }
}

//===========================================================================

void BowlerRunGui::Initialize()
{
    mTotalTime = 0;

    const char* resolutionString = GameApp::GetResolutionString();
    std::string filename = "game\\bowler\\gui\\bowlerrungui";
    filename += resolutionString;
    filename += ".xml";
    Load( filename.c_str() );

    mPauseButton = GetButtonByName( "PauseButton" );
    mPauseButton->SetVisible( false );
    mPauseButton->SetID( GUI_BUTTON_PAUSE );

    mBackground = GetStaticByName( "Background" );
    mBackground->SetVisible( true );

    mBackgroundFade = GetStaticByName( "BackgroundFade" );
    mBackgroundFade->SetVisible( true );

    mMenuButton = GetButtonByName( "MenuButton" );
    mMenuButton->SetVisible( false );
    mMenuButton->SetID( GUI_BUTTON_MENU );

    mReplayButton = GetButtonByName( "ReplayButton" );
    mReplayButton->SetVisible( false );
    mReplayButton->SetID( GUI_BUTTON_MENU_REPLAY );

    mNextButton = GetButtonByName( "NextButton" );
    mNextButton->SetVisible( false );
    mNextButton->SetID( GUI_BUTTON_MENU_NEXT );

    mCloseButton = GetButtonByName( "CloseButton" );
    mCloseButton->SetVisible( false );
    mCloseButton->SetID( GUI_BUTTON_MENU_CLOSE );

    mLock = GetStaticByName( "Lock" );
    mLock->SetVisible( false );

    mTimesUp = GetStaticByName( "TimesUp" );
    mTimesUp->SetVisible( false );

    mCollectiblesInGame = GetStaticByName( "Collect" );
    mCollectiblesInGame->SetVisible( false );

    mCollectScore = GetStaticByName( "CollectScore" );
    mCollectScore->SetVisible( false );

    mGoButton = GetButtonByName( "GoButton" );
    mGoButton->SetVisible( true );
    mGoButton->SetID( GUI_BUTTON_MENU_GO );

    mLockTimeAttack = GetStaticByName( "LockTimeAttack" );
    mLockTimeAttack->SetVisible( true );

    mTimeAttackButton = GetButtonByName( "TimeAttackButton" );
    mTimeAttackButton->SetVisible( true );
    mTimeAttackButton->SetEnabled( false );
    mTimeAttackButton->SetDisabledAlpha( 0.5f );
    mTimeAttackButton->SetID( GUI_BUTTON_MENU_TIME_ATTACK );

    mBackgroundSliderCloud = GetStaticByName( "BackgroundSliderCloud" );
    mBackgroundSliderCloud->SetVisible( false );

    mBackgroundSliderColor = GetStaticByName( "BackgroundSliderColor" );
    mBackgroundSliderColor->SetVisible( false );

    mLevelCompletedLabel = GetStaticByName( "CompleteLabel" );
    mLevelCompletedLabel->SetVisible( false );

    mLevelCompletedButton = GetButtonByName( "CompleteButton" );
    mLevelCompletedButton->SetVisible( false );
    mLevelCompletedButton->SetID( GUI_BUTTON_LEVEL_COMPLETED );

    mCompletionTime = GetStaticByName( "CompletionTime" );
    mCompletionTime->SetVisible( false );

    mCompleteTime = GetStaticByName( "CompleteTime" );
    mCompleteTime->SetVisible( false );

    mFinishLabel = GetStaticByName( "FinishLabel" );
    mFinishLabel->SetVisible( false );

    mScoreLabel = GetStaticByName( "ScoreLabel" );
    mScoreLabel->SetVisible( false );

    mCollectiblesFinish = GetStaticByName( "CompleteScore" );
    mCollectiblesFinish->SetVisible( false );

    mCurrentCollect = 0;

    mTimeCount = 0;
    mCount = 0;
}


//===========================================================================

bool BowlerRunGui::UpdateInput( GuiInput* input )
{
    Assert( input != NULL, "" );

    return GuiScreen::UpdateInput( input );
}

//===========================================================================

void BowlerRunGui::Update( float elapsedTime )
{
    if ( mBackgroundSliderColor->GetAnimationState() == ANIM_STATE_STOPPED && mReplayButton->GetVisible() )
    {
        mBackgroundSliderCloud->SetVisible( false );
        mBackgroundSliderColor->SetVisible( false );
    }

    if ( mCollectiblesFinish->GetVisible() && mBackgroundSliderColor->GetAnimationState() == ANIM_STATE_STOPPED && mBackgroundSliderColor->GetVisible() )
    {
        mTimeCount += elapsedTime;
        if ( mTimeCount > 0.05f )
        {
            char buffer[8];
            snprintf( buffer, 8, "x%i", mCount );
            mCollectiblesFinish->SetText( buffer );
            mTimeCount = 0;
            if ( mCount < mCurrentCollect )
            {
                mCount++;
            }
        }
    }

    GuiScreen::Update( elapsedTime );
}

//===========================================================================

void BowlerRunGui::OnEvent( EventType event, int controlID, GuiControl* /*control*/ )
{
    if ( event == EVENT_BUTTON_CLICKED )
    {
        mAudioManager->PlayAudio(SOUND_TYPE_BUTTON_CLICK);

        switch ( controlID )
        {
        case GUI_BUTTON_MENU_CLOSE:
            {
                mGuiRunState = GUI_RUN_STATE_RUNNING;

                BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
                bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_UNPAUSE;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );

                ShowInGameGUI( true );
                ShowPauseDialog( false );
            }
            break;
        case GUI_BUTTON_PAUSE:
            {
                mGuiRunState = GUI_RUN_STATE_PAUSED;

                BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
                bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_PAUSE;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );

                ShowInGameGUI( false );
                ShowPauseDialog( true );
            }
            break;
        case GUI_BUTTON_MENU:
            {
                mGuiRunState = GUI_RUN_STATE_NONE;

                ShowInGameGUI( false );

                BowlerStateChangeMessage* bowlerStateChangeMessage = NEW_PTR( "Message" ) BowlerStateChangeMessage;
                bowlerStateChangeMessage->mToState = BOWLER_STATE_TYPE_SELECT;
                bowlerStateChangeMessage->mLevel = 1;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerStateChangeMessage );

                BowlerGuiEventMessage* bowlerGuiEventMessage = NEW_PTR( "Message" ) BowlerGuiEventMessage;
                bowlerGuiEventMessage->mToGuiType = BOWLER_GUI_SELECT;
                bowlerGuiEventMessage->mEventType = BOWLER_GUI_EVENT_TYPE_CHANGE;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerGuiEventMessage );

            }
            break;
        case GUI_BUTTON_MENU_GO:
            {
                mGuiRunState = GUI_RUN_STATE_RUNNING;

                mBowlerDatabase->SetGameMode( GameMode_Main );

                ShowInGameGUI( true );

                ShowGoDialog( false );

                BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
                bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_START_GO;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
            }
            break;
        case GUI_BUTTON_MENU_TIME_ATTACK:
            {
                mGuiRunState = GUI_RUN_STATE_RUNNING;

                mBowlerDatabase->SetGameMode( GameMode_TimeAttack );

                ShowInGameGUI( true );

                ShowGoDialog( false );

                BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
                bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_START_GO;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
            }
            break;
        case GUI_BUTTON_LEVEL_COMPLETED:
            {
                ShowInGameGUI( false );
                ShowLevelCompleteDialog( false );
                mLevelCompletedButton->SetVisible( false );
                ShowPauseDialog( true, false );
                mBackgroundSliderColor->SetVisible( true );
                int animationIndex = mBackgroundSliderColor->FindAnimationSequenceIndexByName( "Fly-out" );
                mBackgroundSliderColor->SetCurrentAnimationSequence( animationIndex );
                mBackgroundSliderColor->PlayAnimation();

                mBackgroundSliderCloud->SetVisible( true );
                animationIndex = mBackgroundSliderCloud->FindAnimationSequenceIndexByName( "Fly-out" );
                mBackgroundSliderCloud->SetCurrentAnimationSequence( animationIndex );
                mBackgroundSliderCloud->PlayAnimation();

                mBackgroundSliderCloud->SetVisible( true );
            }
            break;
        case GUI_BUTTON_MENU_NEXT:
            {
                ShowInGameGUI( false );

                ShowPauseDialog( false );
                ShowGoDialog( true );

                BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
                bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_NEXT_LEVEL;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
            }
            break;
        case GUI_BUTTON_MENU_REPLAY:
            {
                mGuiRunState = GUI_RUN_STATE_START;

                ShowInGameGUI( false );

                ShowLevelCompleteDialog( false );
                ShowPauseDialog( false );
                ShowGoDialog( true );

                BowlerEventMessage* bowlerEventMessage = NEW_PTR( "Message" ) BowlerEventMessage;
                bowlerEventMessage->mBowlerEventType = BOWLER_EVENT_TYPE_REPLAY;
                GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, bowlerEventMessage );
            }
            break;
        }
    }
}

//===========================================================================

void BowlerRunGui::ShowLevelCompleteDialog( bool visible )
{
    mLevelCompletedButton->SetVisible( visible );
    mBackgroundSliderColor->SetVisible( visible );
    int animationIndex = mBackgroundSliderColor->FindAnimationSequenceIndexByName( "Fly-in" );
    mBackgroundSliderColor->SetCurrentAnimationSequence( animationIndex );
    mBackgroundSliderColor->PlayAnimation();

    mBackgroundSliderCloud->SetVisible( visible );
    animationIndex = mBackgroundSliderCloud->FindAnimationSequenceIndexByName( "Fly-in" );
    mBackgroundSliderCloud->SetCurrentAnimationSequence( animationIndex );
    mBackgroundSliderCloud->PlayAnimation();

    mLevelCompletedLabel->SetVisible( visible );

    GameMode gameMode = mBowlerDatabase->GetGameMode();
    if ( gameMode == GameMode_Main )
    {
        mScoreLabel->SetVisible( visible );
        mCollectiblesFinish->SetVisible( visible );
        mFinishLabel->SetVisible( visible );
    }
    else if ( gameMode == GameMode_TimeAttack )
    {
        mCompleteTime->SetVisible( visible );
        mCompletionTime->SetVisible( visible );
    }
}

//===========================================================================

void BowlerRunGui::ShowGoDialog( bool visible )
{
    mGoButton->SetVisible( visible );
    mTimeAttackButton->SetVisible( visible );
    DatabaseDataUser data = mBowlerDatabase->GetUserData( mCurrentLevel );
    if ( data.mNumCollected >= MAX_COLLECTIBLES_PER_LEVEL )
    {
        mLockTimeAttack->SetVisible( false );
        mTimeAttackButton->SetEnabled( true );
    }
    else
    {
        mLockTimeAttack->SetVisible( visible );
        mTimeAttackButton->SetEnabled( false );
    }
    mBackground->SetVisible( visible );
    mBackgroundFade->SetVisible( visible );
}

//===========================================================================

void BowlerRunGui::ShowPauseDialog( bool visible, bool closeButton, bool timesUp )
{
    mIsDialogOpened = visible;
    mReplayButton->SetVisible( visible );
    mMenuButton->SetVisible( visible );
    mNextButton->SetVisible( visible );
    mCloseButton->SetVisible( visible && closeButton );
    mBackground->SetVisible( visible );
    mBackgroundFade->SetVisible( visible );
    mTimesUp->SetVisible( visible && timesUp );

    if ( visible )
    {
        int totalCollected = 0;
        for ( int level = 1; level <= MAX_LEVELS; ++level )
        {
            DatabaseDataUser data = mBowlerDatabase->GetUserData( level );
            totalCollected += data.mNumCollected;
        }

        int nextLevel = mCurrentLevel + 1;
        if ( nextLevel <= MAX_LEVELS )
        {
            DatabaseDataReadOnly readOnlyData = mBowlerDatabase->GetReadOnlyData( nextLevel );
            if ( totalCollected < readOnlyData.mNumToUnlock )
            {
                mNextButton->SetEnabled( false );
                mLock->SetVisible( true );
            }
        }
		else
		{
			mNextButton->SetEnabled( false );
		}
    }
    else
    {
        mLock->SetVisible( false );
    }
}

//===========================================================================

void BowlerRunGui::ShowInGameGUI( bool visible )
{
    mPauseButton->SetVisible( visible );

    GameMode gameMode = mBowlerDatabase->GetGameMode();
    if ( gameMode == GameMode_Main )
    {
        mCollectiblesInGame->SetVisible( visible );
        mCollectScore->SetVisible( visible );
    }
}