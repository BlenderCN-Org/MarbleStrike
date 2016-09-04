//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/profilermanager.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace System
{
    ProfilerManager* ProfilerManager::mInstance = NULL;
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
    void ProfilerManager::Create()
    {
#if defined PLAT_PROFILE
        Assert( mInstance == NULL, "" );
        if ( mInstance == NULL )
        {
            mInstance = NEW_PTR( "ProfilerManager" ) ProfilerManager;
        }
#endif
    }

    //===========================================================================

    void ProfilerManager::Destroy()
    {
#if defined PLAT_PROFILE
        Assert( mInstance != NULL,  "" );
        if ( mInstance )
        {
            DELETE_PTR( mInstance );
        }
#endif
    }

    //===========================================================================

    ProfilerManager* ProfilerManager::Get()
    {
#if defined PLAT_PROFILE
        Assert( mInstance != NULL, "" );
        return mInstance;
#else
        return NULL;
#endif
    }

    //===========================================================================

    int ProfilerManager::BeginProfiling( const char* name )
    {
#if defined PLAT_PROFILE
        int currentProfiler = mNumProfilers;
        int previousProfiler = currentProfiler - 1;
        while ( previousProfiler >= 0 )
        {
            if ( mProfilers[previousProfiler].mIsRunning )
            {
                mProfilers[currentProfiler].mParent = previousProfiler;
                break;
            }
            else
            {
                previousProfiler = mProfilers[previousProfiler].mParent;
            }
        }

        mProfilers[currentProfiler].mIsRunning = true;
        mProfilers[currentProfiler].mProfiler.SetName( name );
        mProfilers[currentProfiler].mProfiler.Start();

        mNumProfilers++;

        return currentProfiler;
#else
        UNUSED_ALWAYS( name );
        return 0;
#endif
    }

    //===========================================================================

    void ProfilerManager::EndProfiling( int index )
    {
#if defined PLAT_PROFILE
        mProfilers[index].mIsRunning = false;
        mProfilers[index].mProfiler.Stop();
#else
        UNUSED_ALWAYS( index );
#endif
    }

    //===========================================================================

    void ProfilerManager::Print()
    {
#if defined PLAT_PROFILE
        for ( int i = 0; i < mNumProfilers; ++i )
        {
            int parent = mProfilers[i].mParent;
            int indentCount = 0;
            while ( parent >= 0 )
            {
                indentCount++;
                parent = mProfilers[parent].mParent;
            }

            Profiler* profiler = &( mProfilers[i].mProfiler);
            
            for ( int i = 0; i < indentCount; ++i )
            {
                System::PrintToConsole( " " );
            }
            System::PrintToConsole( "%s : %f\n", profiler->GetName(), profiler->GetTime() );
        }
#endif
    }

    //===========================================================================

    void ProfilerManager::Clear()
    {
#if defined PLAT_PROFILE
        mNumProfilers = 0;
#endif
    }

    //===========================================================================

    ProfilerManager::ProfilerManager()
    {
#if defined PLAT_PROFILE
        mNumProfilers = 0;
#endif
    }

    //===========================================================================

    ProfilerManager::~ProfilerManager()
    {
    }    
}
