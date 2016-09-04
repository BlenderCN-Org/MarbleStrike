#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory/memorydlmalloc.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory/dlmalloc.h"

#if defined PLAT_PC
#include <crtdbg.h>
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{  

	MemoryDLMalloc::MemoryDLMalloc(void* mem, size_t memSize) 
		:	mMemory(mem)
		,	mMemSize(memSize)
		,	mDLMemSpace(NULL)
	{
		mDLMemSpace = create_mspace_with_base(mem, memSize, 0);
		Assert(mDLMemSpace != NULL, "create_mspace_with_base() failed.")
	}

	//===========================================================================

	MemoryDLMalloc::~MemoryDLMalloc()
	{
		destroy_mspace(mDLMemSpace);
	}

	//===========================================================================

	void* MemoryDLMalloc::Alloc(size_t size)
	{
		return mspace_memalign(mDLMemSpace, MEMORY_DEFAULT_ALIGNMENT, size);
	}

	//===========================================================================

	void* MemoryDLMalloc::AllocAlign(size_t size, size_t align)
	{
		return mspace_memalign(mDLMemSpace, align, size);
	}

	//===========================================================================

	bool MemoryDLMalloc::Free(void* mem)
	{
		if (reinterpret_cast<size_t>(mem) >= reinterpret_cast<size_t>(mDLMemSpace) && reinterpret_cast<size_t>(mem) < reinterpret_cast<size_t>(mDLMemSpace) + mMemSize)
		{
			mspace_free(mDLMemSpace, mem);
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//===========================================================================

	void MemoryDLMalloc::Validate()
	{
	}
}


#endif