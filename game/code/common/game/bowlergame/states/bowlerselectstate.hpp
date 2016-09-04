#ifndef BOWLER_SELECT_STATE_HPP
#define BOWLER_SELECT_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/states/bowlerstate.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BowlerInput;
class PhysicsManager;
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

class BowlerSelectState : public BowlerState
{
public:

	BowlerSelectState( 
		BowlerInput* bowlerInput, 
		PhysicsManager* physicsManager,
        Audio::AudioManager* audioManager,
		BowlerDatabase* bowlerDatabase
		);
	~BowlerSelectState();

	void Initialize();
    void Shutdown()
    {
    }

	void Update( float elapsedTime );
	void RenderUpdate();

private:
    
};

#endif