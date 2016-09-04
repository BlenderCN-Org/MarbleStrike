#ifndef OPENAL_AUDIO_STREAM_SOURCE_HPP
#define OPENAL_AUDIO_STREAM_SOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC || defined PLAT_IOS

#include "common/engine/audio/openal/openalwrapper.hpp"
#include "common/engine/audio/audiostreamsource.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/audio/format/iaudioformat.hpp"
#include <thread>
#include <mutex>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
	class IAudioStreamFormat;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class OpenALAudioStreamSource : public AudioStreamSource
	{
	public:	

		OpenALAudioStreamSource();
		~OpenALAudioStreamSource();

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

		void SetVolume(float vol);
		float GetVolume();

	private:

        void EmptyQueue();
        static void AudioUpdateThread( OpenALAudioStreamSource* openALAudioStreamSource );

		bool Stream( ALuint buffer );		
		ALuint mMusicBuffers[2];
		ALuint mMusicSource;
		
		bool mShouldExitUpdateLoop;
        std::thread mAudioThread;
		std::mutex mAudioMutex;
	};
}

#endif

#endif