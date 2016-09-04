#ifndef IOS_FORMAT_HPP
#define IOS_FORMAT_HPP

#ifdef PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudiostaticformat.hpp"
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
	class iOSFormat : public IAudioStaticFormat
	{
	public:	

		iOSFormat();

		virtual ~iOSFormat();

		void Load( const char* filename );

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

		int GetDataSize()
		{
			return miOSFormatInterface.GetDataSize();
		}

		char* GetData()
		{
			return mData;
		}

	private:

		char* mData;	
		iOSFormatInterface miOSFormatInterface;
	};
}

#endif

#endif