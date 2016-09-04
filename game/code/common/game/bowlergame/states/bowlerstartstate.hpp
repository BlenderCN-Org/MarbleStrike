#ifndef BOWLER_START_STATE_HPP
#define BOWLER_START_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/states/bowlerstate.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BlenderAsset;
class BowlerInput;
class PhysicsManager;
class RenderObject;
class Texture;

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

class BowlerStartState : public BowlerState
{
public:

    BowlerStartState( 
        BowlerInput* bowlerInput, 
        PhysicsManager* physicsManager,
        Audio::AudioManager* audioManager,
        BowlerDatabase* bowlerDatabase
        );
    ~BowlerStartState();

    void Initialize();
    void Shutdown()
    {
    }

    void Update( float elapsedTime );
    void RenderUpdate();

private:

    float mTotalElapsedTime;
    bool mMusicStarted;
};

#endif