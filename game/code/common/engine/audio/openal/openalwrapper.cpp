#if defined PLAT_PC || defined PLAT_IOS

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/audio/openal/openalwrapper.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace Audio
{
    namespace OpenAL
    {
        static std::mutex gOpenALMutex;
#if PLAT_IOS
        static alcMacOSXMixerOutputRateProcPtr sAlcMacOSXMixerOutputRateProc = NULL;
#endif
    }
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace Audio
{
    namespace OpenAL
    {
#if PLAT_IOS
        ALvoid alcMacOSXMixerOutputRateProc(const ALdouble value)
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            CheckError();
            if (sAlcMacOSXMixerOutputRateProc == NULL)
            {
                sAlcMacOSXMixerOutputRateProc = reinterpret_cast<alcMacOSXMixerOutputRateProcPtr> (alcGetProcAddress(NULL, "alcMacOSXMixerOutputRate"));
            }

            if (sAlcMacOSXMixerOutputRateProc != NULL)
            {
                sAlcMacOSXMixerOutputRateProc(value);
            }
            CheckError();
        }
#endif

        //===========================================================================

        void CheckError()
        {
#if AUDIO_OPENAL_CHECK_ERROR
            ALenum errorCode = alGetError();

            if ( errorCode != AL_NO_ERROR )
            {
                System::PrintToConsole("OpenAL CheckError() failed: %s\n", alGetString(errorCode)); 
                Assert( false, "" );
            }
#endif
        }

        //===========================================================================

        ALuint GetFormatType( int bitsPerSample, int channels )
        {
            ALuint format = 0;

            switch ( bitsPerSample )
            {
            case 8:
                if ( channels == 1 )
                {
                    format = AL_FORMAT_MONO8;
                }
                else
                {
                    format = AL_FORMAT_STEREO8;
                }			
                break;
            case 16:
                if ( channels == 1 )
                {
                    format = AL_FORMAT_MONO16;
                }
                else
                {
                    format = AL_FORMAT_STEREO16;
                }				
                break;
            default:
                Assert( false, "OpenAL::GetFormatType() unexpected bitsPerSample." );
                break;
            }

            return format;
        }

        //===========================================================================

        void oalGenBuffers( ALsizei n, ALuint* buffers )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alGenBuffers( n, buffers );
            CheckError();
        }

        //===========================================================================

        void oalGenSources( ALsizei n, ALuint* sources )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alGenSources( n, sources );
            CheckError();
        }

        //===========================================================================

        void oalBufferData( ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alBufferData( bid, format, data, size, freq );
            CheckError();
        }

        //===========================================================================

        void oalSourcei( ALuint sid, ALenum param, ALint value )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourcei( sid, param, value );
            CheckError();
        }

        //===========================================================================

        void oalSourcef( ALuint sid, ALenum param, ALfloat value )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourcef( sid, param, value );
            CheckError();
        }

        //===========================================================================

        void oalGetSourcei( ALuint sid, ALenum param, ALint* value )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alGetSourcei( sid, param, value );
            CheckError();
        }

        //===========================================================================

        void oalGetSourcef( ALuint sid, ALenum param, ALfloat* value )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alGetSourcef( sid, param, value );
            CheckError();
        }

        //===========================================================================

        void oalSourcePlay( ALuint sid )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourcePlay( sid );
            CheckError();
        }

        //===========================================================================

        void oalSourceStop( ALuint sid )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourceStop( sid );
            CheckError();
        }

        //===========================================================================

        void oalSourcePause( ALuint sid )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourcePause( sid );
            CheckError();
        }

        //===========================================================================

        void oalSourceRewind( ALuint sid )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourceRewind( sid );
            CheckError();
        }

        //===========================================================================

        void oalDeleteSources( ALsizei n, const ALuint* sources )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alDeleteSources( n, sources );
            CheckError();
        }

        //===========================================================================

        void oalDeleteBuffers( ALsizei n, const ALuint* buffers )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alDeleteBuffers( n, buffers );
            CheckError();
        }

        //===========================================================================

        void oalSourceQueueBuffers( ALuint sid, ALsizei numEntries, const ALuint *bids )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourceQueueBuffers( sid, numEntries, bids );
            CheckError();
        }

        //===========================================================================//===========================================================================

        void oalSourceUnqueueBuffers( ALuint sid, ALsizei numEntries, ALuint* bids )
        {
            std::lock_guard<std::mutex> lock( gOpenALMutex );
            alSourceUnqueueBuffers( sid, numEntries, bids );
            CheckError();
        }
    }
}

#endif