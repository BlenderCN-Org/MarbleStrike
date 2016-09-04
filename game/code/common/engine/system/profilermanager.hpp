#ifndef PROFILER_MANAGER_HPP
#define PROFILER_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/profiler.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

const int MAX_PROFILERS = 50;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
    struct ProfilerData
    {
        ProfilerData()
        {
            mParent = -1;
            mIsRunning = false;
        }

        Profiler mProfiler;
        int mParent;
        bool mIsRunning;
    };

	class ProfilerManager  
	{
	public:

        static void Create();
        static void Destroy();
        static ProfilerManager* Get();

        int BeginProfiling( const char* name );
        void EndProfiling( int index );

        void Print();
        void Clear();
        
	private:

        ProfilerManager();
		~ProfilerManager();

        static ProfilerManager* mInstance;

        int mNumProfilers;
        ProfilerData mProfilers[MAX_PROFILERS];
        
	};
}

#endif 
