#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory/memorymalloc.hpp"
#include "common/engine/system/memory/memorysba.hpp"
#include "common/engine/system/memory/memorydlmalloc.hpp"
#include "common/engine/system/memory/memorylog.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/timer.hpp"

#include <time.h>
#include <memory.h>


namespace System
{  
    const int RAND_MEM_TEST_SIZE = 10000;
    static char* sRandMemTestList[RAND_MEM_TEST_SIZE] = { NULL };
    static size_t sRandMemTestSize[RAND_MEM_TEST_SIZE] = { 0 };

    // unittest for MemorySBA class
    void UnitTest_Memory_MemorySBA()
    {
        MemoryMalloc memMalloc;

        const int BLOCK_COUNT = 1024;
        const int BLOCK_SIZE = 16;
        void* mem[BLOCK_COUNT] = { NULL };
        int memIndex = 0;


        MemorySBA memSBA(BLOCK_SIZE, BLOCK_COUNT, &memMalloc);

        for (int i = 0; i < 100; i++)
        {
            mem[memIndex] = memSBA.Alloc(BLOCK_SIZE);
            Assert( mem[memIndex] != NULL, "Memory unit test failed.");
            memIndex++;
        }

        for (int i = 0; i < 10; i++)
        {
            memSBA.Free(mem[i]);
            mem[i] = NULL;
        }

        for (int i = 0; i < 100; i++)
        {
            mem[memIndex] = memSBA.Alloc(BLOCK_SIZE);
            Assert( mem[memIndex] != NULL, "Memory unit test failed.");
            memIndex++;
        }

        for (int i = 20; i < 30; i++)
        {
            memSBA.Free(mem[i]);
            mem[i] = NULL;
        }

        for (int i = 100; i < 120; i++)
        {
            memSBA.Free(mem[i]);
            mem[i] = NULL;
        }

        for (int i = 0; i < memIndex; i++)
        {
            memSBA.Free(mem[i]);
            mem[i] = NULL;
        }
    }

    void UnitTest_Memory_MemoryDLMalloc()
    {
        srand( (unsigned)time( NULL ) );
        size_t memBlockSize = 1024*1024 + MemoryDLMalloc::DLHEAP_HEADER_SIZE;
        void* memBlock = malloc(memBlockSize);
        {
            memset(sRandMemTestList, 0, sizeof(sRandMemTestList));

            MemoryDLMalloc memDLMalloc(memBlock, memBlockSize);
            size_t totalMemSize = 0;
            for (int i = 0; i < 100; i++)
            {
                size_t memSize = rand() % (10 * 1024);
                sRandMemTestList[i] = reinterpret_cast<char*>(memDLMalloc.Alloc(memSize));
                Assert( sRandMemTestList[i] != NULL, "Memory unit test failed.");
                totalMemSize += memSize;
            }

            for (int i = 0; i < 100; i++)
            {
                memDLMalloc.Free(sRandMemTestList[i]);
            }

            // try to allocate entire heap, this will return non-NULL ptr.
            void* mem = NULL;
            mem = memDLMalloc.Alloc(1024*1024);
            Assert( mem != NULL, "Memory unit test failed.");
            memDLMalloc.Free(mem);
            mem = NULL;

            // try to allocate larger than heap, this will return NULL ptr;
            mem = memDLMalloc.Alloc(2024*1024);
            Assert( mem == NULL, "Memory unit test failed.");
        }

        free(memBlock);
    }


    // unittest for operator new and delete
    void UnitTest_Memory_NewDelete()
    {
  //      size_t memUsedStarted = MemoryLog::GetInstance()->GetMemoryUsed();
		//UNUSED_ALWAYS( memUsedStarted );
  //      size_t memUsed = 0;
  //      for (int i = 0; i < 2048; i++)
  //      {
  //          char* mem = new("Unit Test", __FILE__, __LINE__) char[i];
  //          memUsed = MemoryLog::GetInstance()->GetMemoryUsed();
  //          delete mem;
  //          memUsed = MemoryLog::GetInstance()->GetMemoryUsed();
  //      }
  //      Assert(MemoryLog::GetInstance()->GetMemoryUsed() == memUsedStarted, "Memory unit test failed.");
    }


    
    void UnitTest_Memory_Performance()
    {
        memset(sRandMemTestList, 0, sizeof(sRandMemTestList));

		Memory::Create();

        //
        // SBA performance testing
        //
        for (int i = 0; i < RAND_MEM_TEST_SIZE; i++)
        {
            sRandMemTestSize[i] = rand() % (2048);
        }
        static double sbaRuntime = 0.0;
        {
        System::Timer timer;
        timer.Start();

        double startTime = timer.GetTime();

        for (int c = 0; c < 100; c++)
        {
            for (int i = 0; i < 1000; i++)
            {
                sRandMemTestList[i] = new char[sRandMemTestSize[i]];
            }

            for (int i = 0; i < 1000; i++)
            {
                delete[] sRandMemTestList[i];
            }
        }
        double stopTime = timer.GetTime();

        sbaRuntime = stopTime - startTime;
        }

        static double mallocRuntime = 0.0;
        {
        System::Timer timer;
        timer.Start();

        double startTime = timer.GetTime();

        for (int c = 0; c < 100; c++)
        {
            for (int i = 0; i < 1000; i++)
            {
                sRandMemTestList[i] = reinterpret_cast<char*>(malloc(sRandMemTestSize[i]));
            }

            for (int i = 0; i < 1000; i++)
            {
                free(sRandMemTestList[i]);
            }
        }
        double stopTime = timer.GetTime();

        mallocRuntime = stopTime - startTime;
        }


		Memory::Destroy();
    }

    void UnitTest_Memory()
    {
        UnitTest_Memory_Performance();

        UnitTest_Memory_MemorySBA();

        UnitTest_Memory_MemoryDLMalloc();

        UnitTest_Memory_NewDelete();
    }
}

extern "C" void RunUnitTest()
{
	//System::UnitTest_Memory();
}

#endif