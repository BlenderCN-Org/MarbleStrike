#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8

#ifndef MEDIA_FOUNDATION_AUDIO_STREAM_SOURCE
#define MEDIA_FOUNDATION_AUDIO_STREAM_SOURCE

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <mfapi.h>
#include <mfidl.h>
#include <Mfmediaengine.h>
#include <Strsafe.h>
#include "common/engine/audio/audiostreamsource.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class MediaFoundationAudioStreamSource : public AudioStreamSource
	{
	public:	

		MediaFoundationAudioStreamSource();
		~MediaFoundationAudioStreamSource();

		void Load( const char* filename );
		void Unload();

		void Play();
		void Stop();
		void Pause();
		void Unpause();
		void Rewind();
		bool IsFinished();

		AudioState GetState();

		void Update();

		void SetVolume(float volume);
		float GetVolume();

		void SetDone( bool value );

	private:

		bool mDone;
	};
}

#endif

#endif