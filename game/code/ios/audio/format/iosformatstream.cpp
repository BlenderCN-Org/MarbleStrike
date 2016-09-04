#ifdef PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include "ios/audio/format/iosformatstream.hpp"


//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	iOSFormatStream::iOSFormatStream()
		:	mEndOfFile( false )
	{	
	}

	//===========================================================================

	iOSFormatStream::~iOSFormatStream()
	{	
		mEndOfFile = false;
		ExtAudioFileDispose( mExtAudioFileRef );
	}

	//===========================================================================

	void iOSFormatStream::Load( const char* filename )
	{
		char fullPathFilename[256];
		System::StringCopy( fullPathFilename, 256, "" );
		System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
		System::StringConcat( fullPathFilename, 256, filename );
		System::StringConcat( fullPathFilename, 256, ".mp4" );
		
		mExtAudioFileRef = miOSFormatInterface.GetExtAudioFileRef( System::Path::SystemSlash( fullPathFilename ), mAudioStreamBasicDescription );
	}

	//===========================================================================

	void iOSFormatStream::CloseStream()
	{
		mEndOfFile = false;
	}

	//===========================================================================

	int iOSFormatStream::ReadStream( char* data, int maxSize )
	{	
		int bufferSize = 0;
		miOSFormatInterface.GetDataFromExtAudioRef(
													  mExtAudioFileRef, 
													  mAudioStreamBasicDescription, 
													  maxSize, 
													  &data, 
													  bufferSize
													  ); 
		if ( 0 == bufferSize ) // will loop music on EOF (which is 0 bytes) 
		{
			miOSFormatInterface.GetRewindExtAudioData( mExtAudioFileRef ); 
			mEndOfFile = true;
		}
		
		return bufferSize;
	}
}

#endif