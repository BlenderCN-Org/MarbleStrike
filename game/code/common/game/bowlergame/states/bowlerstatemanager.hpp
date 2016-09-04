#ifndef BOWLER_STATE_MANAGER_HPP
#define BOWLER_STATE_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BowlerState;
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

enum BowlerStateType
{
	BOWLER_STATE_TYPE_START,
	BOWLER_STATE_TYPE_SELECT,
	BOWLER_STATE_TYPE_RUN
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerStateManager
{
public:

	BowlerStateManager( 
		BowlerInput* bowlerInput, 
		PhysicsManager* physicsManager,
        Audio::AudioManager* audioManager,
		BowlerDatabase* bowlerDatabase 
		);
	~BowlerStateManager();

	void OnMessageCallback( void* userData );
	void Update( float elapsedTime );
	void RenderUpdate();
	BowlerStateType GetBowlerStateType()
	{
		return mCurrentStateType;
	}

private:

	void ChangeState( BowlerStateType state );

	int mCurrentLevel;
	BowlerStateType mCurrentStateType;
	BowlerState* mCurrentState;
	BowlerInput* mBowlerInput;
	PhysicsManager* mPhysicsManager;
    Audio::AudioManager* mAudioManager;
	BowlerDatabase* mBowlerDatabase;
};

#endif