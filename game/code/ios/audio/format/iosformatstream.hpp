#ifndef IOS_FORMAT_STREAM_HPP
#define IOS_FORMAT_STREAM_HPP

#ifdef PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudiostreamformat.hpp"
#include "ios/audio/format/iosformatinterface.hpp"
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
	class iOSFormatStream : public IAudioStreamFormat
	{
	public:	

		iOSFormatStream();

		virtual ~iOSFormatStream();

		void Load( const char* filename );
		void CloseStream();
		int ReadStream( char* data, int maxSize );	

		int GetChannels()
		{
			return miOSFormatInterface.GetChannels();
		}

		int GetSamplesPerSecond()
		{
			return miOSFormatInterface.GetSamplesPerSecond();
		}

		int GetBitsPerSample()
		{
			return miOSFormatInterface.GetBitsPerSample();
		}

		int GetBufferSize()
		{			
			return 100 * 1024;
		}

		bool IsEOF()
		{
			return mEndOfFile;
		}

	private:

		iOSFormatInterface miOSFormatInterface;
		bool mEndOfFile;
		ExtAudioFileRef mExtAudioFileRef;
		AudioStreamBasicDescription mAudioStreamBasicDescription;
	};
}

#endif

#endif