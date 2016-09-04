#ifndef BOWLER_DATABASE_HPP
#define BOWLER_DATABASE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "tinyxml2.h"
#include <vector>
#include <string>
#include "common/game/bowlergame/bowleraudio.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_LEVELS = 15;
const int MAX_LEVELS_PER_WORLD = 15;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

enum ControlPosition
{
	ControlPosition_Left,
	ControlPosition_Right
};

enum GameMode
{
	GameMode_Main,
	GameMode_TimeAttack
};

struct DatabaseDataUser
{
	DatabaseDataUser()
	{
		mNumCollected = 0;
		mBestTime = 0;
		mBestTimeAttack = 0;
		mCompleted = false;
	}

	int mNumCollected;
	float mBestTime;
	float mBestTimeAttack;
	bool mCompleted;
};

struct DatabaseDataReadOnly
{
	DatabaseDataReadOnly()
	{
        mTimeAttackTime = 0;
        mNumToUnlock = 0;
	}

    std::string mBgmFileName;
    float mTimeAttackTime;
    int mNumToUnlock;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerDatabase
{
public:

	BowlerDatabase();
	~BowlerDatabase();

	void Load();
	void SaveUserData();

	DatabaseDataUser GetUserData( int level );
	DatabaseDataReadOnly GetReadOnlyData( int level );
	void SetUserData( int level, const DatabaseDataUser& data );

    void ResetData();

    MusicType GetMusicType( int level );

	void SetControlPosition( ControlPosition position );
	ControlPosition GetControlPosition();

	void SetGameMode( GameMode gameMode );
	GameMode GetGameMode();
		
private:

	void LoadDatabase( const char* filename, void (*Loader)( tinyxml2::XMLNode* node, BowlerDatabase* database ) );	
	static void LoadReadOnlyDatabase( tinyxml2::XMLNode* node, BowlerDatabase* database );
	static void LoadUserDatabase( tinyxml2::XMLNode* node, BowlerDatabase* database );

	std::vector<DatabaseDataUser> mDatabaseUser;
	std::vector<DatabaseDataReadOnly> mDatabaseReadOnly;
	ControlPosition mControlPosition;

	GameMode mGameMode;
};

#endif