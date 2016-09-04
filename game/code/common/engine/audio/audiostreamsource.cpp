//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/audio/audiostreamsource.hpp"
#include "common/engine/audio/format/iaudioformat.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////
namespace Audio
{
	AudioStreamSource::AudioStreamSource()		
		:	mData( NULL )
		,   mDataSize( 0 )
		,	mAudioStreamFormat( NULL )
        ,   mAudioState( AUDIOSTATE_STOPPED )
		,	mVolume( 0 )
	{
	}

	//===========================================================================

	AudioStreamSource::~AudioStreamSource()
	{
	}

	//===========================================================================

	void AudioStreamSource::Unload()
	{
        mAudioState = AUDIOSTATE_STOPPED;
	}

	//===========================================================================

	void AudioStreamSource::Play()
	{
        mAudioState = AUDIOSTATE_PLAYING;
	}

	//===========================================================================

	void AudioStreamSource::Stop()
	{
        mAudioState = AUDIOSTATE_STOPPED;
	}

	//===========================================================================

	void AudioStreamSource::Pause()
	{
        mAudioState = AUDIOSTATE_PAUSED;
	}

    //===========================================================================

	void AudioStreamSource::Unpause()
	{
        mAudioState = AUDIOSTATE_PLAYING;
	}

	//===========================================================================

	void AudioStreamSource::Rewind()
	{
	}

	//===========================================================================

	bool AudioStreamSource::IsPaused()
	{
		return ( mAudioState == AUDIOSTATE_PAUSED );
	}

	//===========================================================================

	bool AudioStreamSource::IsPlaying()
	{
		return ( mAudioState == AUDIOSTATE_PLAYING );
	}

	//===========================================================================

	bool AudioStreamSource::IsFinished()
	{
        return false;
	}

	//===========================================================================

	void* AudioStreamSource::ThreadUpdate( void* /*userdata*/ )
	{
		return NULL;
	}

	//===========================================================================

	void AudioStreamSource::Update()
	{
	}

	//===========================================================================

	void AudioStreamSource::EmptyQueue()
	{
	}

	//===========================================================================

	void AudioStreamSource::SetVolume( float volume )
	{
        mVolume = volume;
	}

	//===========================================================================

	float AudioStreamSource::GetVolume()
	{
		return mVolume;
	}
}