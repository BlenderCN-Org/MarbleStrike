#ifndef OPENAL_AUDIO_STATIC_SOURCE_HPP
#define OPENAL_AUDIO_STATIC_SOURCE_HPP

#if defined PLAT_PC || defined PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedstring.hpp"
#include "common/engine/audio/audiostaticsource.hpp"
#include "common/engine/audio/openal/openalwrapper.hpp"

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
	class OpenALAudioStaticSource : public AudioStaticSource
	{
	public:	

		OpenALAudioStaticSource();
		~OpenALAudioStaticSource();

		void Load( const char* filename );

		void Play();
		void Pause();
		void Stop();

		ALsizei GetSamplesPerSecond();
        AudioState GetState();
        void SetVolume( float volume );

	private:

		ALuint mBuffer;
		ALuint mSource;
		ALsizei mSamplePerSecond;
	};

}

#endif

#endif