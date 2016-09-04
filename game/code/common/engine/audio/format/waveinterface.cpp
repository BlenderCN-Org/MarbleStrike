//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/waveinterface.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////
namespace Audio
{
	WaveInterface::WaveInterface()
	{
		memset( &mWaveHeader, 0, sizeof ( WaveHeader ) );
	}

	//===========================================================================

	WaveInterface::~WaveInterface()
	{
	}

	//===========================================================================

	FILE* WaveInterface::Load( const char* filename )
	{
		char fullPathFilename[256];
		System::StringCopy( fullPathFilename, 256, "" );
#if defined PLAT_IOS
		System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
#endif		
		System::StringConcat( fullPathFilename, 256, filename );

		char gamePath[256];
		System::StringCopy( gamePath, 256, "" );
#if defined PLAT_XBOX360	
		System::StringConcat( gamePath, 256, "game:\\" );	
#endif
		System::StringConcat( gamePath, 256, fullPathFilename );	
		System::StringConcat( gamePath, 256, ".wav" );

		FILE* file = NULL;
		file = System::OpenFile( gamePath, "rb" );
		Assert( file != NULL, "" );

		fread( &mWaveHeader, sizeof( WaveHeader ), 1, file );

		mWaveHeader.EndianSwap();

		if ( 
			!(
			memcmp( "RIFF", mWaveHeader.mRiff, 4 ) == 0 || 	
			memcmp( "WAVE", mWaveHeader.mWave, 4 )  == 0 || 
			memcmp( "fmt ", mWaveHeader.mFmt, 4 )  == 0 || 
			memcmp( "data", mWaveHeader.mData, 4 ) == 0 
			)
			)
		{
			Assert( false, "" );
		}

		return file;
	}
}

