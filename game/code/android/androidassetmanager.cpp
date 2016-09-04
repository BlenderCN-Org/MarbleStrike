//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "android/androidassetmanager.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

AndroidAssetManager* AndroidAssetManager::mInstance = NULL;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

void AndroidAssetManager::Create()
{
    Assert( mInstance == NULL, "" );
    mInstance = NEW_PTR( "AndroidAssetManager" ) AndroidAssetManager;
}

//===========================================================================

void AndroidAssetManager::Destroy()
{
    Assert( mInstance != NULL, "" );
    DELETE_PTR( mInstance );
}

//===========================================================================

AndroidAssetManager* AndroidAssetManager::GetInstance()
{
    Assert( mInstance != NULL, "" );
    return mInstance;
}

//===========================================================================

AndroidAssetManager::AndroidAssetManager()
{
    mAndroidApp = NULL;
}

//===========================================================================

AndroidAssetManager::~AndroidAssetManager()
{
}

//===========================================================================

FILE* AndroidAssetManager::OpenFile( const char* filename, const char* mode )
{
    std::string newPath = mAndroidApp->activity->internalDataPath;
    newPath += "/";
    std::string newFilename = filename;

	FILE* newFile = fopen( filename, "rb" );
	if ( newFile != NULL )
	{
		return newFile;
	}

    size_t findIndex = newFilename.find( newPath );
    if ( findIndex == std::string::npos )
    {
    	return NULL;
    }
    newFilename.replace( findIndex, newPath.size(), "" );

    AAssetManager* assetManager = mAndroidApp->activity->assetManager;  
    AAsset* asset = AAssetManager_open( assetManager, newFilename.c_str(), AASSET_MODE_UNKNOWN );    
    if ( asset == NULL )
    {
    	return NULL;
    }

    char* assetBuffer = (char*)AAsset_getBuffer( asset );
    off_t dataLength = AAsset_getLength( asset );    

    std::string path;
    std::vector<std::string> tokens;
    std::string currentFilename = filename;
	System::Tokenize( tokens, (char*)filename, "/" );
    int count = 0;
    for ( int i = 0; i < tokens.size() - 1; ++i )
    {
    	const char* currentToken = tokens[i].c_str();
    	path += currentToken;
    	path += "/";
    	mkdir( path.c_str(), 0770 );
    }

    newFile = fopen( currentFilename.c_str(), "w+" );

    if ( newFile == NULL )
    {
    	return NULL;
    }

    fwrite( assetBuffer, sizeof( char ), dataLength, newFile );
    fseek( newFile, 0, SEEK_SET );
    
    return newFile;
}
