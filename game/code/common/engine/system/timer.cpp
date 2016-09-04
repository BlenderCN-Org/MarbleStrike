//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/timer.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

#if defined PLAT_PS3
#include <sys/sys_time.h>
#endif

#if defined PLAT_IOS
#include "ios/system/iostimer.h"
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
    Timer::Timer() 
		:	mFramesPerSecond( 0 )
        ,	mLastUpdate( 0 )
		,	mFramesPerSecondUpdateInterval( 0.5f )
		,   mNumFrames( 0 )
		,	mTimeAtGameStart( 0 )
		,   mCurrentTime( 0 )
		,	mIsInitialize( false )
    {
    }

    //===========================================================================

    Timer::~Timer()
    {
    }

    //===========================================================================

    void Timer::Start()
    {
		mFramesPerSecond = 0;
		mLastUpdate = 0;
		mFramesPerSecondUpdateInterval = 0.5f;
		mNumFrames = 0;
		mTimeAtGameStart = 0;
		mIsInitialize = true;

#if defined PLAT_PC || defined PLAT_XBOX360 || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
        if ( !QueryPerformanceFrequency( &mFrequency ) )
        {
            mFrequency.QuadPart = 1000;
        }
		
		QueryPerformanceCounter( &mStartTick );		
#endif
        mCurrentTime = GetTime();
	}
    
    //===========================================================================

    double Timer::GetTime()
    {
        Assert( mIsInitialize == true, "Timer is not initialized" );

#if defined( PLAT_ANDROID )
		struct timespec now;
		int result = clock_gettime(CLOCK_MONOTONIC, &now);
		Assert( result == 0, "" );
		const float MULTIPLIER = 0.000000001f;
		double time = now.tv_sec + now.tv_nsec * MULTIPLIER;
#endif

#if defined ( PLAT_PC ) || defined ( PLAT_XBOX360 ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
        LARGE_INTEGER ticks;
        double time;

        if( !QueryPerformanceCounter( &ticks ))
        {
#if defined PLAT_PC
            ticks.QuadPart = timeGetTime();
#endif
#if defined PLAT_XBOX360
			Assert( false, "" );
#endif
        }

        time = (double)( ticks.QuadPart - mStartTick.QuadPart ) / (double)mFrequency.QuadPart;		
#endif

#if defined PLAT_PS3
		double time;
		time = static_cast<float>( sys_time_get_system_time() ) / 1000000.0f;
#endif
		
#if defined PLAT_IOS
		double time;
		time = iOSTimer::GetTime();
#endif

        return time;
    }

    //===========================================================================

    void Timer::UpdateFPS()
    {
        mNumFrames++;
        double currentUpdate = GetTime();
        if( currentUpdate - mLastUpdate > mFramesPerSecondUpdateInterval )
        {
            mFramesPerSecond = mNumFrames / ( currentUpdate - mLastUpdate );
            mLastUpdate = currentUpdate;
            mNumFrames = 0;
        }
    }

    //===========================================================================

    float Timer::GetElapsedTime()
    {
        double prevTime = mCurrentTime;
        mCurrentTime = GetTime();

        double deltaTime = mCurrentTime - prevTime;

        return static_cast<float>( deltaTime );
    }
}
