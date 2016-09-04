//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiostaticsource.hpp"
#include "common/engine/audio/format/iaudiostaticformat.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
	AudioStaticSource::AudioStaticSource()		
	{
        mVolume = 1.0f;
		mWasStopped = false;
	}

	//===========================================================================

	AudioStaticSource::~AudioStaticSource()
	{	
	}

    //===========================================================================

    const char* AudioStaticSource::GetFileName()
    {
        return mFileName;
    }

	//===========================================================================

	void AudioStaticSource::Play()
	{
		mWasStopped = false;
	}

	//===========================================================================

	void AudioStaticSource::Pause()
	{
	}

	//===========================================================================

	void AudioStaticSource::Stop()
	{
		mWasStopped = true;
	}

	//===========================================================================

    bool AudioStaticSource::IsPaused()
	{
		return (GetState() == AUDIOSTATE_PAUSED);
	}

	//===========================================================================

	bool AudioStaticSource::IsPlaying()
	{
		return (GetState() == AUDIOSTATE_PLAYING);
	}

	//===========================================================================

	bool AudioStaticSource::IsFinished()
	{
		return (GetState() == AUDIOSTATE_STOPPED) && !mWasStopped;
	}
}