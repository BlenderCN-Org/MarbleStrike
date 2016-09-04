#ifndef AUDIO_STATIC_SOURCE_HPP
#define AUDIO_STATIC_SOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudioformat.hpp"
#include "common/engine/system/fixedstring.hpp"

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
	class AudioStaticSource
	{
	public:	

		AudioStaticSource();
		virtual ~AudioStaticSource();

		virtual void Load( const char* filename ) = 0;
        const char* GetFileName();
        
		virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;

		bool IsPaused();
		bool IsPlaying();
		bool IsStopped();
		bool IsFinished();

		virtual AudioState GetState() = 0;        
		virtual void SetVolume( float volume ) = 0;		

	protected:	

		float mVolume;		
		bool mWasStopped;
        System::FixedString<256> mFileName;
	};
}

#endif