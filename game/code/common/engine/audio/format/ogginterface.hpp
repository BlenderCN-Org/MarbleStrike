#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 ) || defined( PLAT_ANDROID )

#ifndef OGG_INTERFACE_HPP
#define OGG_INTERFACE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "vorbis/vorbisfile.h"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class OggInterface
	{
	public:	

		OggInterface();
		virtual ~OggInterface();

		void Clear();
		OggVorbis_File* Load( const char* filename );

		int GetChannels()
		{
			return static_cast<int>( mVorbisInfo->channels );
		}

		int GetSamplesPerSecond()
		{
			return static_cast<int>( mVorbisInfo->rate );
		}

		int GetBitsPerSample()
		{
			return 16;
		}

	private:

		FILE* LoadHeader( const char* filename );

		FILE* mFile;
		OggVorbis_File* mOggVorbisFile;
		vorbis_info* mVorbisInfo;

	};
}

#endif

#endif