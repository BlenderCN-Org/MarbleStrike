//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/memory/memorylog.hpp"
#include "common/engine/system/memory/memorymalloc.hpp"
#include "common/engine/system/memory/memorydlmalloc.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace System
{
#if defined USE_MEMORY_HEAPS
    const int MAX_MEMORY_LOG_SIZE = 100 * 1024;
	static char gLogMemData[64 * MAX_MEMORY_LOG_SIZE + MemoryDLMalloc::DLHEAP_HEADER_SIZE]; // memory needed for logging + initial memory needed for dlmalloc header

	static char gLogMemHeapData[sizeof(MemoryDLMalloc)];
	static MemoryDLMalloc* gLogMemHeapPtr = NULL;

	static char gMemoryLogData[sizeof(MemoryLog)];
	static MemoryLog* gMemoryLogPtr = NULL;
#endif

	inline size_t GetAlignValue(size_t x, size_t align)
	{
		size_t value = x / align * align;

		if (x % align > 0)
		{
			value += align;
		}

		return value;
	}
}

namespace System
{
	MemoryLog* MemoryLog::GetInstance()
	{
#if defined USE_MEMORY_HEAPS
		if (gMemoryLogPtr == NULL)
		{
			gLogMemHeapPtr = new(gLogMemHeapData) MemoryDLMalloc(gLogMemData, sizeof(gLogMemData));
			gMemoryLogPtr = new(gMemoryLogData) MemoryLog(gLogMemHeapPtr);
		}
#endif
		return 0;
	}
}


//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
#if defined USE_MEMORY_HEAPS
	MemoryLog::MemoryLog(IAllocator* logMemHeap) 
		:	mLogMemPool(GetAlignValue(sizeof(LogNode), MEMORY_DEFAULT_ALIGNMENT), MAX_MEMORY_LOG_SIZE, logMemHeap)
	{
#endif
    MemoryLog::MemoryLog()
    {
		for ( int i = 0; i < MAX_MEMORY_LOG_HASH; ++i )
		{
			mLogList[i] = NULL;
		}

		mNumItems = 0;
		mTotalMemoryUsage = 0;
	}

	//===========================================================================

	MemoryLog::~MemoryLog()
	{
	}

	//===========================================================================
	
	void MemoryLog::Insert( void* ptr, MemoryData data )
	{
		unsigned int i = GetMemoryAddressHashValue(*((unsigned int*)&ptr));

#if defined USE_MEMORY_HEAPS
		void* mem = mLogMemPool.Alloc(sizeof(LogNode));
		Assert(mem != NULL, "MemoryLog: not enough memory allocated for logging.");
        LogNode* newNode = new(mem) LogNode;
#endif

		LogNode* newNode = new LogNode;
		newNode->mData = data;
		newNode->mNext = NULL;

		if ( mLogList[i] == NULL )
		{
			mLogList[i] = newNode;
		}
		else
		{
			newNode->mNext = mLogList[i];
			mLogList[i] = newNode;
		}

		mNumItems++;

		mTotalMemoryUsage += data.mSize;
	}

	//===========================================================================
	
	bool MemoryLog::Remove( void* ptr )
	{
		if ( ptr != NULL )
		{
			if ( RemoveInternal( reinterpret_cast<size_t>(ptr) ) == false )
			{
				if ( RemoveInternal( reinterpret_cast<size_t>(ptr) - 4 ) == false )
				{
					//Assert( false, "MemoryLog internal error, Remove() call failed." );
					return false;
				}
			}

			return true;
		}
		else
		{
			return true;
		}
	}

	//===========================================================================
	
	bool MemoryLog::RemoveInternal( size_t index )
	{
		unsigned int i = GetMemoryAddressHashValue((int)index);

		if ( mLogList[i] == NULL )
		{
			return false;
		}

		LogNode* currentNode = mLogList[i];
		LogNode* prevNode = NULL;
		while ( currentNode != NULL )
		{
			if ( currentNode->mData.mAddress == index )
			{
				if ( prevNode == NULL )
				{
					mLogList[i] = currentNode->mNext;				
				}
				else
				{
					prevNode->mNext = currentNode->mNext;
				}

				mTotalMemoryUsage -= currentNode->mData.mSize;

#if defined USE_MEMORY_HEAPS
				mLogMemPool.Free(currentNode);
#endif
                delete currentNode;

				mNumItems--;
				return true;
			}
			else
			{
				prevNode = currentNode;
				currentNode = currentNode->mNext;
			}		
		}		

		return false;
	}

	//===========================================================================
	
	void MemoryLog::CheckMemoryLog()
	{
		const int KNOWN_MEM_LEAK = 1;

		if ( mNumItems > KNOWN_MEM_LEAK )
		{
            DumpMemory();
			Assert( false, "Memory Leak" );
		}
	}

	//===========================================================================
	
	void MemoryLog::DumpMemory()
	{
		int totalMemory = 0;

		FILE* dumpLog = System::OpenFile( "dumplog.txt", "w+" );

		for ( int i = 0; i < MAX_MEMORY_LOG_HASH; ++i )
		{
			if ( mLogList[i] != NULL )
			{
				LogNode* currentNode = mLogList[i];
				while ( currentNode )
				{
					int memorySize = static_cast<int>( currentNode->mData.mSize );
                    System::PrintToConsole( "%s : %i \n", currentNode->mData.mName, memorySize );
					fprintf( dumpLog, "%s : %i \n", currentNode->mData.mName, memorySize );

					totalMemory+= currentNode->mData.mSize;

					currentNode = currentNode->mNext;
				}
			}
		}

        System::PrintToConsole( "Total Memory: %i\n", totalMemory );
		fprintf( dumpLog, "Total Memory: %i\n", totalMemory );
		
		int totalMemoryUsage = static_cast<int>( mTotalMemoryUsage );
        System::PrintToConsole( "Total Memory: %i\n", totalMemoryUsage );
		fprintf( dumpLog, "Total Memory: %i\n", totalMemoryUsage );

		fclose( dumpLog );
		dumpLog = NULL;
	}

	//===========================================================================
	
	int MemoryLog::GetMemoryAddressHashValue(unsigned int index)
	{
		return (index /16) % MAX_MEMORY_LOG_HASH;
	}
}