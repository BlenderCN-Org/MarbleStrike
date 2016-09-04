#ifndef MEMORY_MEMORY_SBA_HPP
#define MEMORY_MEMORY_SBA_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	class MemorySBA : public IAllocator
	{
	public:
		MemorySBA(size_t blockSize, size_t blockCount, IAllocator* rawMemAllocator);
		virtual ~MemorySBA();

		void* operator new(size_t, void* mem) 
		{ 
			return mem; 
		}

#if __GNUC__
		void operator delete(void*, size_t) { }
#elif _MSC_VER
		void operator delete(void*, void*) { }
#endif

		virtual void* Alloc(size_t size);
		virtual void* AllocAlign(size_t size, size_t align);
		virtual bool Free(void* mem);

		virtual void Validate();

	private:

		bool IsInAllocator(void* mem);

		bool IsInFreeList(void* mem);

		struct SBALink
		{
			SBALink* next;
		};

		IAllocator* mRawMemAllocator;

		void* mSBAMem;
		size_t mBlockSize;
		size_t mBlockCount;

		size_t mUsedBlock;

		SBALink* mFreeList;
		SBALink* mFreeListEndNode;
	};
}

#endif 
