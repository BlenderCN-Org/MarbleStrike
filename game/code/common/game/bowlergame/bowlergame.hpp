#ifndef BOWLER_GAME_HPP
#define BOWLER_GAME_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/game.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class PhysicsManager;
class BowlerStateManager;
class BowlerGuiManager;
class BowlerInput;
class BowlerDatabase;

namespace Audio
{
	class AudioManager;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerGame : public Game
{
public:

	BowlerGame();
	~BowlerGame();

	void Initialize();
	void Update( float elapsedTime );
	void UpdateVariable( float elapsedTime );
	void RenderUpdate();
    void Show();
    void Hide();
	void SetVolume();
	void OnBackButton();
	bool IsExitable();

private:

	PhysicsManager* mPhysicsManager;
	BowlerStateManager* mBowlerStateManager;
	BowlerGuiManager* mBowlerGuiManager;
	BowlerInput* mBowlerInput;
	BowlerDatabase* mBowlerDatabase;
    Audio::AudioManager* mAudioManager;
};

#endif