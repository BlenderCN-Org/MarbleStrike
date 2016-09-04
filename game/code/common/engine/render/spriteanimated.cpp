//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/spriteanimated.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/texturemanager.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

SpriteAnimated::SpriteAnimated() 
{
	mTimeCount = 0;
	mCurrentFrame = 0;
	mNumFrames = 0;
	mTextureAtlas = NULL;
	mFramesPerSecond = 24.0f;
}

//============================================================================

SpriteAnimated::~SpriteAnimated()
{	
}

//============================================================================

void SpriteAnimated::Copy( const SpriteAnimated &other )
{
	mTextureAtlas = other.mTextureAtlas;
	mFrameTexCoords = other.mFrameTexCoords;	
	mTimeCount = other.mTimeCount;
	mCurrentFrame = other.mCurrentFrame;	
	mNumFrames = other.mNumFrames;
	mFramesPerSecond = other.mFramesPerSecond;
}

//============================================================================

void SpriteAnimated::Load( const char* filename )
{
	mTextureAtlas = GameApp::Get()->GetTextureManager()->GetTexture( filename );

	char dataFileBuffer[256];
	System::StringCopy( dataFileBuffer, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( dataFileBuffer, 256, filename );
	System::StringConcat( dataFileBuffer, 256, ".txt" );

	FILE* fileHandle = System::OpenFile( System::Path::SystemSlash( dataFileBuffer ), "r" );
	Assert( fileHandle != NULL, "" );

	char lineString[1024];
	while ( fgets( lineString, 1024, fileHandle ) != NULL )	
	{
		if ( (lineString[0] != '#' && lineString[0] != '\r' && lineString[0] != '\n') && strlen( lineString ) > 1 )
		{
            std::string myString = lineString;
			const char* delim =" ";
			
			size_t findIndex = myString.find_first_of( delim );
			std::string findString = myString.substr( 0, findIndex );
			const char* filenameToken = findString.c_str();
            myString = myString.substr( findIndex + 1 );
			UNUSED_ALWAYS( filenameToken );
						
			findIndex = myString.find_first_of( delim );
			findString = myString.substr( 0, findIndex );
            const char* equal = findString.c_str();
            myString = myString.substr( findIndex + 1 );
			UNUSED_ALWAYS( equal );
			
			findIndex = myString.find_first_of( delim );
			findString = myString.substr( 0, findIndex );
            const char* widthOffset = findString.c_str();
            myString = myString.substr( findIndex + 1 );
			float widthOffsetFloat = static_cast<float>( atof( widthOffset ) ) / static_cast<float>( mTextureAtlas->GetWidth() );

			findIndex = myString.find_first_of( delim );
			findString = myString.substr( 0, findIndex );
            const char* heightOffset = findString.c_str();
            myString = myString.substr( findIndex + 1 );
			float heightOffsetFloat = static_cast<float>( atof( heightOffset ) ) / static_cast<float>( mTextureAtlas->GetHeight() );
			
			findIndex = myString.find_first_of( delim );
			findString = myString.substr( 0, findIndex );
            const char* width = findString.c_str();
            myString = myString.substr( findIndex + 1 );
			float widthFloat = static_cast<float>( atof( width ) ) / static_cast<float>( mTextureAtlas->GetWidth() );

			findIndex = myString.find_first_of( delim );
			findString = myString.substr( 0, findIndex );
            const char* height = findString.c_str();
            myString = myString.substr( findIndex + 1 );
			float heightFloat = static_cast<float>( atof( height ) ) / static_cast<float>( mTextureAtlas->GetHeight() );

			SpriteAnimated::FrameTexCoord frameTexCoord;

			frameTexCoord.mTexCoords[0].Set( widthOffsetFloat, ( 1.0f - heightOffsetFloat ) - heightFloat );
			frameTexCoord.mTexCoords[1].Set( widthOffsetFloat + widthFloat, ( 1.0f - heightOffsetFloat ) - heightFloat  );
			frameTexCoord.mTexCoords[2].Set( widthOffsetFloat + widthFloat, 1.0f - heightOffsetFloat );
			frameTexCoord.mTexCoords[3].Set( widthOffsetFloat, 1.0f - heightOffsetFloat  );

			mFrameTexCoords.push_back( frameTexCoord );
		}
	}

	fclose( fileHandle );
	fileHandle = NULL;

	mNumFrames = (int)mFrameTexCoords.size();
}

//============================================================================

void SpriteAnimated::Update( float elapsedTime )
{
	mTimeCount += elapsedTime;
	if ( mTimeCount > ( 1.0f / mFramesPerSecond ) )
	{
		mCurrentFrame++;		
		mCurrentFrame = mCurrentFrame % mNumFrames;	
		mTimeCount = 0;
	}
}

//============================================================================

void SpriteAnimated::SetFramesPerSecond( float fps )
{
	if ( fps > 0 )
	{
		mFramesPerSecond = fps;
	}	
}

//============================================================================

float SpriteAnimated::GetFramesPerSecond()
{
	return mFramesPerSecond;
}

//============================================================================

Texture* SpriteAnimated::GetTexture()
{
	return mTextureAtlas;
}

//============================================================================

SpriteAnimated::FrameTexCoord SpriteAnimated::GetCurrentFrameTexCoord()
{
	return mFrameTexCoords[mCurrentFrame];
}