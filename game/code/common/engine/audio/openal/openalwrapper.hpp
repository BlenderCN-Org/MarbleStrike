#if defined PLAT_PC || defined PLAT_IOS

#ifndef OPENAL_WRAPPER_HPP
#define OPENAL_WRAPPER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include <mutex>

#if defined PLAT_PC
#include "al.h"
#include "alc.h"
#endif

#if defined PLAT_IOS
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

#if _DEBUG
#define AUDIO_OPENAL_CHECK_ERROR 1
#endif

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace Audio
{
	namespace OpenAL
	{
#if defined PLAT_IOS
        typedef ALvoid (*alcMacOSXMixerOutputRateProcPtr) (const ALdouble value);
        ALvoid alcMacOSXMixerOutputRateProc(const ALdouble value);
#endif

		void CheckError();
		ALuint GetFormatType(int bitsPerSample, int channels);

		void oalGenBuffers( ALsizei n, ALuint* buffers );
		void oalGenSources( ALsizei n, ALuint* sources );
		void oalBufferData( ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq );

		void oalSourcePlay( ALuint sid );
		void oalSourceStop( ALuint sid );
		void oalSourcePause( ALuint sid );
		void oalSourceRewind( ALuint sid );

		void oalSourcei( ALuint sid, ALenum param, ALint value );
		void oalSourcef( ALuint sid, ALenum param, ALfloat value );

		void oalGetSourcei( ALuint sid, ALenum param, ALint* value );
		void oalGetSourcef( ALuint sid, ALenum param, ALfloat* value );

		void oalDeleteSources( ALsizei n, const ALuint* sources );
		void oalDeleteBuffers( ALsizei n, const ALuint* buffers );
		void oalSourceQueueBuffers( ALuint sid, ALsizei numEntries, const ALuint *bids );
		void oalSourceUnqueueBuffers( ALuint sid, ALsizei numEntries, ALuint *bids );
	}
}

#endif

#endif