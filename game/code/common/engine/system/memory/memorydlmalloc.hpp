#if defined PLAT_PC

#ifndef MEMORY_MEMORY_DLMALLOC_HPP
#define MEMORY_MEMORY_DLMALLOC_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{
	class MemoryDLMalloc : public IAllocator
	{
	public:

		static const int DLHEAP_HEADER_SIZE = 128 * sizeof(size_t) + 256;

		MemoryDLMalloc(void* mem, size_t memSize);
		virtual ~MemoryDLMalloc();

		void* operator new(size_t, void* mem) 
		{ 
			return mem; 
		}

#if __GNUC__
		void operator delete(void*, size_t) { }
#elif _MSC_VER
		void operator delete(void*, void*) { }
#endif
		virtual void* Alloc( size_t size );
		virtual void* AllocAlign( size_t size, size_t align );
		virtual bool Free( void* mem );
		virtual void Validate();

	private:

		void* mMemory;
		size_t mMemSize;
		void* mDLMemSpace;
	};
}

#endif

#endif