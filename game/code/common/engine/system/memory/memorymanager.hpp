#if defined PLAT_PC
#ifndef MEMORY_MANAGER_HPP
#define MEMORY_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"
#include "common/engine/system/memory/memorymalloc.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

namespace System
{
	class MemorySBA;
	class MemoryDLMalloc;
}

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{
	class MemoryManager : public IAllocator
	{
	public:
		MemoryManager();
		virtual ~MemoryManager();

		void* operator new(size_t, void* mem) 
		{ 
			return mem; 
		}

#if __GNUC__
		void operator delete(void*, size_t) { }
#elif _MSC_VER
		void operator delete(void*, void*) { }
#endif

		// alloc/free while MemoryManager isn't created yet.
		static void* StaticAlloc(size_t size);
		static void* StaticAllocAlign(size_t size, size_t align);
		static bool StaticFree(void* mem);

		// normal alloc/free
		virtual void* Alloc(size_t size);
		virtual void* AllocAlign(size_t size, size_t align);
		virtual bool Free(void* mem);
		virtual void Validate();

	private:

		void Initialize(
			IAllocator* baseAllocator, 
			size_t s16B, 
			size_t s32B, 
			size_t s64B, 
			size_t s128B, 
			size_t s256B, 
			size_t s512B, 
			size_t largeHeapInByte
			);

		IAllocator* mBaseAllocator;

		MemoryMalloc mMemoryMalloc;

		MemorySBA* mSBA16B;
		MemorySBA* mSBA32B;
		MemorySBA* mSBA64B;
		MemorySBA* mSBA128B;
		MemorySBA* mSBA256B;
		MemorySBA* mSBA512B;
		MemoryDLMalloc* mDLMalloc;
		void* mDLMallocData;
	};
}

#endif 

#endif