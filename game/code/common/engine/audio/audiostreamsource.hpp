#ifndef AUDIO_STREAM_SOURCE_HPP
#define AUDIO_STREAM_SOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedstring.hpp"
#include "common/engine/audio/format/iaudioformat.hpp"

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
	class AudioStreamSource
	{
	public:	

		AudioStreamSource();
		virtual ~AudioStreamSource();

		virtual void Load( const char* filename ) = 0;
		virtual void Unload();

		virtual void Play();
		virtual void Stop();
		virtual void Pause();
		virtual void Unpause();
		virtual void Rewind();

		bool IsPaused();
		bool IsPlaying();
		virtual bool IsFinished();

		virtual void Update();

		virtual void SetVolume( float volume );
		float GetVolume();

	protected:

		static void* ThreadUpdate(void* userdata);
        void EmptyQueue();

		float mVolume;
		char* mData;
		int mDataSize;
		IAudioStreamFormat* mAudioStreamFormat;
		bool mIsEndofStream;
        
        AudioState mAudioState;
	};
}

#endif