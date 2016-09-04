#ifndef BOWLER_RUN_GUI_HPP
#define BOWLER_RUN_GUI_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guiscreen.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BowlerDatabase;
class GuiButton;
class GuiInput;

namespace System
{
	class Timer;
}

namespace Audio
{
	class AudioManager;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_COLLECTIBLES_PER_LEVEL = 5;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerRunGui : public GuiScreen
{
public:

	enum GuiButtonID
	{
		GUI_BUTTON_PAUSE,
		GUI_BUTTON_MENU,
		GUI_BUTTON_LEVEL_COMPLETED,
		GUI_BUTTON_MENU_GO,
		GUI_BUTTON_MENU_TIME_ATTACK,
		GUI_BUTTON_MENU_NEXT,
		GUI_BUTTON_MENU_REPLAY,
		GUI_BUTTON_MENU_CLOSE
	};

    enum GuiRunState
    {
        GUI_RUN_STATE_NONE,
        GUI_RUN_STATE_START,
        GUI_RUN_STATE_RUNNING,
        GUI_RUN_STATE_PAUSED,
        GUI_RUN_STATE_COMPLETED
    };

	BowlerRunGui( Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase );
	~BowlerRunGui();

	void OnMessageCallback( void* userData );
	void Initialize();
    bool UpdateInput( GuiInput* input );
	void Update( float elapsedTime );
	void OnEvent( EventType event, int controlID, GuiControl* control );

protected:

    void ShowLevelCompleteDialog( bool visible );
    void ShowGoDialog( bool visible );
    void ShowPauseDialog( bool visible, bool closeButton = true, bool timesUp = false );
	void ShowInGameGUI( bool visible );

    Audio::AudioManager* mAudioManager;
	BowlerDatabase* mBowlerDatabase;

	System::Timer* mTimer;
	float mTotalTime;

	int mCurrentLevel;

	GuiButton* mPauseButton;	
	GuiStatic* mBackground;
	GuiStatic* mBackgroundFade;

	GuiButton* mMenuButton;
	GuiButton* mReplayButton;
	GuiButton* mNextButton;
	GuiButton* mCloseButton;
	GuiStatic* mLock;
	GuiStatic* mTimesUp;

	GuiStatic* mCollectiblesInGame;
	GuiStatic* mCollectScore;
    GuiButton* mGoButton;	
	GuiButton* mTimeAttackButton;
	GuiStatic* mLockTimeAttack;

	GuiStatic* mBackgroundSliderCloud;
	GuiStatic* mBackgroundSliderColor;

	GuiButton* mLevelCompletedButton;
	GuiStatic* mLevelCompletedLabel;
	GuiStatic* mFinishLabel;
	GuiStatic* mScoreLabel;
    GuiStatic* mCollectiblesFinish;	    
	GuiStatic* mCompletionTime;
	GuiStatic* mCompleteTime;
    
	Math::Vector4 mIconColor;
	int mCurrentCollect;
    GuiRunState mGuiRunState;

	int mCount;
	float mTimeCount;

    bool mIsDialogOpened;
};

#endif
