#if defined AUDIO_OPENAL

#ifndef OPENAL_AUDIO_MANAGER_HPP
#define OPENAL_AUDIO_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomusiclist.hpp"
#include "common/engine/audio/audiosoundbank.hpp"
#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/system/fixedstring.hpp"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

namespace Audio
{
	class AudioStaticSource;
	class AudioStreamSource;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class OpenALAudioManager : public AudioManager
	{
	public:	

		OpenALAudioManager();
		virtual ~OpenALAudioManager();

		void Initialize();
		void Shutdown();
        void SetSoundBank( const SoundBank& soundBank );		

	private:

		void InitializeOpenAL();
		void ShutdownOpenAL();

        int mMixerOutputRate;
	};
}

#endif

#endif