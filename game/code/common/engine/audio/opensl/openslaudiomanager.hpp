#if defined AUDIO_OPENSL

#ifndef OPENSL_AUDIO_MANAGER_HPP
#define OPENSL_AUDIO_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//#include "common/engine/audio/audiomusiclist.hpp"
//#include "common/engine/audio/audiosoundbank.hpp"
#include "common/engine/audio/audiomanager.hpp"
//#include "common/engine/system/fixedstring.hpp"
//#include <vector>
#include <SLES/OpenSLES.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//namespace Audio
//{
//	class AudioStaticSource;
//	class AudioStreamSource;
//}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	class OpenSLAudioManager : public AudioManager
	{
	public:	

		OpenSLAudioManager();
		virtual ~OpenSLAudioManager();

		void Initialize();
		void Shutdown();
        void SetSoundBank( const SoundBank& soundBank );		

	private:

		void InitializeOpenSL();
		void ShutdownOpenSL();
		void InitializeSoundPlayer();

        SLObjectItf mOpenSLEngineObject;
        SLEngineItf mOpenSLEngineEngine;
		SLObjectItf mOutputMixObject;		
	};
}

#endif

#endif