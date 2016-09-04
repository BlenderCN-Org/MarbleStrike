#if defined PLAT_PC || defined PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/format/iaudioformat.hpp"
#include "common/engine/audio/format/oggstream.hpp"
#include "common/engine/audio/format/wavestream.hpp"
#include "common/engine/audio/openal/openalaudiostreamsource.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "ios/audio/format/iosformatstream.hpp"


#if defined PLAT_IOS
#include <sys/time.h>
#include <errno.h>
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////
namespace Audio
{
    OpenALAudioStreamSource::OpenALAudioStreamSource()        
    {
        mMusicSource = 0;
        mShouldExitUpdateLoop = false;

        mMusicBuffers[0] = 0;
        mMusicBuffers[1] = 0;

        mAudioThread = std::thread( AudioUpdateThread, this );
    }

    //===========================================================================

    OpenALAudioStreamSource::~OpenALAudioStreamSource()
    {
        Unload();

        mAudioMutex.lock();

        if (mData != NULL)
        {
            DELETE_PTR( mData );
            mDataSize = 0;
        }

        mShouldExitUpdateLoop = true;

        mAudioMutex.unlock();

        mAudioThread.join();
    }

    //===========================================================================

    void OpenALAudioStreamSource::Load(const char* filename)
    {
        Unload();

        mAudioMutex.lock();

        OpenAL::oalGenBuffers( 2, mMusicBuffers );
        OpenAL::oalGenSources( 1, &mMusicSource );
        OpenAL::oalSourcef(mMusicSource, AL_GAIN, mVolume);

#ifdef PLAT_PC
        mAudioStreamFormat = NEW_PTR( "Ogg Stream" ) OggStream;
#endif
#ifdef PLAT_IOS
        mAudioStreamFormat = NEW_PTR( "iOS Stream" ) iOSFormatStream;
#endif

        // re-allocate the mData if buffer size is different.
        if (mData != NULL && mDataSize != mAudioStreamFormat->GetBufferSize())
        {
            DELETE_PTR(mData);
            mDataSize = 0;
        }

        if (mData == NULL)
        {
            mDataSize = mAudioStreamFormat->GetBufferSize();
            mData = NEW_PTR( "AudioStreamSource::mData" ) char[mDataSize];
        }

        mAudioStreamFormat->Load( filename );

        mIsEndofStream = false;

        mAudioMutex.unlock();
    }

    //===========================================================================

    void OpenALAudioStreamSource::Unload()
    {
        AudioStreamSource::Unload();

        Stop();

        mAudioMutex.lock();

        if ( mAudioStreamFormat != NULL)
        {
            mAudioStreamFormat->CloseStream();
            DELETE_PTR(mAudioStreamFormat);
        }

        if (mMusicSource != 0)
        {
            OpenAL::oalDeleteSources( 1, &mMusicSource);
        }

        if (mMusicBuffers[0] != 0)
        {
            OpenAL::oalDeleteBuffers( 1, &mMusicBuffers[0] );
        }

        if (mMusicBuffers[1] != 0)
        {
            OpenAL::oalDeleteBuffers( 1, &mMusicBuffers[1] );
        }

        mMusicSource = 0;
        mMusicBuffers[0] = 0;
        mMusicBuffers[1] = 0;

        mIsEndofStream = false;

        mAudioMutex.unlock();
    }

    //===========================================================================

    void OpenALAudioStreamSource::Play()
    {
        AudioStreamSource::Play();

        mAudioMutex.lock();

        if ( mMusicSource != 0 )
        {
            bool buffer1Used = Stream( mMusicBuffers[0] );
            bool buffer2Used = Stream( mMusicBuffers[1] );

            if ( buffer1Used && buffer2Used )
            {
                OpenAL::oalSourceQueueBuffers( mMusicSource, 2, mMusicBuffers );
            }
            else if ( buffer1Used )
            {
                OpenAL::oalSourceQueueBuffers( mMusicSource, 1, mMusicBuffers );
            }

            OpenAL::oalSourcePlay( mMusicSource );
            mIsEndofStream = false;
        }

        mAudioMutex.unlock();
    }

    //===========================================================================

    void OpenALAudioStreamSource::Stop()
    {
        AudioStreamSource::Stop();

        mAudioMutex.lock();

        if (mMusicSource != 0)
        {
            OpenAL::oalSourceStop( mMusicSource );
        }

        mAudioMutex.unlock();
    }

    //===========================================================================

