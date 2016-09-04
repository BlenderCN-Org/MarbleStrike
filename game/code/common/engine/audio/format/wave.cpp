//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/wave.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	Wave::Wave()
		:	mWaveData( NULL )
	{
	}

	//===========================================================================

	Wave::~Wave()
	{	
		DELETE_PTR( mWaveData );
	}

	//===========================================================================

	void Wave::Load( const char* filename )
	{
		FILE* file = mWaveInterface.Load( filename );

		int dataSize = mWaveInterface.GetDataSize();
		mWaveData = NEW_PTR( "Wave Data" ) char[dataSize];
		fread( mWaveData, sizeof( char ), dataSize, file );

		EndianSwap();

		fclose( file );
	}

	//===========================================================================

	void Wave::EndianSwap()
	{
#if defined PLAT_XBOX360
		if ( mWaveInterface.GetBitsPerSample() == 16 )
		{
			int dataCount = mWaveInterface.GetDataSize();
			int countTest = mWaveInterface.GetDataSize() % 2;
			Assert( countTest == 0, "" );

			for ( int i = 0; i < dataCount; i += 2 )
			{
				unsigned short data = *( ( unsigned short* )&mWaveData[i] );
				System::EndianSwap( data );
				*( ( unsigned short* )&mWaveData[i] ) = data;
			}
		}
#endif
	}
}

