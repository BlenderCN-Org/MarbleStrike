#if defined AUDIO_OPENSL

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudiostaticformat.hpp"
#include "common/engine/audio/format/ogg.hpp"
#include "common/engine/audio/opensl/openslaudiostaticsource.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include <SLES/OpenSLES_android.h>
#include <math.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

#if defined AUDIO_OPENSL

namespace Audio
{
	OpenSLAudioStaticSource::OpenSLAudioStaticSource()        
	{
		mBuffer = NULL;
		mBufferSize = 0;
		mPlayerObject = NULL;
		mPlayer = NULL;
        mPlayerQueue = NULL;
		mOpenSLEngineEngine = NULL;
        mVolume = NULL;
	}

	//===========================================================================

	OpenSLAudioStaticSource::~OpenSLAudioStaticSource()
	{	
		DELETE_PTR_ARRAY( mBuffer );		

		if ( mPlayerObject != NULL ) 
		{
			(*mPlayerObject)->Destroy( mPlayerObject );
			mPlayerObject = NULL; 
			mPlayer = NULL; 
			mPlayerQueue = NULL;
		}
	}

	//===========================================================================

	void OpenSLAudioStaticSource::Load( const char* filename )
	{
        mFileName = filename;

		IAudioStaticFormat* audioStaticFormat = NEW_PTR( "Audio Static Format" ) Ogg;

		audioStaticFormat->Load( filename );

		mBufferSize = audioStaticFormat->GetDataSize();
		mBuffer = NEW_PTR( "Audio Buffer" ) char[mBufferSize];
		memcpy( (void*)mBuffer, audioStaticFormat->GetData(), mBufferSize );

		SLresult result;

		SLDataLocator_AndroidSimpleBufferQueue dataLocatorIn;
		dataLocatorIn.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;		
		dataLocatorIn.numBuffers = 1;

		SLDataFormat_PCM dataFormat;
		dataFormat.formatType = SL_DATAFORMAT_PCM;
		dataFormat.numChannels = audioStaticFormat->GetChannels();
		dataFormat.samplesPerSec = SL_SAMPLINGRATE_44_1;
		//dataFormat.samplesPerSec = audioStaticFormat->GetSamplesPerSecond();
		dataFormat.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;//audioStaticFormat->GetBitsPerSample();
		//dataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_8;
		dataFormat.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;

        if ( dataFormat.numChannels > 1 ) 
        {
            dataFormat.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
        }
        else 
        {
            dataFormat.channelMask = SL_SPEAKER_FRONT_CENTER;
        }
		
		dataFormat.endianness = SL_BYTEORDER_LITTLEENDIAN;

		SLDataSource dataSource;
		dataSource.pLocator = &dataLocatorIn;
		dataSource.pFormat = &dataFormat;

		SLDataLocator_OutputMix dataLocatorOut;
		dataLocatorOut.locatorType = SL_DATALOCATOR_OUTPUTMIX;
		dataLocatorOut.outputMix = mOutputMixObject;

		SLDataSink dataSink;
		dataSink.pLocator = &dataLocatorOut;
		dataSink.pFormat = NULL;

		const SLuint32 soundPlayerIIDCount = 3;
		const SLInterfaceID soundPlayerIIDs[] = { SL_IID_PLAY, SL_IID_BUFFERQUEUE, SL_IID_VOLUME };
		const SLboolean soundPlayerReqs[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };

		result = (*mOpenSLEngineEngine)->CreateAudioPlayer( 
			mOpenSLEngineEngine, 
			&mPlayerObject, 
			&dataSource, 
			&dataSink, 
			soundPlayerIIDCount, 
			soundPlayerIIDs, 
			soundPlayerReqs
			);
		Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mPlayerObject)->Realize( mPlayerObject, SL_BOOLEAN_FALSE );
		Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mPlayerObject)->GetInterface( mPlayerObject, SL_IID_PLAY, &mPlayer );
		Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mPlayerObject)->GetInterface( mPlayerObject,	SL_IID_BUFFERQUEUE, &mPlayerQueue );
		Assert( result == SL_RESULT_SUCCESS, "" );

        result = (*mPlayerObject)->GetInterface( mPlayerObject, SL_IID_VOLUME, &mVolume );
        Assert( result == SL_RESULT_SUCCESS, "" );

		result = (*mPlayer)->SetPlayState( mPlayer, SL_PLAYSTATE_PLAYING );
		Assert( result == SL_RESULT_SUCCESS, "" );

        SetVolume( Database::Get()->GetVolume() );

        DELETE_PTR( audioStaticFormat );
	}

	//===========================================================================

	void OpenSLAudioStaticSource::Play()
	{
		SLresult result;
		SLuint32 playerState;
		(*mPlayerObject)->GetState( mPlayerObject, &playerState );

		if ( playerState == SL_OBJECT_STATE_REALIZED ) 
		{				
			result = (*mPlayerQueue)->Clear( mPlayerQueue );
			Assert( result == SL_RESULT_SUCCESS, "" );
						
			result = (*mPlayerQueue)->Enqueue( mPlayerQueue, mBuffer, mBufferSize );
			Assert( result == SL_RESULT_SUCCESS, "" );
		}


		AudioStaticSource::Play();
	}

	//===========================================================================

	void OpenSLAudioStaticSource::Pause()
	{
	}

	//===========================================================================

	void OpenSLAudioStaticSource::Stop()
	{

        AudioStaticSource::Stop();		
	}

	//===========================================================================

	AudioState OpenSLAudioStaticSource::GetState()
	{    
        return AUDIOSTATE_UNDEFINED;	
	}

	//===========================================================================

	void OpenSLAudioStaticSource::SetVolume( float volume )
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

	void OpenSLAudioStaticSource::SetOpenSLEngineEngine( SLEngineItf openSLEngineEngine )
	{
		mOpenSLEngineEngine = openSLEngineEngine;
	}

	//===========================================================================

    void OpenSLAudioStaticSource::SetOutputMixObject( SLObjectItf outputMixObject )
    {
        mOutputMixObject = outputMixObject;
    }
}

#endif

#endif
