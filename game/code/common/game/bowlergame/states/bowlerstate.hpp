#ifndef BOWLER_STATE_HPP
#define BOWLER_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

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

class BowlerState
{
public:

	BowlerState( 
		BowlerInput* bowlerInput, 
		PhysicsManager* physicsManager,
        Audio::AudioManager* audioManager,
		BowlerDatabase* bowlerDatabase
		);
	virtual ~BowlerState();

	virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
	virtual void Update( float elapsedTime ) = 0;
	virtual void RenderUpdate() = 0;

protected:

	BowlerInput* mBowlerInput;
	PhysicsManager* mPhysicsManager;
    Audio::AudioManager* mAudioManager;
	BowlerDatabase* mBowlerDatabase;

private:	
	
};

#endif