#ifndef SYSTEM_TIMER_HPP
#define SYSTEM_TIMER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined ( PLAT_PC ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
#include <windows.h>
#include <mmsystem.h>
#endif

#if defined PLAT_XBOX360
#include <xtl.h>
#endif

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	class Timer  
	{
	public:
		Timer();
		~Timer();

        void Start();

        double GetTime(); 

        void UpdateFPS(); 

		float GetElapsedTime(); 

		double GetFPS() 
		{ 
			return mFramesPerSecond; 
		} 

	private:

        double mFramesPerSecond;
		double mLastUpdate;
		double mFramesPerSecondUpdateInterval;
		unsigned int  mNumFrames;    
		double mTimeAtGameStart;
		double mCurrentTime;

#if defined ( PLAT_PC ) || defined ( PLAT_XBOX360 ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		LARGE_INTEGER mStartTick;
		LARGE_INTEGER mFrequency;
#endif
		bool mIsInitialize;
	};
}

#endif 
