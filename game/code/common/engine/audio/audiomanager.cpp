//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/audiomanager.hpp"
#include "common/engine/audio/audiostreamsource.hpp"
#include "common/engine/audio/audiostaticsource.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

#if defined AUDIO_OPENAL
#include "common/engine/audio/openal/openalaudiomanager.hpp"
#endif

#if defined AUDIO_XAUDIO2
#include "common/engine/audio/xaudio2/xaudio2audiomanager.hpp"
#endif

#if defined AUDIO_OPENSL
#include "common/engine/audio/opensl/openslaudiomanager.hpp"
#endif


//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace Audio
{
	const int AUDIO_DEFAULT_SAMPLE_RATE = 48000;

    AudioManager* AudioManager::mInstance = NULL;
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
    AudioManager* AudioManager::Create()
    {
        Assert( mInstance == NULL, "" );
#if defined AUDIO_OPENAL
        mInstance = NEW_PTR( "OpenAL AudioManager" ) OpenALAudioManager;
#endif
#if defined AUDIO_XAUDIO2
        mInstance = NEW_PTR( "XAudio2 AudioManager" ) XAudio2AudioManager;
#endif
#if defined AUDIO_OPENSL
        mInstance = NEW_PTR( "OpenSL AudioManager" ) OpenSLAudioManager;
#endif
        Assert( mInstance != NULL, "" );
        
        return mInstance;
    }

    //===========================================================================

    void AudioManager::Destroy()
    {
        DELETE_PTR( mInstance );
    }

    //===========================================================================

	AudioManager::AudioManager()		
	{
        mMusicVolume = 1.0f;
		mSoundVolume = 1.0f;
        mStreamSource = NULL;
	}

	//===========================================================================

	AudioManager::~AudioManager()
	{
	}
    
	//===========================================================================

	void AudioManager::Shutdown()
	{
		std::vector<AudioStaticSource*>::iterator iter = mStaticSources.begin();
		while( iter != mStaticSources.end() )
		{	
			AudioStaticSource* source = *iter;
			DELETE_PTR( source );
			iter++;
		}
		mStaticSources.clear();

		if ( mStreamSource != NULL )
		{
			DELETE_PTR( mStreamSource );
		}
	}

	//===========================================================================

	void AudioManager::Update()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Update();

			if ( mStreamSource->IsFinished() )
			{
				mStreamSource->Unload();

				const char* filename = mMusicList.GetNextFile();
				if ( filename != NULL )
				{
					mStreamSource->Load( filename );
					mStreamSource->Play();
				}
			}
		}
	}

	//===========================================================================

	void AudioManager::SetMusicList( const MusicList& musicList )
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Unload();
		}
        
		mMusicList = musicList;

		const char* file = mMusicList.GetCurrentFile();

		if ( file != NULL )
		{
            if ( mStreamSource != NULL )
			{                
                mStreamSource->Load( file );
            }
		}
	}

	//===========================================================================

	void AudioManager::ClearMusicList()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Unload();
		}
		mMusicList.Clear();
	}

	//===========================================================================

	void AudioManager::PlayMusic()
	{
        if ( Database::Get()->GetStartupData()->mEnableAudio == false )
        {
            return;
        }

		if ( mStreamSource != NULL )
		{
			mStreamSource->Play();
		}
	}

	//===========================================================================

	bool AudioManager::PlayMusic( const char* filename )
	{
        if ( Database::Get()->GetStartupData()->mEnableAudio == false )
        {
            return false;
        }

		if ( mMusicList.SetCurrentFile( filename ) )
		{
			if ( mStreamSource != NULL )
			{
				mStreamSource->Unload();
				mStreamSource->Load( filename );
                mStreamSource->Play();

				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}

	//===========================================================================

	void AudioManager::StopMusic()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Stop();
		}
	}

	//===========================================================================

	void AudioManager::PauseMusic()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Pause();
		}
	}

    //===========================================================================

	void AudioManager::UnpauseMusic()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Unpause();
		}
	}

	//===========================================================================

	bool AudioManager::IsMusicPaused()
	{
		if ( mStreamSource != NULL )
		{
			return mStreamSource->IsPaused();
		}

		return true;
	}

	//===========================================================================

	bool AudioManager::IsMusicPlaying()
	{
		if ( mStreamSource != NULL )
		{
			return mStreamSource->IsPlaying();
		}

		return false;
	}

	//===========================================================================

	void AudioManager::NextMusic()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Unload();

			const char* nextfile = mMusicList.GetNextFile();

			if ( nextfile != NULL )
			{
				mStreamSource->Load( nextfile );
				mStreamSource->Play();
			}
		}
	}

	//===========================================================================

	void AudioManager::PrevMusic()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Unload();

			const char* nextfile = mMusicList.GetPrevFile();

			if ( nextfile != NULL )
			{
				mStreamSource->Load( nextfile );
				mStreamSource->Play();
			}
		}
	}

	//===========================================================================

	void AudioManager::RewindMusic()
	{
		if ( mStreamSource != NULL )
		{
			mStreamSource->Rewind();
		}
	}

	//===========================================================================

	void AudioManager::ClearSoundBank()
	{
		std::vector<AudioStaticSource*>::iterator iter = mStaticSources.begin();
		while( iter != mStaticSources.end() )
		{	
			AudioStaticSource* source = *iter;
			DELETE_PTR( source );
			iter++;
		}
		mStaticSources.clear();
	}

    //===========================================================================

	void AudioManager::PlayAudio( unsigned int index )
	{
        if ( Database::Get()->GetStartupData()->mEnableAudio == false )
        {
            return;
        }

        Assert( 0 <= index && index < mStaticSources.size(), "" );
        if ( mStaticSources[index] != NULL )
        {
            mStaticSources[index]->Play();
        }
	}

	//===========================================================================

	void AudioManager::PlayAudio(const char* filename)
	{
        if ( Database::Get()->GetStartupData()->mEnableAudio == false )
        {
            return;
        }

		std::vector<AudioStaticSource*>::iterator iter = mStaticSources.begin();
		while ( iter != mStaticSources.end() )
		{	
			AudioStaticSource* source = *iter;
			if ( System::StringICmp( source->GetFileName(), filename ) == 0 )
			{
				source->Play();
			}
			iter++;
		}
	}

	//===========================================================================

	void AudioManager::PauseAllAudio()
	{
		std::vector<AudioStaticSource*>::iterator iter = mStaticSources.begin();
		while( iter != mStaticSources.end() )
		{	
			AudioStaticSource* source = *iter;
			if ( source->IsPlaying() )
			{
				source->Pause();
			}
			iter++;
		}

        if ( mStreamSource )
        {
            mStreamSource->Pause();
        }
	}

	//===========================================================================

	void AudioManager::UnpauseAllAudio()
	{
		std::vector<AudioStaticSource*>::iterator iter = mStaticSources.begin();
		while( iter != mStaticSources.end() )
		{	
			AudioStaticSource* source = *iter;
			if ( source->IsPaused() )
			{
				source->Play();
			}
			iter++;
		}

        if ( mStreamSource && mStreamSource->IsPaused() )
        {
            mStreamSource->Unpause();
        }
	}

	//===========================================================================

	void AudioManager::SetMusicVolume( float volume )
	{
		mMusicVolume = volume;
		if ( mStreamSource != NULL )
		{
			mStreamSource->SetVolume(mMusicVolume);
		}
	}

	//===========================================================================

	void AudioManager::SetSoundVolume( float volume )
	{
		mSoundVolume = volume;
		std::vector<AudioStaticSource*>::iterator iter = mStaticSources.begin();
		while( iter != mStaticSources.end() )
		{	
			AudioStaticSource* source = *iter;
			source->SetVolume( mSoundVolume );
			iter++;
		}
	}
}
