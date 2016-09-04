#ifndef IOS_FORMAT_INTERFACE_HPP
#define IOS_FORMAT_INTERFACE_HPP

#ifdef PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#import <AudioToolbox/AudioToolbox.h>
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
	class iOSFormatInterface
	{
	public:	
		
		iOSFormatInterface();
		virtual ~iOSFormatInterface();
		
		void Clear();
		
		int GetChannels()
		{
			return mChannels;
		}
		
		int GetSamplesPerSecond()
		{
			return mSamplesPerSecond;
		}
		
		int GetBitsPerSample()
		{
			return 16;
		}
		
		int GetDataSize()
		{
			return mDataSize;
		}
		
		ExtAudioFileRef GetExtAudioFileRef( 
										   const char* filename, 
										   AudioStreamBasicDescription &audio_description
										   );
		OSStatus GetDataFromExtAudioRef(
										ExtAudioFileRef ext_file_ref, 
										const AudioStreamBasicDescription &output_format, 
										int max_buffer_size, 
										char** data_buffer, 
										int &data_buffer_size 
										);
		char* LoadAllData( const char* filename );
		void GetRewindExtAudioData(ExtAudioFileRef ext_file_ref);		
		
	private:
		
		int mChannels;
		int mSamplesPerSecond;	
		int mDataSize;
		
	};
}

#endif

#endif