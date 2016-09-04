//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/profiler.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	Profiler::Profiler( const char* name ) 
		:	mProfileTime( 0 )
	{
		System::StringCopy( mName, 256, name );
	}
    
    //===========================================================================

    Profiler::Profiler()
    {
        System::StringCopy( mName, 256, "" );
    }

	//===========================================================================

	Profiler::~Profiler()
	{
	}
    
    //===========================================================================
    
    void Profiler::SetName( const char *name )
    {
        System::StringCopy( mName, 256, name );
    }

    //===========================================================================

    const char* Profiler::GetName()
    {
        return mName;
    }
    
    //===========================================================================
    
    float Profiler::GetTime()
    {
        return mProfileTime;
    }

	//===========================================================================

	void Profiler::Start()
	{
		mTimer.Start();
	}

	//===========================================================================

	void Profiler::Stop()
	{
		mProfileTime = mTimer.GetElapsedTime() * 1000.0f;        
	}

	//===========================================================================

	void Profiler::Print()
	{		
		System::PrintToConsole( "%s : %f\n", mName, mProfileTime );
	}
}
