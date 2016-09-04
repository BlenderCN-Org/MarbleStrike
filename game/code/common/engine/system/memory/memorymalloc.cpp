//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC

#include <stdlib.h>
#include "common/engine/system/memory/memorymalloc.hpp"
#include "common/engine/system/systemdefines.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{  
	MemoryMalloc::MemoryMalloc()
	{
	}

	//===========================================================================

	MemoryMalloc::~MemoryMalloc()
	{
	}

	//===========================================================================

	void* MemoryMalloc::Alloc(size_t size)
	{
#if PLAT_IOS
		void* mem = NULL;
		int ret = posix_memalign(&mem, MEMORY_DEFAULT_ALIGNMENT, size);
		UNUSED_ALWAYS( ret );
		return mem;
#elif PLAT_PC
		return _aligned_malloc(size, MEMORY_DEFAULT_ALIGNMENT);
#endif
	}

	//===========================================================================

	void* MemoryMalloc::AllocAlign(size_t size, size_t align)
	{
		if (align <= MEMORY_DEFAULT_ALIGNMENT)
		{
#if PLAT_IOS
			void* mem = NULL;
			int ret = posix_memalign(&mem, MEMORY_DEFAULT_ALIGNMENT, size);
			UNUSED_ALWAYS( ret );
			return mem;
#elif PLAT_PC
			return _aligned_malloc(size, MEMORY_DEFAULT_ALIGNMENT);
#endif
		}
		else
		{
#if PLAT_IOS
			void* mem = NULL;
			int ret = posix_memalign(&mem, align, size);
			UNUSED_ALWAYS( ret );
			return mem;
#elif PLAT_PC
			return _aligned_malloc(size, align);
#endif
		}
	}

	//===========================================================================

	bool MemoryMalloc::Free(void* mem)
	{
#if PLAT_IOS
		free(mem);
#elif PLAT_PC
		_aligned_free(mem);
#endif
		return true;
	}

	//===========================================================================

	void MemoryMalloc::Validate()
	{
	}
}

#endif