#ifndef LEVEL_SCRIPT_6_HPP
#define LEVEL_SCRIPT_6_HPP

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

class LevelScript6 : public LevelScript
{
public:

	LevelScript6();
	~LevelScript6();

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

	bool mAllTriggered;

};

#endif