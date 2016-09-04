//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <memory.h>
#include "common/engine/system/memory/memorysba.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//#define ENABLE_MEMORY_ALLOCATOR_VALIDATE 1

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	MemorySBA::MemorySBA(size_t blockSize, size_t blockCount, IAllocator* rawMemAllocator) 
		:	mRawMemAllocator(rawMemAllocator)
		,	mSBAMem(NULL)
		,	mBlockSize(blockSize)
		,	mBlockCount(blockCount)
		,	mUsedBlock(0)
		,	mFreeList(NULL)
		,	mFreeListEndNode(NULL)
	{
		Assert(mRawMemAllocator != NULL, "mRawMemAllocator must not be NULL.");
		Assert(mBlockSize % MEMORY_DEFAULT_ALIGNMENT == 0, "mBlockSize must be multiple of MEMORY_DEFAULT_ALIGNMENT.");
		Assert(mBlockCount % 16 == 0, "mBlockSize must be multiple of 16.");

		mSBAMem = mRawMemAllocator->AllocAlign(mBlockSize * mBlockCount, MEMORY_DEFAULT_ALIGNMENT);
		Assert(mSBAMem != NULL, "mSBAMem must not be NULL.");

		char* currBlock = static_cast<char*>(mSBAMem);
		mFreeList = reinterpret_cast<SBALink*>(currBlock);

		SBALink* currLink = NULL;
		for (size_t i = 0; i < mBlockCount; i++)
		{
			currLink = reinterpret_cast<SBALink*>(currBlock);
			currBlock += mBlockSize;
			currLink->next = reinterpret_cast<SBALink*>(currBlock);
		}

		currLink->next = NULL;
		mFreeListEndNode = currLink;

#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif
	}

	//===========================================================================

	MemorySBA::~MemorySBA()
	{
#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif

		Assert(mUsedBlock == 0, "Deleting MemorySBA while memory inside still not freed.");

#ifdef _DEBUG
		memset(mSBAMem, 0xff, mBlockSize * mBlockCount);
#endif
		mRawMemAllocator->Free(mSBAMem); mSBAMem = NULL;
		mRawMemAllocator = NULL;

		mFreeList = NULL;
		mFreeListEndNode = NULL;
		mUsedBlock = 0;
		mBlockSize = 0;
		mBlockCount = 0;
	}

	//===========================================================================

	void* MemorySBA::Alloc(size_t size)
	{
#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif

		void* memret = NULL;

		if (size <= mBlockSize)
		{
			if (mFreeList != NULL)
			{
				memret = mFreeList;
				Assert(mFreeList->next == NULL || *((unsigned int*)&(mFreeList->next)) >= *((unsigned int*)&mSBAMem), "Invalid ptr.");
				mFreeList = mFreeList->next;
				mUsedBlock++;

				if (mFreeList == NULL)
				{
					mFreeListEndNode = NULL;
				}
			}
			else
			{
				// no more memory left
				Assert(mFreeListEndNode == NULL, "Consistency check failed.");
			}
		}

#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif
		return memret;
	}

	//===========================================================================

	void* MemorySBA::AllocAlign(size_t size, size_t align)
	{
#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif
		void* memret = NULL;

		if ((MEMORY_DEFAULT_ALIGNMENT >= align && (MEMORY_DEFAULT_ALIGNMENT % align) == 0))
		{
			memret = Alloc(size);
		}
		else
		{
			// alignment for this value is not supported.
			memret = NULL;
		}
#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif
		return memret;
	}

	//===========================================================================

	bool MemorySBA::Free(void* mem)
	{
#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif

		bool freedHere = false;

		if (IsInAllocator(mem))
		{
			char* memblock = reinterpret_cast<char*>(( *((unsigned int*)&mem) - *((unsigned int*)&mSBAMem) ) / mBlockSize * mBlockSize + *((unsigned int*)&mSBAMem));

#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
			Assert(IsInFreeList(mem) == false, "memory block is already freed, attempt double free.");
#endif

			if (mFreeList == NULL)
			{
				Assert(mFreeListEndNode == NULL, "Consistency check failed.");
				mFreeList = reinterpret_cast<SBALink*>(memblock);
				mFreeList->next = NULL;
				mFreeListEndNode = mFreeList;
			}
			else
			{
				Assert(mFreeListEndNode != NULL, "Consistency check failed.");
				Assert(mFreeListEndNode->next == NULL, "Consistency check failed.");
				mFreeListEndNode->next = reinterpret_cast<SBALink*>(memblock);
				mFreeListEndNode = reinterpret_cast<SBALink*>(memblock);
				mFreeListEndNode->next = NULL;
			}
			mUsedBlock--;
			freedHere = true;
		}

#if ENABLE_MEMORY_ALLOCATOR_VALIDATE
		Validate();
#endif
		return freedHere;
	}

	//===========================================================================

	bool MemorySBA::IsInAllocator(void* mem)
	{
        unsigned int memInt = *((unsigned int*)&mem);
        unsigned int sbaInt = *((unsigned int*)&mSBAMem);
		if (
			memInt >= sbaInt &&
			memInt < (sbaInt + mBlockSize * (mBlockCount + 1))
			)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//===========================================================================

	bool MemorySBA::IsInFreeList(void* mem)
	{
        unsigned int memInt = *((unsigned int*)&mem);
        SBALink* currFree = mFreeList;
        unsigned int currFreeInt = *((unsigned int*)&currFree);
		while(currFree != NULL)
		{
			if (
				memInt >= currFreeInt &&
				memInt < (currFreeInt + mBlockSize)
				)
			{
				return true;
			}
			currFree = currFree->next;
		}
		return false;
	}

	//===========================================================================

	void MemorySBA::Validate()
	{
		size_t freeCount = 0;
		if (mFreeList != NULL)
		{
			SBALink* currFree = mFreeList;
			SBALink* lastFree = mFreeList;

			while(currFree != NULL)
			{
				freeCount++;
				currFree = currFree->next;

				if (currFree != NULL && !IsInAllocator(currFree))
				{
					Assert(false, "MemorySBA::Validate() failed, SBALink list is invalide.");
					return;
				}

				if (currFree != NULL)
				{
					lastFree = currFree;
				}
			}

			Assert(mFreeListEndNode == lastFree, "MemorySBA::Validate() failed, mFreeListEndNode is not what expected.");
		}
		else
		{
			Assert(mFreeListEndNode == NULL, "MemorySBA::Validate() failed, mFreeListEndNode is not what expected.");
		}

		Assert(mBlockCount == (freeCount + mUsedBlock), "MemorySBA::Validate() failed, free count + used count is not same as total block count");
	}
}
