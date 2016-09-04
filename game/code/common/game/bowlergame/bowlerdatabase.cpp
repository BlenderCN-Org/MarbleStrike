//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static MusicType gMusicLevelType[MUSIC_LEVEL_TYPE_MAX] =
{
#define MUSIC_LEVEL_TYPE_ENTRY( ENUM, MUSIC_TYPE ) MUSIC_TYPE,
	MUSIC_LEVEL_TUPLE
#undef MUSIC_LEVEL_TYPE_ENTRY
};

const char* gDatabaseReadOnlyFilename = "game\\bowler\\database_readonly.xml";
const char* gDatabaseUserFilename = "game\\bowler\\database_user.xml";

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerDatabase::BowlerDatabase()
{
	mDatabaseUser.resize( MAX_LEVELS );
	mDatabaseReadOnly.resize( MAX_LEVELS );
	mControlPosition = ControlPosition_Left;
	mGameMode = GameMode_Main;
}

//===========================================================================

BowlerDatabase::~BowlerDatabase()
{
}

//===========================================================================

void BowlerDatabase::Load()
{
    std::string libraryPath = Database::Get()->GetLibraryPath();
    libraryPath += "game";
    System::Path::MakeDirectory( System::Path::SystemSlash( libraryPath.c_str() ) );
    libraryPath += "\\bowler";
    System::Path::MakeDirectory( System::Path::SystemSlash( libraryPath.c_str() ) );

	LoadDatabase( gDatabaseReadOnlyFilename, &( BowlerDatabase::LoadReadOnlyDatabase ) );
	LoadDatabase( gDatabaseUserFilename, &( BowlerDatabase::LoadUserDatabase ) );
}

//===========================================================================

void BowlerDatabase::SaveUserData()
{
	char buffer[256];
	System::StringCopy( buffer, 256, gDatabaseUserFilename );
	System::FixedString<256> loadPath = System::Path::SystemSlash( buffer );
	char filenameFullPath[256];
	System::StringCopy( filenameFullPath, 256, "" );
	const char* resourcePath = Database::Get()->GetLibraryPath();
	System::StringCopy( filenameFullPath, 256, resourcePath );
	System::StringConcat( filenameFullPath, 256, loadPath );

	tinyxml2::XMLDocument doc;	
	doc.LinkEndChild( doc.NewDeclaration() );  

	tinyxml2::XMLElement* root = doc.NewElement( "database" );  
	doc.LinkEndChild( root );  

	size_t numItems = mDatabaseUser.size();
	for ( int i = 0; i < numItems; ++i )
	{
		DatabaseDataUser databaseData = mDatabaseUser[i];

		tinyxml2::XMLElement* data = doc.NewElement( "data" );  
		root->LinkEndChild( data );  

		int level = i + 1;
		data->SetAttribute( "level", level );
		data->SetAttribute( "collected", databaseData.mNumCollected );
		data->SetAttribute( "time_best", databaseData.mBestTime );
		data->SetAttribute( "timeattack_best", databaseData.mBestTimeAttack );

		const char* isCompleted = "false";
		if ( databaseData.mCompleted == true )
		{
			isCompleted = "true";
		}
		data->SetAttribute( "completed", isCompleted );
	}

	tinyxml2::XMLElement* control = doc.NewElement( "control_position" );
	root->LinkEndChild( control );
	int value = static_cast<int>( mControlPosition );
	control->SetAttribute( "value", value );

	doc.SaveFile( filenameFullPath );
}

//===========================================================================

DatabaseDataUser BowlerDatabase::GetUserData( int level )
{
	Assert( level > 0, "" );

	int index = level - 1;

	int databaseSize = static_cast<int>( mDatabaseUser.size() );
	if ( index > databaseSize )
	{
		return mDatabaseUser[0];
	}

	return mDatabaseUser[index];
}

//===========================================================================

DatabaseDataReadOnly BowlerDatabase::GetReadOnlyData( int level )
{
	Assert( level > 0, "" );

	int index = level - 1;

	int databaseSize = static_cast<int>( mDatabaseReadOnly.size() );
	if ( index > databaseSize )
	{
		return mDatabaseReadOnly[0];
	}

	return mDatabaseReadOnly[index];
}

//===========================================================================

void BowlerDatabase::SetUserData( int level, const DatabaseDataUser& data )
{
	Assert( level > 0, "" );

	int index = level - 1;
	mDatabaseUser[index] = data;
}

//===========================================================================

