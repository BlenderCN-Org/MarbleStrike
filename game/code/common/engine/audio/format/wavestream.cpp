//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/wavestream.hpp"
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
	WaveStream::WaveStream()
		:	mStreamFile( NULL )
		,	mEndOfFile( true )
		,	mBufferSize( 0 )
	{	
	}

	//===========================================================================

	WaveStream::~WaveStream()
	{	
		if ( mStreamFile )
		{
			fclose( mStreamFile );
			mStreamFile = NULL;
		}
	}

	//===========================================================================

	void WaveStream::Load( const char* filename )
	{
		mStreamFile = mWaveInterface.Load( filename );
		mEndOfFile = false;

		mBufferSize = mWaveInterface.GetBytesPerSecond();			
		mBufferSize -= ( mBufferSize % mWaveInterface.GetBlockAlignment() );
		mBufferSize = 1024 * 128;
	}

	//===========================================================================

	void WaveStream::CloseStream()
	{
		if ( mStreamFile )
		{
			fclose( mStreamFile );
			mStreamFile = NULL;
		}	
	}

	//===========================================================================

	int WaveStream::ReadStream( char* data, int maxSize )
	{
		size_t numRead = fread( data, sizeof( char ), maxSize, mStreamFile );
		if ( numRead < maxSize )
		{
			mEndOfFile = true;
		}
		return (int)numRead;
	}
}

