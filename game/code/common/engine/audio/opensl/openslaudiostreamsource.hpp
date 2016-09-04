#ifndef OPENSL_AUDIO_STREAM_SOURCE_HPP
#define OPENSL_AUDIO_STREAM_SOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined AUDIO_OPENSL

#include "common/engine/audio/openal/openalwrapper.hpp"
#include "common/engine/audio/audiostreamsource.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/audio/format/iaudioformat.hpp"
#include <thread>
#include <mutex>
#include <SLES/OpenSLES.h>

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
	class OpenSLAudioStreamSource : public AudioStreamSource
	{
	public:	

		OpenSLAudioStreamSource();
		~OpenSLAudioStreamSource();

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

		void SetVolume( float volume );
		float GetVolume();

        void SetOpenSLEngineEngine( SLEngineItf openSLEngineEngine );
        void SetOutputMixObject( SLObjectItf outputMixObject );

	private:

        SLObjectItf mBGMPlayerObject; 
		SLPlayItf mBGMPlayer;
        SLSeekItf mBGMPlayerSeek;
        SLVolumeItf mVolume; 
        SLObjectItf mOutputMixObject;
        SLEngineItf mOpenSLEngineEngine;
	};
}

#endif

#endif