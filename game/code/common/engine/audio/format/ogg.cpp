#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 ) || defined( PLAT_ANDROID )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/ogg.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include <vector>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	void Ogg::Load( const char* filename )
	{
		OggVorbis_File* oggVorbisFile = mOggInterface.Load( filename );

		int endian = 0;
		int bitStream = 0;
		int bytes = 0;
		const int BUFFER_SIZE = 32768;
		char array[BUFFER_SIZE];

		do 
		{	
			bytes = ov_read( oggVorbisFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream );	
			mBufferData.insert( mBufferData.end(), array, array + bytes );
		} 
		while ( bytes > 0 );	
	}
}

#endif