void BowlerDatabase::LoadDatabase( const char* filename, void (*Loader)( tinyxml2::XMLNode* node, BowlerDatabase* database ) )
{
	char buffer[256];

	System::StringCopy( buffer, 256, filename );	
	System::FixedString<256> loadPath = System::Path::SystemSlash( buffer );

	char filenameFullPath[256];

	System::StringCopy( filenameFullPath, 256, "" );    
	const char* libraryPath = Database::Get()->GetLibraryPath();
	System::StringCopy( filenameFullPath, 256, libraryPath );
	System::StringConcat( filenameFullPath, 256, loadPath );
	
	FILE* file = System::OpenFile( filenameFullPath, "rb" );
	tinyxml2::XMLDocument doc;	
	if ( file == NULL )
	{
		System::StringCopy( filenameFullPath, 256, "" );        
		const char* resourcePath = Database::Get()->GetResourcePath();
		System::StringCopy( filenameFullPath, 256, resourcePath );
		System::StringConcat( filenameFullPath, 256, loadPath );
		file = System::OpenFile( filenameFullPath, "rb" );		
	}

	tinyxml2::XMLError loadOkay = doc.LoadFile( file );

    if ( file )
    {
        fclose( file );
    }

	Assert( loadOkay == tinyxml2::XML_SUCCESS, "" );
	if ( loadOkay == tinyxml2::XML_SUCCESS )
	{
		tinyxml2::XMLNode* node = &doc;

		for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
		{
			tinyxml2::XMLElement* element = currentNode->ToElement();
			if ( element )
			{	
				const char* elementValue = element->Value();
				if ( strcmp( elementValue, "database" ) == 0 )
				{
					Loader( currentNode, this );
				}
			}
		}
	}	
}

//===========================================================================

void BowlerDatabase::LoadReadOnlyDatabase( tinyxml2::XMLNode* node, BowlerDatabase* database )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
		tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "data" ) == 0 )
			{	
				int level = element->IntAttribute( "level" );

                double attackTime = element->DoubleAttribute( "attacktime" );

				int numToUnlock = element->IntAttribute( "numToUnlock" );

				const char* bgmFileName = element->Attribute( "bgm" );

				DatabaseDataReadOnly data;
                data.mTimeAttackTime = static_cast<float>( attackTime );
				data.mNumToUnlock = numToUnlock;

				if (bgmFileName != NULL)
				{
					data.mBgmFileName = bgmFileName;
				}

				int index = level - 1;
				if ( index < MAX_LEVELS )
				{
					database->mDatabaseReadOnly[index] = data;
				}
			}			
		}
	}
}

//===========================================================================

void BowlerDatabase::LoadUserDatabase( tinyxml2::XMLNode* node, BowlerDatabase* database )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
		tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "data" ) == 0 )
			{	
				int level = element->IntAttribute( "level" );

				int collected = element->IntAttribute( "collected" );

				double time = element->DoubleAttribute( "time_best" );

				bool isCompleted = false;
				const char* completedText = element->Attribute( "completed" );
				if ( strcmp( completedText, "true" ) == 0 )
				{
					isCompleted = true;
				}

				DatabaseDataUser data;
				data.mNumCollected = collected;
				data.mBestTime = static_cast<float>( time );
				data.mCompleted = isCompleted;

				int index = level - 1;
				if ( index < MAX_LEVELS )
				{
					database->mDatabaseUser[index] = data;
				}
			}
			else if ( strcmp( elementValue, "control_position" ) == 0 )
			{
				int value = element->IntAttribute( "value" );
				database->mControlPosition = static_cast<ControlPosition>( value );
			}
		}
	}
}

//===========================================================================

void BowlerDatabase::ResetData()
{
	size_t databaseSize = mDatabaseUser.size();
	for ( int i = 0; i < databaseSize; ++i )
	{
		mDatabaseUser[i].mCompleted = false;
		mDatabaseUser[i].mNumCollected = 0;
		mDatabaseUser[i].mBestTime = 0;
	}

	SaveUserData();
}

//===========================================================================

MusicType BowlerDatabase::GetMusicType( int level )
{
	Assert( level >= 0 && level < MUSIC_LEVEL_TYPE_MAX, "" );

	MusicType musicType = gMusicLevelType[level];

	return musicType;
}

//===========================================================================

void BowlerDatabase::SetControlPosition( ControlPosition position )
{
	mControlPosition = position;
}

//===========================================================================

ControlPosition BowlerDatabase::GetControlPosition()
{
	return mControlPosition;
}

//===========================================================================

void BowlerDatabase::SetGameMode( GameMode gameMode )
{
	mGameMode = gameMode;
}

//===========================================================================

GameMode BowlerDatabase::GetGameMode()
{
	return mGameMode;
}
