#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory/memorymanager.hpp"
#include "common/engine/system/memory/memorymalloc.hpp"
#include "common/engine/system/memory/memorysba.hpp"
#include "common/engine/system/memory/memorydlmalloc.hpp"
#include "common/engine/system/assert.hpp"

#include <stdio.h>
#include <string.h>

#if defined PLAT_PC
#include <memory.h>
#include <crtdbg.h>
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace System
{
	const int sStaticDLMallocHeapSize = 1024 * 1024;
	static char sStaticDLMallocHeap[sStaticDLMallocHeapSize + MemoryDLMalloc::DLHEAP_HEADER_SIZE];
	static char sStaticDLMallocObj[sizeof(MemoryDLMalloc)];
	MemoryDLMalloc* sStaticDLMallocPtr = NULL;
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	void InitStaticDLMalloc()
	{
		if (sStaticDLMallocPtr == NULL)
		{
			sStaticDLMallocPtr = new(sStaticDLMallocObj) MemoryDLMalloc(sStaticDLMallocHeap, sStaticDLMallocHeapSize);
		}
	}

	//===========================================================================

	void* MemoryManager::StaticAlloc(size_t size)
	{
		InitStaticDLMalloc();
		return sStaticDLMallocPtr->Alloc(size);
	}

	//===========================================================================

	void* MemoryManager::StaticAllocAlign(size_t size, size_t align)
	{
		InitStaticDLMalloc();
		return sStaticDLMallocPtr->AllocAlign(size, align);
	}

	//===========================================================================

	bool MemoryManager::StaticFree(void* mem)
	{
		InitStaticDLMalloc();
		return sStaticDLMallocPtr->Free(mem);
	}

	//===========================================================================

	MemoryManager::MemoryManager() 
		:	mBaseAllocator(NULL)
		,	mSBA16B(NULL)
		,	mSBA32B(NULL)
		,	mSBA64B(NULL)
		,	mSBA128B(NULL)
		,	mSBA256B(NULL)
		,	mSBA512B(NULL)
		,	mDLMalloc(NULL)
		,	mDLMallocData(NULL)
	{
        int totalMemory = 24;
#if defined PLAT_PC
        totalMemory = 256;
#endif
		Initialize( &mMemoryMalloc, 32*1024, 16*1024, 8*1024, 4*1024, 2*1024, 1024, totalMemory * 1024 * 1024);
	}

	//===========================================================================

	MemoryManager::~MemoryManager()
	{
		Assert(mBaseAllocator != NULL, "mBaseAllocator cannot be NULL.");
		if (mSBA16B != NULL)
		{
			mSBA16B->~MemorySBA();
			mBaseAllocator->Free(mSBA16B);
			mSBA16B = NULL;
		}

		if (mSBA32B != NULL)
		{
			mSBA32B->~MemorySBA();
			mBaseAllocator->Free(mSBA32B);
			mSBA32B = NULL;
		}

		if (mSBA64B != NULL)
		{
			mSBA64B->~MemorySBA();
			mBaseAllocator->Free(mSBA64B);
			mSBA64B = NULL;
		}

		if (mSBA128B != NULL)
		{
			mSBA128B->~MemorySBA();
			mBaseAllocator->Free(mSBA128B);
			mSBA128B = NULL;
		}

		if (mSBA256B != NULL)
		{
			mSBA256B->~MemorySBA();
			mBaseAllocator->Free(mSBA256B);
			mSBA256B = NULL;
		}

		if (mSBA512B != NULL)
		{
			mSBA512B->~MemorySBA();
			mBaseAllocator->Free(mSBA512B);
			mSBA512B = NULL;
		}

		if (mDLMalloc != NULL)
		{
			mDLMalloc->~MemoryDLMalloc();
			mBaseAllocator->Free(mDLMalloc);
			mBaseAllocator->Free(mDLMallocData);
			mDLMalloc = NULL;
			mDLMallocData = NULL;
		}
	}

	//===========================================================================

	void MemoryManager::Initialize(
		IAllocator* baseAllocator, 
		size_t s16B, 
		size_t s32B, 
		size_t s64B, 
		size_t s128B, 
		size_t s256B, 
		size_t s512B, 
		size_t largeHeapInByte
		)
	{
		mBaseAllocator = baseAllocator;
		void* mem = NULL;

		mem = mBaseAllocator->Alloc(sizeof(MemorySBA));
		mSBA16B = new(mem) MemorySBA(16, s16B, baseAllocator);

		mem = mBaseAllocator->Alloc(sizeof(MemorySBA));
		mSBA32B = new(mem) MemorySBA(32, s32B, baseAllocator);

		mem = mBaseAllocator->Alloc(sizeof(MemorySBA));
		mSBA64B = new(mem) MemorySBA(64, s64B, baseAllocator);

		mem = mBaseAllocator->Alloc(sizeof(MemorySBA));
		mSBA128B = new(mem) MemorySBA(128, s128B, baseAllocator);

		mem = mBaseAllocator->Alloc(sizeof(MemorySBA));
		mSBA256B = new(mem) MemorySBA(256, s256B, baseAllocator);

		mem = mBaseAllocator->Alloc(sizeof(MemorySBA));
		mSBA512B = new(mem) MemorySBA(512, s512B, baseAllocator);

		mem = mBaseAllocator->Alloc(sizeof(MemoryDLMalloc));
		size_t heapMemSize = largeHeapInByte + MemoryDLMalloc::DLHEAP_HEADER_SIZE;
		mDLMallocData = mBaseAllocator->AllocAlign(heapMemSize, MEMORY_DEFAULT_ALIGNMENT);
		mDLMalloc = new(mem) MemoryDLMalloc(mDLMallocData, heapMemSize);
	}

	//===========================================================================

	void* MemoryManager::Alloc(size_t size)
	{
		void* mem = NULL;
		if (size <= 16)
		{
			mem = mSBA16B->Alloc(size);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 32)
		{
			mem = mSBA32B->Alloc(size);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 64)
		{
			mem = mSBA64B->Alloc(size);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 128)
		{
			mem = mSBA128B->Alloc(size);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 256)
		{
			mem = mSBA256B->Alloc(size);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 512)
		{
			mem = mSBA512B->Alloc(size);
			if (mem != NULL)
			{
				return mem;
			}
		}

		mem = mDLMalloc->Alloc(size);
		Assert(mem != NULL, "Not enough memory reserved in the MemoryManager.");

		if (mem == NULL)
		{
			mem = mBaseAllocator->Alloc(size);
		}

		Assert(mem != NULL, "MemoryManager: not enough reserved memory.");
		return mem;
	}

	//===========================================================================

	void* MemoryManager::AllocAlign(size_t size, size_t align)
	{
		void* mem = NULL;
		if (size <= 16)
		{
			mem = mSBA16B->AllocAlign(size, align);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 32)
		{
			mem = mSBA32B->AllocAlign(size, align);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 64)
		{
			mem = mSBA64B->AllocAlign(size, align);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 128)
		{
			mem = mSBA128B->AllocAlign(size, align);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 256)
		{
			mem = mSBA256B->AllocAlign(size, align);
			if (mem != NULL)
			{
				return mem;
			}
		}

		if (size <= 512)
		{
			mem = mSBA512B->AllocAlign(size, align);
			if (mem != NULL)
			{
				return mem;
			}
		}

		mem = mDLMalloc->AllocAlign(size, align);
		Assert(mem != NULL, "Not enough memory reserved in the MemoryManager.");

		if (mem == NULL)
		{
			mem = mBaseAllocator->AllocAlign(size, align);
		}

		Assert(mem != NULL, "MemoryManager: not enough reserved memory.");
		return mem;
	}

	//===========================================================================

	bool MemoryManager::Free(void* mem)
	{
		if (mSBA16B->Free(mem))
		{
			return true;
		}

		if (mSBA32B->Free(mem))
		{
			return true;
		}

		if (mSBA64B->Free(mem))
		{
			return true;
		}

		if (mSBA128B->Free(mem))
		{
			return true;
		}

		if (mSBA256B->Free(mem))
		{
			return true;
		}

		if (mSBA512B->Free(mem))
		{
			return true;
		}

		if (mDLMalloc->Free(mem))
		{
			return true;
		}

		if (StaticFree(mem))
		{
			return true;
		}

		return mBaseAllocator->Free(mem);
	}

	//===========================================================================

	void MemoryManager::Validate()
	{
		mSBA16B->Validate();
		mSBA32B->Validate();
		mSBA64B->Validate();
		mSBA128B->Validate();
		mSBA256B->Validate();
		mSBA512B->Validate();
		mDLMalloc->Validate();
	}
}

#endif