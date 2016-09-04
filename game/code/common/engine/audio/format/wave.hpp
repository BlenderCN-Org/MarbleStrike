#ifndef WAVE_HPP
#define WAVE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudiostaticformat.hpp"
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
	class Wave : public IAudioStaticFormat
	{
	public:	

		Wave();
		virtual ~Wave();

		void Load( const char* filename );

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

		int GetDataSize()
		{
			return mWaveInterface.GetDataSize();
		}

		char* GetData()
		{
			return mWaveData;
		}

		WaveInterface* GetInterface()
		{
			return &mWaveInterface;
		}

	private:

		void EndianSwap();

		WaveInterface mWaveInterface;

		char* mWaveData;	
	};
}

#endif