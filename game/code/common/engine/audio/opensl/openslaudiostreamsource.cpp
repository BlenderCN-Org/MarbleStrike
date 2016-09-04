#if defined AUDIO_OPENSL

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/audio/format/oggstream.hpp"
#include "common/engine/audio/format/wavestream.hpp"
#include "common/engine/audio/opensl/openslaudiostreamsource.hpp"
#include "common/engine/audio/format/iaudioformat.hpp"
#include "common/engine/system/pathutilities.hpp"
#include <SLES/OpenSLES_Android.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////
namespace Audio
{
    OpenSLAudioStreamSource::OpenSLAudioStreamSource()        
    {
		mBGMPlayerObject = NULL;
		mBGMPlayer = NULL;
		mBGMPlayerSeek = NULL;
        mOutputMixObject = NULL;
        mOpenSLEngineEngine = NULL;
        mVolume = NULL;
    }

    //===========================================================================

    OpenSLAudioStreamSource::~OpenSLAudioStreamSource()
    {
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Load(const char* filename)
    {
		Assert( mOutputMixObject != NULL, "" );

		char buffer[256];
		System::StringCopy( buffer, 256, filename );
		System::StringConcat( buffer, 256, ".mp4" );
		System::FixedString<256> loadPath = System::Path::SystemSlash( buffer );

		char filenameFullPath[256];
		const char* resourcePath = Database::Get()->GetResourcePath();
		System::StringCopy( filenameFullPath, 256, "" );
		System::StringConcat( filenameFullPath, 256, resourcePath );
		System::StringConcat( filenameFullPath, 256, loadPath );

        std::string testString = filenameFullPath;
        FILE* file = System::OpenFile( filenameFullPath, "rb" );
        fclose( file );

		SLresult result;
        		
        SLDataLocator_URI dataLocatorUri = { SL_DATALOCATOR_URI, (SLchar*)testString.c_str() };

        SLDataFormat_MIME dataFormat;
        dataFormat.formatType = SL_DATAFORMAT_MIME;
        dataFormat.mimeType = NULL;
        dataFormat.containerType = SL_CONTAINERTYPE_UNSPECIFIED;

        SLDataSource dataSource;
        dataSource.pLocator = &dataLocatorUri;
        dataSource.pFormat = &dataFormat;

		SLDataLocator_OutputMix dataLocatorOutputMix;
        dataLocatorOutputMix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
        dataLocatorOutputMix.outputMix = mOutputMixObject;

        SLDataSink dataSink;
        dataSink.pLocator = &dataLocatorOutputMix;
        dataSink.pFormat = NULL;

        const SLuint32 BGMPlayerIIDCount = 3;
        const SLInterfaceID BGMPlayerIIDs[] = { SL_IID_PLAY, SL_IID_SEEK, SL_IID_VOLUME };
        const SLboolean BGMPlayerReqs[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

        result = (*mOpenSLEngineEngine)->CreateAudioPlayer(
            mOpenSLEngineEngine, 
            &mBGMPlayerObject, 
            &dataSource, 
            &dataSink, 
            BGMPlayerIIDCount, 
            BGMPlayerIIDs, 
            BGMPlayerReqs
            );
        Assert( result == SL_RESULT_SUCCESS, "" );

        result = (*mBGMPlayerObject)->Realize( mBGMPlayerObject, SL_BOOLEAN_FALSE );
        Assert( result == SL_RESULT_SUCCESS, "" );

        result = (*mBGMPlayerObject)->GetInterface( mBGMPlayerObject, SL_IID_PLAY, &mBGMPlayer );
        Assert( result == SL_RESULT_SUCCESS, "" );

        result = (*mBGMPlayerObject)->GetInterface( mBGMPlayerObject, SL_IID_SEEK, &mBGMPlayerSeek );
        Assert( result == SL_RESULT_SUCCESS, "" );

        result = (*mBGMPlayerObject)->GetInterface( mBGMPlayerObject, SL_IID_VOLUME, &mVolume );
        Assert( result == SL_RESULT_SUCCESS, "" );

        result = (*mBGMPlayerSeek)->SetLoop( mBGMPlayerSeek, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN) ;
        Assert( result == SL_RESULT_SUCCESS, "" );

        SetVolume( Database::Get()->GetVolume() );
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Unload()
    {
        SLresult result;
        if ( mBGMPlayerObject )
        {
            (*mBGMPlayerObject)->Destroy( mBGMPlayerObject );            
            mBGMPlayerObject = NULL;
            mBGMPlayer = NULL;
            mBGMPlayerSeek = NULL;
            mVolume = NULL;
        }
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Play()
    {
        SLresult result;
        if ( mBGMPlayer != NULL ) 
        {
            SLuint32 BGMPlayerState;
            result = (*mBGMPlayerObject)->GetState( mBGMPlayerObject, &BGMPlayerState );
            Assert( result == SL_RESULT_SUCCESS, "" );

            if ( BGMPlayerState == SL_OBJECT_STATE_REALIZED ) 
            {
                result = (*mBGMPlayer)->SetPlayState( mBGMPlayer, SL_PLAYSTATE_PLAYING );
                Assert( result == SL_RESULT_SUCCESS, "" );
            }
        }
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Stop()
    {
        SLresult result;
        if ( mBGMPlayer != NULL ) 
        {
            SLuint32 BGMPlayerState;
            result = (*mBGMPlayerObject)->GetState( mBGMPlayerObject, &BGMPlayerState );
            Assert( result == SL_RESULT_SUCCESS, "" );

            if ( BGMPlayerState == SL_OBJECT_STATE_REALIZED ) 
            {
                result = (*mBGMPlayer)->SetPlayState( mBGMPlayer, SL_PLAYSTATE_STOPPED );                
                Assert( result == SL_RESULT_SUCCESS, "" );
            }
        }
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Pause()
    {
        SLresult result;
        if ( mBGMPlayer != NULL ) 
        {
            SLuint32 BGMPlayerState;
            result = (*mBGMPlayerObject)->GetState( mBGMPlayerObject, &BGMPlayerState );
            Assert( result == SL_RESULT_SUCCESS, "" );

            if ( BGMPlayerState == SL_OBJECT_STATE_REALIZED ) 
            {
                result = (*mBGMPlayer)->SetPlayState( mBGMPlayer, SL_PLAYSTATE_PAUSED );                
                Assert( result == SL_RESULT_SUCCESS, "" );
            }
        }
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Unpause()
    {
        Play();
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Rewind()
    {
        Stop();
    }

    //===========================================================================

    bool OpenSLAudioStreamSource::IsFinished()
    {
        SLresult result;
        SLuint32 playState = 0;
        if ( mBGMPlayer != NULL ) 
        {
            SLuint32 BGMPlayerState;
            result = (*mBGMPlayerObject)->GetState( mBGMPlayerObject, &BGMPlayerState );
            Assert( result == SL_RESULT_SUCCESS, "" );

            if ( BGMPlayerState == SL_OBJECT_STATE_REALIZED ) 
            {
                result = (*mBGMPlayer)->GetPlayState( mBGMPlayer, &playState );                
                Assert( result == SL_RESULT_SUCCESS, "" );

                return playState == SL_PLAYSTATE_STOPPED;
            }
        }

        return false;
    }

    //===========================================================================

    void OpenSLAudioStreamSource::Update()
    {
    }

    //===========================================================================

	void OpenSLAudioStreamSource::SetVolume( float volume )
    {		
        SLresult result;

        if ( mVolume )
        {
        	SLmillibel maxVolume = 0;
        	result = (*mVolume)->GetMaxVolumeLevel( mVolume, &maxVolume );
        	Assert( result == SL_RESULT_SUCCESS, "" );

        	SLmillibel minVolume = SL_MILLIBEL_MIN;

            float maxPower = pow( 10, maxVolume / 1000.0f );
            float minPower = pow( 10, minVolume / 1000.0f );
            
        	float newPower = minPower + ( maxPower - minPower ) * volume;
            
            SLmillibel newVolume = SL_MILLIBEL_MIN;

            if ( newPower > 0 )
            {
                newVolume = log( newPower ) * 1000.0f;
            }

            if ( volume == 0 )
            {
                newVolume = SL_MILLIBEL_MIN;
            }

        	result = (*mVolume)->SetVolumeLevel( mVolume, newVolume );
        	Assert( result == SL_RESULT_SUCCESS, "" );
        }
    }

    //===========================================================================

    void OpenSLAudioStreamSource::SetOpenSLEngineEngine( SLEngineItf openSLEngineEngine )
    {
        mOpenSLEngineEngine = openSLEngineEngine;
    }

    //===========================================================================

    void OpenSLAudioStreamSource::SetOutputMixObject( SLObjectItf outputMixObject )
    {
        mOutputMixObject = outputMixObject;
    }
}

#endif
