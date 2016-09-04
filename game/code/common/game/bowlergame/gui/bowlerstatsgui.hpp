#ifndef BOWLER_STATS_GUI_HPP
#define BOWLER_STATS_GUI_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guiscreen.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BowlerDatabase;
class GuiControl;
class GuiStatic;

namespace Audio
{
	class AudioManager;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_ITEMS_PER_PAGE = 5;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerStatsGui : public GuiScreen
{
public:

	enum GuiButtonID
	{
		GUI_BUTTON_PREV,
		GUI_BUTTON_NEXT,
		GUI_BUTTON_BACK,
		GUI_BUTTON_TOGGLE_VIEW
	};

	enum ViewState
	{
		VIEW_STATE_MAIN,
		VIEW_STATE_TIME_ATTACK
	};

	BowlerStatsGui( Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase );
	~BowlerStatsGui();

	void OnMessageCallback( void* userData );
	void Initialize();
	void OnEvent( EventType event, int controlID, GuiControl* control );

protected:

	void ShowSummary( int page, ViewState viewState );

	Audio::AudioManager* mAudioManager;
	BowlerDatabase* mBowlerDatabase;

	GuiStatic* mLevelTitle[MAX_ITEMS_PER_PAGE];
	GuiStatic* mLevelCollect[MAX_ITEMS_PER_PAGE];
	GuiStatic* mLevelTime[MAX_ITEMS_PER_PAGE];
	GuiStatic* mLevelAttackTime;
	GuiButton* mToggleViewButton;
	GuiStatic* mBestTimeLabel;
	GuiStatic* mBestAttackTimeLabel;
	GuiStatic* mCollectImage;

	GuiButton* mLeftButton;
	GuiButton* mRightButton;

	int mPageIndex;

	ViewState mViewState;
};

#endif
