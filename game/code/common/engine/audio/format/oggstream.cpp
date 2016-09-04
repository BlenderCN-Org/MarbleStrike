#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/oggstream.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	OggStream::OggStream()
		:	mOggVorbisFile( NULL )
		,	mEndOfFile( false )
	{	
	}

	//===========================================================================

	OggStream::~OggStream()
	{	
		mOggVorbisFile = NULL;
		mEndOfFile = false;
	}

	//===========================================================================

	void OggStream::Load( const char* filename )
	{
		mOggVorbisFile = mOggInterface.Load( filename );
	}

	//===========================================================================

	void OggStream::CloseStream()
	{
		mOggVorbisFile = NULL;
		mEndOfFile = false;
	}

	//===========================================================================

	int OggStream::ReadStream( char* data, int maxSize )
	{
		Assert( mOggVorbisFile != NULL, "" );

		int  section;
		int numRead = ov_read( mOggVorbisFile, data, maxSize, 0, 2, 1, & section );	
		if ( numRead == 0 )
		{
			mEndOfFile = true;
		}
		return numRead;	
	}
}

#endif