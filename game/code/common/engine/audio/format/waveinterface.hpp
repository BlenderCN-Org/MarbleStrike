#ifndef WAVE_INTERFACE_HPP
#define WAVE_INTERFACE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/utilities.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

namespace Audio
{
	struct WaveHeader
	{
		char mRiff[4]; //'RIFF'
		unsigned int mRiffSize;
		char mWave[4]; //'WAVE'
		char mFmt[4]; //'fmt '
		unsigned int mFmtSize;
		unsigned short mFormat;
		unsigned short mChannels;
		unsigned int mSamplesPerSec;
		unsigned int mBytesPerSec;
		unsigned short mBlockAlign;
		unsigned short mBitsPerSample;
		char mData[4]; //'data'
		unsigned int mDataSize;

		void EndianSwap()
		{
			System::EndianSwap( mRiffSize );
			System::EndianSwap( mFmtSize );
			System::EndianSwap( mFormat );
			System::EndianSwap( mChannels );
			System::EndianSwap( mSamplesPerSec );
			System::EndianSwap( mBytesPerSec );
			System::EndianSwap( mBlockAlign );
			System::EndianSwap( mBitsPerSample );
			System::EndianSwap( mDataSize );
		}
	};
}

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class WaveInterface
	{
	public:	

		WaveInterface();
		virtual ~WaveInterface();

		FILE* Load( const char* filename );

		int GetChannels()
		{
			return static_cast<int>( mWaveHeader.mChannels );
		}

		int GetSamplesPerSecond()
		{
			return static_cast<int>( mWaveHeader.mSamplesPerSec );
		}

		int GetBitsPerSample()
		{
			return static_cast<int>( mWaveHeader.mBitsPerSample );
		}

		int GetDataSize()
		{
			return static_cast<int>( mWaveHeader.mDataSize );
		}

		int GetBlockAlignment()
		{
			return static_cast<int>( mWaveHeader.mBlockAlign );
		}

		int GetBytesPerSecond()
		{
			return static_cast<int>( mWaveHeader.mBytesPerSec );
		}

	private:

		WaveHeader mWaveHeader;	
	};
}

#endif
