#ifdef PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include "ios/audio/format/iosformatinterface.hpp"


//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	iOSFormatInterface::iOSFormatInterface()
	{	
		Clear();
	}

	//===========================================================================

	iOSFormatInterface::~iOSFormatInterface()
	{
	}
	
	//===========================================================================

	
	void iOSFormatInterface::Clear()
	{
		mChannels = 0;
		mSamplesPerSecond = 0;
		mDataSize = 0;
	}
	
	//===========================================================================

	//
	// This function returns by reference an open file reference (ExtAudioFileRef). You are responsbile for releasing this
	// this reference yourself when you are finished with it by calling ExtAudioFileDispose().
	// 
	ExtAudioFileRef iOSFormatInterface::GetExtAudioFileRef( const char* filename, AudioStreamBasicDescription &audio_description )
	{
		OSStatus error_status = noErr;
		ExtAudioFileRef	ext_file_ref = NULL;
		
		// Open a file with ExtAudioFileOpen()
		CFStringRef fileStringRef = CFStringCreateWithCString( kCFAllocatorDefault, filename, NULL );
		CFStringRef s = CFURLCreateStringByAddingPercentEscapes( kCFAllocatorDefault, fileStringRef, NULL, NULL, kCFStringEncodingUTF8 );
		CFRelease( fileStringRef );
		CFURLRef file_url = CFURLCreateWithString( kCFAllocatorDefault, s, NULL );
		CFRelease( s );

		error_status = ExtAudioFileOpenURL( file_url, &ext_file_ref );
		CFRelease( file_url );

		if ( noErr != error_status )
		{
			printf( "GetExtAudioFileRef: ExtAudioFileOpenURL failed, Error = %d\n", (int)error_status );
			if ( NULL != ext_file_ref )
			{
				ExtAudioFileDispose( ext_file_ref );
			}
			return NULL;
		}
		
		// Get the audio data format
		AudioStreamBasicDescription	file_format;
		UInt32 property_size = sizeof( file_format );		
		error_status = ExtAudioFileGetProperty( ext_file_ref, kExtAudioFileProperty_FileDataFormat, &property_size, &file_format );
		if ( noErr != error_status )
		{
			printf( "GetExtAudioFileRef: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) failed, Error = %d\n", (int)error_status );
			ExtAudioFileDispose(ext_file_ref);
			return NULL;
		}
		
		//
		// Don't know how to handle sounds with more than 2 channels (i.e. stereo)
		// Remember that OpenAL sound effects must be mono to be spatialized anyway.
		//
		if ( file_format.mChannelsPerFrame > 2 ) 
		{
			printf( "GetExtAudioFileRef: Unsupported Format, channel count (=%d) is greater than stereo\n", (int)file_format.mChannelsPerFrame ); 
			ExtAudioFileDispose( ext_file_ref );
			return NULL;
		}
		
		//
		// The output format must be linear PCM because that's the only type OpenAL knows how to deal with.
		// Set the client format to 16 bit signed integer (native-endian) data because that is the most
		// optimal format on iPhone/iPod Touch hardware.
		// Maintain the channel count and sample rate of the original source format.
		//
		AudioStreamBasicDescription output_format;
		output_format.mSampleRate = file_format.mSampleRate; // preserve the original sample rate
		output_format.mChannelsPerFrame = file_format.mChannelsPerFrame; // preserve the number of channels
		output_format.mFormatID = kAudioFormatLinearPCM; // We want linear PCM data
		output_format.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
		output_format.mFramesPerPacket = 1; // We know for linear PCM, the definition is 1 frame per packet
		output_format.mBitsPerChannel = 16; // We know we want 16-bit
		output_format.mBytesPerPacket = 2 * output_format.mChannelsPerFrame; // We know we are using 16-bit, so 2-bytes per channel per frame
		output_format.mBytesPerFrame = 2 * output_format.mChannelsPerFrame; // For PCM, since 1 frame is 1 packet, it is the same as mBytesPerPacket
		
		mChannels = file_format.mChannelsPerFrame;
		mSamplesPerSecond = file_format.mSampleRate;
		
		//
		// Set the desired client (output) data format
		//		
		error_status = ExtAudioFileSetProperty(ext_file_ref, kExtAudioFileProperty_ClientDataFormat, sizeof(output_format), &output_format);
		if ( noErr != error_status )
		{
			printf( "GetExtAudioFileRef: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) failed, Error = %d\n", (int)error_status );
			ExtAudioFileDispose( ext_file_ref );
			return NULL;
		}	
		
		//
		// Copy the output format to the audio_description that was passed in so the 
		// info will be returned to the user.
		// 
		audio_description = output_format;
		return ext_file_ref;
	}
	
	//===========================================================================
	
	OSStatus iOSFormatInterface::GetDataFromExtAudioRef(
														   ExtAudioFileRef ext_file_ref, 
														   const AudioStreamBasicDescription &output_format, 
														   int max_buffer_size, 
														   char** data_buffer, 
														   int &dataSize
														   )
	{
		OSStatus error_status = noErr;	
		SInt64 buffer_size_in_frames = 0;
		
		// Compute how many frames will fit into our max buffer size
		buffer_size_in_frames = max_buffer_size / output_format.mBytesPerFrame;
		
		if ( *data_buffer )
		{
			AudioBufferList audio_buffer_list;
			audio_buffer_list.mNumberBuffers = 1;
			audio_buffer_list.mBuffers[0].mDataByteSize = max_buffer_size;
			audio_buffer_list.mBuffers[0].mNumberChannels = output_format.mChannelsPerFrame;
			audio_buffer_list.mBuffers[0].mData = *data_buffer;
			
			// Read the data into an AudioBufferList
			error_status = ExtAudioFileRead( ext_file_ref, (UInt32*)&buffer_size_in_frames, &audio_buffer_list );
			if ( error_status == noErr )
			{
				// Success
				// Note: 0 == buffer_size_in_frames is a legitimate value meaning we are EOF.
				
				// ExtAudioFileRead returns the number of frames actually read. Need to convert back to bytes.
				dataSize = (int)(buffer_size_in_frames * output_format.mBytesPerFrame);
			}
			else 
			{
				printf( "GetDataFromExtAudioRef: ExtAudioFileRead failed, Error = %d\n", (int)error_status );
			}	
		}
		return error_status;
	}
	
	//===========================================================================
	
	//
	// Returns a pointer to a buffer containing all the pcm data. This memory was allocated in this function by using malloc.
	// You are responsible for freeing this memory yourself.
	// 
	char* iOSFormatInterface::LoadAllData( const char* filename )
	{
		OSStatus error_status = noErr;	
		AudioStreamBasicDescription	output_format;
		int max_buffer_size;
		char* pcm_data;
		ExtAudioFileRef ext_file_ref = GetExtAudioFileRef( filename, output_format);
		if ( NULL == ext_file_ref )
		{
			return NULL;
		}
		
		// Get the total frame count
		UInt32 property_size;
		SInt64 file_length_in_frames = 0;		
		property_size = sizeof(file_length_in_frames);
		error_status = ExtAudioFileGetProperty(ext_file_ref, kExtAudioFileProperty_FileLengthFrames, &property_size, &file_length_in_frames);
		if ( noErr != error_status )
		{
			printf( "GetOpenALAudioDataAll: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) failed, Error = %d\n", (int)error_status );
			ExtAudioFileDispose( ext_file_ref );
			return NULL;
		}
		
		// Compute the number of bytes needed to hold all the data in the file.
		max_buffer_size = (int)file_length_in_frames * output_format.mBytesPerFrame;
		
		// Allocate memory to hold all the decoded PCM data.
		pcm_data = NEW_PTR( "PCM Data" ) char[max_buffer_size];
		if ( NULL == pcm_data )
		{
			printf( "GetOpenALAudioDataAll: memory allocation failed\n" );
			ExtAudioFileDispose( ext_file_ref );
			return NULL;
		}
		error_status = GetDataFromExtAudioRef( ext_file_ref, output_format, max_buffer_size, &pcm_data, mDataSize );
		
		Assert( max_buffer_size == mDataSize, "" );
		if ( noErr != error_status )
		{
			free(pcm_data);
			ExtAudioFileDispose(ext_file_ref);
			return NULL;
		}
		
		// Don't need file ref any more.
		ExtAudioFileDispose(ext_file_ref);
		
		return pcm_data;
	}
	
	//===========================================================================
	
	void iOSFormatInterface::GetRewindExtAudioData(ExtAudioFileRef ext_file_ref)
	{
		OSStatus error_status = noErr;
		error_status = ExtAudioFileSeek(ext_file_ref, 0);
		if ( error_status != noErr )
		{
			printf("MyRewindExtAudioData: ExtAudioFileSeek FAILED, Error = %d\n", (int)error_status);
	}
}
}

#endif