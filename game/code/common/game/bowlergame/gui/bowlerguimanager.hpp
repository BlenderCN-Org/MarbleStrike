#ifndef BOWLER_GUI_MANAGER_HPP
#define BOWLER_GUI_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guimanager.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
	class AudioManager;
}

class BowlerDatabase;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum BowlerGuiStateType
{
	BOWLER_GUI_START,
	BOWLER_GUI_SELECT,
	BOWLER_GUI_OPTIONS,
	BOWLER_GUI_RUN,
    BOWLER_GUI_LOAD,
	BOWLER_GUI_STATS,
	BOWLER_GUI_MAX
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerGuiManager : public GuiManager
{
public:

	BowlerGuiManager(Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase );
	~BowlerGuiManager();

	void Initialize();
	void Shutdown();
	void OnMessageCallback( void* userData );	
	void OpenScreen( BowlerGuiStateType state );
	void CloseScreen();
	int GetCurrentScreenType();

private:
    Audio::AudioManager* mAudioManager;
    BowlerDatabase* mBowlerDatabase;
};

#endif

