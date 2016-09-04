#ifndef MEMORY_LOG_HPP
#define MEMORY_LOG_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory/memorysba.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

namespace System
{
	const int MAX_MEMORY_LOG_HASH = 500;
}

//////////////////////////////////////////////////////
// STRUCTURE
//////////////////////////////////////////////////////

namespace System
{
	struct MemoryData
	{
		unsigned int mAddress;
		char mName[32];
		const char* mFile;
		int mLineNumber;
		size_t mSize;
	};

	struct LogNode
	{
		MemoryData mData;
		LogNode* mNext;
	};
}

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{
	class MemoryLog
	{
	public:
#if defined USE_MEMORY_HEAPS
		MemoryLog(IAllocator* logMemHeap);
#endif
        MemoryLog();
		~MemoryLog();

		static MemoryLog* GetInstance();

		void Insert( void* ptr, MemoryData data );
		bool Remove( void* ptr );
		void CheckMemoryLog();
		void DumpMemory();
		
		size_t GetMemoryUsed() 
		{ 
			return mTotalMemoryUsage; 
		}

	private:

		int GetMemoryAddressHashValue( unsigned int index );
		bool RemoveInternal( size_t index );

#if defined USE_MEMORY_HEAPS
		MemorySBA mLogMemPool;
#endif
		LogNode* mLogList[MAX_MEMORY_LOG_HASH];
		int mNumItems;
		size_t mTotalMemoryUsage;
	};
}

#endif 