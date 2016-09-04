#ifndef PROFILER_HPP
#define PROFILER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/timer.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	class Profiler  
	{
	public:

		Profiler( const char* name );
        Profiler();
		~Profiler();

        void SetName( const char* name );
        const char* GetName();
        float GetTime();
		void Start();
		void Stop();
		void Print();

	private:

		char mName[256];
		Timer mTimer;
		float mProfileTime;

	};
}

#endif 
