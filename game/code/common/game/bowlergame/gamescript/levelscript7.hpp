#ifndef LEVEL_SCRIPT_7_HPP
#define LEVEL_SCRIPT_7_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/gamescript/levelscript.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class LevelScript7 : public LevelScript
{
public:

	LevelScript7();
	~LevelScript7();

    void Initialize( 
		const std::vector<BowlerObject*> &objects, 
		const std::vector<BowlerObject*> &triggerObjects 
		);

    void Update( 
		float elapsedTime, 
		const std::vector<BowlerObject*> &objects, 
		const std::vector<BowlerObject*> &triggerObjects 
		);

	void Reset();
		
private:

	std::vector<BowlerObject*> mToggleObjects;
	std::vector<BowlerObject*> mTriggerToggleObjects;

	char mMovingPlatformName1[32];
	char mMovingPlatformName2[32];

	bool mAllTriggered;

};

#endif