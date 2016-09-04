#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 ) || defined( PLAT_ANDROID )

#ifndef OGG_HPP
#define OGG_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/ogginterface.hpp"
#include "common/engine/audio/format/iaudiostaticformat.hpp"
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
	class Ogg : public IAudioStaticFormat
	{
	public:	

		Ogg() {}

		virtual ~Ogg()
		{
			mBufferData.clear();
		}

		void Load( const char* filename );

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

		int GetDataSize()
		{
			return static_cast<int>( mBufferData.size() );
		}

		char* GetData()
		{
			return &( mBufferData[0] );
		}

	private:

		std::vector < char > mBufferData;
		OggInterface mOggInterface;
	};
}

#endif

#endif