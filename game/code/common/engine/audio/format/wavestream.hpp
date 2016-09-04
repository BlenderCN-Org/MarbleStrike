#ifndef WAVE_STREAM_HPP
#define WAVE_STREAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudiostreamformat.hpp"
#include "common/engine/audio/format/waveinterface.hpp"

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

	class WaveStream : public IAudioStreamFormat
	{
	public:	

		WaveStream();
		virtual ~WaveStream();

		void Load( const char* filename );
		void CloseStream();
		int ReadStream( char* data, int maxSize );

		int GetChannels()
		{
			return mWaveInterface.GetChannels();
		}

		int GetSamplesPerSecond()
		{
			return mWaveInterface.GetSamplesPerSecond();
		}

		int GetBitsPerSample()
		{
			return mWaveInterface.GetBitsPerSample();
		}

		int GetBufferSize()
		{
			return mBufferSize;
		}

		bool IsEOF()
		{
			return mEndOfFile;
		}

	private:

		WaveInterface mWaveInterface;

		FILE* mStreamFile;
		int mBufferSize;
		bool mEndOfFile;

	};

}

#endif