    void OpenALAudioStreamSource::Pause()
    {
        AudioStreamSource::Pause();

        mAudioMutex.lock();

        if (mMusicSource != 0)
        {
            OpenAL::oalSourcePause( mMusicSource );
        }

        mAudioMutex.unlock();
    }

    //===========================================================================

    void OpenALAudioStreamSource::Unpause()
    {
        AudioStreamSource::Unpause();

        mAudioMutex.lock();

        if (mMusicSource != 0)
        {
            OpenAL::oalSourcePlay( mMusicSource );
        }

        mAudioMutex.unlock();
    }

    //===========================================================================

    void OpenALAudioStreamSource::Rewind()
    {
        mAudioMutex.lock();

        if (mMusicSource != 0)
        {
            OpenAL::oalSourceRewind( mMusicSource );	
            OpenAL::oalSourceStop( mMusicSource );
            mIsEndofStream = false;
        }

        mAudioMutex.unlock();
    }

    //===========================================================================

    bool OpenALAudioStreamSource::IsFinished()
    {
        mAudioMutex.lock();
        bool ret = false;
        ret = ( mAudioState == AUDIOSTATE_STOPPED) && mIsEndofStream;
        mAudioMutex.unlock();

        return ret;
    }


    //===========================================================================

    void OpenALAudioStreamSource::AudioUpdateThread( OpenALAudioStreamSource* openALAudioStreamSource )
    {	
        do
        {
            if ( openALAudioStreamSource->mAudioStreamFormat != NULL )
            {
                bool isEOF = false;
                while ( !isEOF && !openALAudioStreamSource->mShouldExitUpdateLoop )
                {	
                    openALAudioStreamSource->mAudioMutex.lock();

                    if ( openALAudioStreamSource->mMusicSource != 0)
                    {
                        int processed = 0;
                        OpenAL::oalGetSourcei( openALAudioStreamSource->mMusicSource, AL_BUFFERS_PROCESSED, &processed );

                        while ( processed-- )
                        {
                            ALuint buffer;

                            OpenAL::oalSourceUnqueueBuffers( openALAudioStreamSource->mMusicSource, 1, &buffer );
                            openALAudioStreamSource->Stream( buffer );
                            OpenAL::oalSourceQueueBuffers( openALAudioStreamSource->mMusicSource, 1, &buffer);		
                        }

                        isEOF = openALAudioStreamSource->mAudioStreamFormat->IsEOF();
                    }

                    openALAudioStreamSource->mAudioMutex.unlock();
                }

                openALAudioStreamSource->Stop();
                openALAudioStreamSource->mIsEndofStream = isEOF;
            }

        }
        while ( openALAudioStreamSource->mShouldExitUpdateLoop == false );
    }

    //===========================================================================

    void OpenALAudioStreamSource::Update()
    {
    }

    //===========================================================================

    bool OpenALAudioStreamSource::Stream( ALuint buffer )
    {
        int bufferSize = mAudioStreamFormat->GetBufferSize();

        memset( mData, 0, sizeof( char ) * bufferSize );

        int  size = 0;	
        int  result;

        while ( size < bufferSize )
        {
            result = mAudioStreamFormat->ReadStream( mData + size, bufferSize - size );

            if ( result > 0 )
            {
                size += result;
            }
            else
            {
                break;
            }
        }

        if ( size != 0 )
        {
            //
            // Handles the last bit of data from the audio data.  All sizes passed into alBufferData must
            // be block aligned.
            //
            size = bufferSize;

            ALenum format = OpenAL::GetFormatType( mAudioStreamFormat->GetBitsPerSample(), mAudioStreamFormat->GetChannels() );
            ALsizei freq = mAudioStreamFormat->GetSamplesPerSecond();
            OpenAL::oalBufferData( buffer, format, mData, size, freq );
            return true;
        }
        else
        {
            return false;
        }
    }

    //===========================================================================

    void OpenALAudioStreamSource::EmptyQueue()
    {
        int queued = 0;
        OpenAL::oalGetSourcei( mMusicSource, AL_BUFFERS_QUEUED, &queued );
        while(queued--)
        {
            ALuint buffer;
            OpenAL::oalSourceUnqueueBuffers( mMusicSource, 1, &buffer );				
        }
    }

    //===========================================================================

    void OpenALAudioStreamSource::SetVolume( float volume )
    {
        AudioStreamSource::SetVolume( volume );

        mAudioMutex.lock();

        if ( mVolume != volume )
        {
            if ( mMusicSource != 0 )
            {
                OpenAL::oalSourcef(mMusicSource, AL_GAIN, mVolume);
            }
        }

        mAudioMutex.unlock();
    }
}

#endif