#ifndef OPENSL_AUDIO_STATIC_SOURCE_HPP
#define OPENSL_AUDIO_STATIC_SOURCE_HPP

#if defined AUDIO_OPENSL

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedstring.hpp"
#include "common/engine/audio/audiostaticsource.hpp"
#include <SLES/OpenSLES.h>

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
	class OpenSLAudioStaticSource : public AudioStaticSource
	{
	public:	

		OpenSLAudioStaticSource();
		~OpenSLAudioStaticSource();

		void Load( const char* filename );

		void Play();
		void Pause();
		void Stop();

        AudioState GetState();
        void SetVolume( float volume );

		void SetOpenSLEngineEngine( SLEngineItf openSLEngineEngine );
		void SetOutputMixObject( SLObjectItf outputMixObject );

	private:

		char* mBuffer;
		int mBufferSize;
		SLObjectItf mPlayerObject;
		SLPlayItf mPlayer;
        SLBufferQueueItf mPlayerQueue;
		SLObjectItf mOutputMixObject;
		SLEngineItf mOpenSLEngineEngine;
        SLVolumeItf mVolume; 
	};

}

#endif

#endif