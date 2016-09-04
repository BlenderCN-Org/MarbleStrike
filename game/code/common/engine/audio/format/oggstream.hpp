#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )

#ifndef OGG_STREAM_HPP
#define OGG_STREAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudiostreamformat.hpp"
#include "common/engine/audio/format/ogginterface.hpp"
#include "vorbis/vorbisfile.h"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

struct OggVorbis_File;

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
	class OggStream : public IAudioStreamFormat
	{
	public:	
		static const int OGG_BUFFER_SIZE = 100 * 1024;

		OggStream();

		virtual ~OggStream();

		void Load( const char* filename );
		void CloseStream();
		int ReadStream( char* data, int maxSize );	

		int GetChannels()
		{
			return mOggInterface.GetChannels();
		}

		int GetSamplesPerSecond()
		{
			return mOggInterface.GetSamplesPerSecond();
		}

		int GetBitsPerSample()
		{
			return mOggInterface.GetBitsPerSample();
		}

		int GetBufferSize()
		{
			return OGG_BUFFER_SIZE;
		}

		bool IsEOF()
		{
			return mEndOfFile;
		}

	private:

		OggInterface mOggInterface;
		OggVorbis_File* mOggVorbisFile;
		bool mEndOfFile;

	};
}

#endif

#endif