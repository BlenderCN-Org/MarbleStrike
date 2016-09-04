#ifndef BOWLER_SELECT_GUI_HPP
#define BOWLER_SELECT_GUI_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guiscreen.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
	class AudioManager;
}

class SpriteAnimated;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int BUTTON_LEVEL_ID = 1000;
const int MAX_WORLDS = 3;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerSelectGui : public GuiScreen
{
public:

	enum GuiButtonID
	{
		GUI_BUTTON_OPTIONS,		        
        GUI_BUTTON_NEXT,
        GUI_BUTTON_BACK,
		GUI_BUTTON_STATS
	};

    enum GuiSelectState
    {
        GUI_SELECT_STATE_LEVEL_SELECT,
		GUI_SELECT_STATE_OPTIONS,
		GUI_SELECT_STATE_STATS,
    };

	BowlerSelectGui(Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase );
	~BowlerSelectGui();

    void OnMessageCallback( void* userData );
	void Initialize();
	void OnEvent( EventType event, int controlID, GuiControl* control );

protected:

    void SetCurrentWorldVisible( int currentWorld );
    void UpdateCurrentWorldCountItem( int currentWorld, int lastEnabledLevel );
    void CheckLevelLocks();
    
    bool mIsUnlocked[MAX_LEVELS];
    int mTotalCollected;    
    int mLastEnabledLevel;

    Audio::AudioManager* mAudioManager;
    BowlerDatabase* mBowlerDatabase;

	GuiSelectState mGuiSelectState;
    
    int mCurrentWorld;
    std::vector<GuiButton*> mLevelButtons;
    std::vector<GuiStatic*> mCountItems;
    int mCountPerLevel[MAX_LEVELS_PER_WORLD];
    GuiStatic* mCollectedText;
};

#endif
