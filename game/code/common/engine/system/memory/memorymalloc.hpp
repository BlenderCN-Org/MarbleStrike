#ifndef MEMORY_MEMORY_MALLOC_HPP
#define MEMORY_MEMORY_MALLOC_HPP

#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	class MemoryMalloc : public IAllocator
	{
	public:

		MemoryMalloc();
		virtual ~MemoryMalloc();

		virtual void* Alloc( size_t size );
		virtual void* AllocAlign( size_t size, size_t align );
		virtual bool Free( void* mem );
		virtual void Validate();
	};
}

#endif 

#endif