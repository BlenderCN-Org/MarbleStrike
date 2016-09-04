#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "winphone/audio/mediafoundationaudiostreamsource.hpp"
#include "winphone/audio/mediafoundationdevice.hpp"
#include "common/engine/database/database.hpp"

//////////////////////////////////////////////////////
// GLOBAL
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{   	
	MediaFoundationAudioStreamSource::MediaFoundationAudioStreamSource()
		: AudioStreamSource()
	{   
		mDone = false;
	}

	//===========================================================================

	MediaFoundationAudioStreamSource::~MediaFoundationAudioStreamSource()
	{
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Load( const char* filename )
	{		
		char fullPathFilename[256];
		System::StringCopy( fullPathFilename, 256, "" );
		System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
		System::StringConcat( fullPathFilename, 256, filename );
		System::StringConcat( fullPathFilename, 256, ".mp4" );

		size_t returnChar = 0;
		const size_t cSize = strlen(fullPathFilename)+1;
		wchar_t sourcePath[256];
		mbstowcs_s( &returnChar, sourcePath, 256, fullPathFilename, cSize);

		HRESULT hr = MediaFoundationDevice::Get()->GetMediaEngine()->SetSource( sourcePath );
		Assert( hr == S_OK, "" );
        UNUSED_ALWAYS( hr );
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Unload()
	{
        mDone = false;
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Play()
	{
		HRESULT hr = S_OK;
		hr = MediaFoundationDevice::Get()->GetMediaEngine()->Play();
		Assert( hr == S_OK, "" ); 

        AudioStreamSource::Play();
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Stop()
	{
		HRESULT hr = S_OK;
		hr = MediaFoundationDevice::Get()->GetMediaEngine()->Pause();
		Assert( hr == S_OK, "" ); 
		hr = MediaFoundationDevice::Get()->GetMediaEngine()->SetCurrentTime( 0 );
		Assert( hr == S_OK, "" ); 

        AudioStreamSource::Stop();
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Pause()
	{
        //HRESULT hr = S_OK;
        //hr = MediaFoundationDevice::Get()->GetMediaEngine()->Pause();
		//Assert( hr == S_OK, "" ); 

        AudioStreamSource::Pause();
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Unpause()
	{
        Play();
        AudioStreamSource::Unpause();
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Rewind()
	{
        Stop();
	}

	//===========================================================================

	bool MediaFoundationAudioStreamSource::IsFinished()
	{
		return mDone;
	}

	//===========================================================================

	AudioState MediaFoundationAudioStreamSource::GetState()
	{
		return mAudioState;
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::Update()
	{
        if ( MediaFoundationDevice::Get()->GetEventState() == MF_MEDIA_ENGINE_EVENT_ENDED )
        {
            mDone = true;
        }
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::SetVolume( float volume )
	{
        UNUSED_ALWAYS( volume );
	}

	//===========================================================================

	void MediaFoundationAudioStreamSource::SetDone( bool value )
	{
		mDone = value;
	}
}

#endif