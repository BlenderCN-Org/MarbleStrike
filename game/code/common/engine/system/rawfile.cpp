//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/rawfile.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/pathutilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RawFile::RawFile()
:	mData( NULL )
,	mSize( 0 )
{
}

//===========================================================================

RawFile::~RawFile()
{
	DELETE_PTR_ARRAY( mData );
}

//===========================================================================

void RawFile::OpenAsRaw( const char* filename )
{
	char errorString[256];

	FILE* fileHandle = System::OpenFile( filename,  "rb" );	
	if ( fileHandle == NULL )
	{
		snprintf( errorString, 256, "Cannot open file: %s.\n", filename );
		Assert( false, errorString );
	}
	fseek( fileHandle, 0, SEEK_END );
	mSize = (int)ftell( fileHandle );
	fseek( fileHandle, 0, SEEK_SET ); 

	mData = NEW_PTR( "Raw File Data" ) char[mSize];
	fread( mData, sizeof( char ), mSize, fileHandle );	
	fclose( fileHandle );
	fileHandle = NULL;
}

//===========================================================================

void RawFile::OpenAsString( const char* filename )
{
	char errorString[256];

    System::FixedString<256> loadPath = System::Path::SystemSlash( filename );	
    
	FILE* fileHandle = System::OpenFile( loadPath.GetRawData(),  "rb" );	
	if ( fileHandle == NULL )
	{
		snprintf( errorString, 256, "Cannot open file: %s.\n", filename );
		Assert( false, errorString );
	}

	fseek( fileHandle, 0, SEEK_END );
	mSize = (int)ftell( fileHandle );
	fseek( fileHandle, 0, SEEK_SET ); 	

	mData = NEW_PTR( "Raw File Data" ) char[mSize + 1];
	fread( mData, sizeof( char ), mSize, fileHandle );
	mData[mSize] = '\0';
	fclose( fileHandle );
	fileHandle = NULL;	
}

//===========================================================================

void RawFile::Close()
{	
	DELETE_PTR_ARRAY( mData );
	mSize = 0;
}

//===========================================================================

const char* RawFile::GetData() const
{	
	return mData;
}

//===========================================================================

int RawFile::GetSize() const
{
	return mSize;
}