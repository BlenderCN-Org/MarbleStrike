//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/memory/memorylog.hpp"
#include "common/engine/system/memory/memorymanager.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace System
{
#if defined USE_MEMORY_HEAPS
    static char* gMemoryManagerStaticMem[sizeof(MemoryManager)] = { NULL };
    static MemoryManager* gMemoryManagerPtr = NULL;
#endif

    //////static std::mutex gMemoryManagerMutex;	
    static MemoryLog gMemoryLog;
}

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

//void* operator new( std::size_t size )
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//    void *pfs = NULL;
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        pfs = System::gMemoryManagerPtr->Alloc(size);
//    }
//    else
//    {
//        pfs = System::MemoryManager::StaticAlloc(size);
//    }
//
//#if defined ENABLE_MEMORY_LOG
//    System::MemoryData data;
//    data.mAddress = reinterpret_cast< unsigned int >(pfs);
//    System::StringCopy( data.mName, 32, "unnamed" );
//    data.mFile = "unknown_file";
//    data.mLineNumber = 0;
//    data.mSize = static_cast<int>( size );
//    gMemoryLog.Insert( pfs, data );
//#endif
//
//#else
//    pfs = malloc( size );	
//#endif
//
//    return pfs;
//}

//===========================================================================

//void* operator new[]( std::size_t size )
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//    void *pfs = NULL;
//
//#if defined USE_MEMORY_HEAPS		
//    if (System::gMemoryManagerPtr)
//    {
//        pfs = System::gMemoryManagerPtr->Alloc(size);
//    }
//    else
//    {
//        pfs = System::MemoryManager::StaticAlloc(size);
//    }
//
//#if defined ENABLE_MEMORY_LOG
//    System::MemoryData data;
//    data.mAddress = reinterpret_cast< unsigned int >(pfs);
//    System::StringCopy( data.mName, 32, "unnamed" );
//    data.mFile = "unknown_file";
//    data.mLineNumber = 0;
//    data.mSize = static_cast<int>( size );
//
//    gMemoryLog.Insert( pfs, data );
//#endif
//
//#else
//    pfs = malloc( size );	
//#endif
//
//    return pfs;
//}

//===========================================================================

//void operator delete( void* m ) 
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        System::gMemoryManagerPtr->Free(m);
//    }
//    else
//    {
//        System::MemoryManager::StaticFree(m);
//    }
//#else
//    free( m );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::gMemoryLog.Remove( m );
//#endif
//}

//===========================================================================

//void operator delete[]( void* m ) 
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        System::gMemoryManagerPtr->Free(m);
//    }
//    else
//    {
//        System::MemoryManager::StaticFree(m);
//    }
//
//#else
//    free( m );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::gMemoryLog.Remove( m );
//#endif
//}

//===========================================================================

//void *operator new( size_t size, int /*nBlockUse*/, const char* name, int lineNum )
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//    void *pfs = NULL;
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        pfs = System::gMemoryManagerPtr->Alloc(size);
//    }
//    else
//    {
//        pfs = System::MemoryManager::StaticAlloc(size);
//    }
//#else    
//    pfs = malloc( size );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::MemoryData data;
//    data.mAddress = ( unsigned int )pfs;
//    System::StringCopy( data.mName, sizeof(data.mName), name );
//    data.mFile = name;
//    data.mLineNumber = lineNum;
//    data.mSize = static_cast<int>( size );
//
//    System::gMemoryLog.Insert( pfs, data );
//#else
//    UNUSED_ALWAYS( name );
//    UNUSED_ALWAYS( lineNum );
//#endif
//
//    return pfs;
//}

//===========================================================================

//void *operator new( std::size_t size, const char* name, const char* file, int lineNum ) 
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//
//    void *pfs = NULL;
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        pfs = System::gMemoryManagerPtr->Alloc(size);
//    }
//    else
//    {
//        pfs = System::MemoryManager::StaticAlloc(size);
//    }
//#else
//    pfs = malloc( size );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::MemoryData data;
//    data.mAddress = ( unsigned int )pfs;
//    System::StringCopy( data.mName, sizeof(data.mName), name );
//    data.mFile = file;
//    data.mLineNumber = lineNum;
//    data.mSize = static_cast<int>( size );
//
//    System::gMemoryLog.Insert( pfs, data );
//#else
//    UNUSED_ALWAYS( name );
//    UNUSED_ALWAYS( file );	
//    UNUSED_ALWAYS( lineNum );	
//#endif
//
//    return pfs;
//}


//===========================================================================

//void *operator new[]( size_t size, int /*nBlockUse*/, const char* name, int lineNum )
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//
//    void *pfs = NULL;
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        pfs = System::gMemoryManagerPtr->Alloc(size);
//    }
//    else
//    {
//        pfs = System::MemoryManager::StaticAlloc(size);
//    }
//#else
//    pfs = malloc( size );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::MemoryData data;
//    data.mAddress = ( unsigned int )pfs;
//    System::StringCopy( data.mName, sizeof(data.mName), name );
//    data.mFile = name;
//    data.mLineNumber = lineNum;
//    data.mSize = static_cast<int>( size );
//
//    System::gMemoryLog.Insert( pfs, data );
//#else
//    UNUSED_ALWAYS( name );
//    UNUSED_ALWAYS( lineNum );
//#endif
//
//    return pfs;
//}

//===========================================================================

//void *operator new[]( std::size_t size, const char* name, const char* file, int lineNum ) 
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//
//    void *pfs = NULL;
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        pfs = System::gMemoryManagerPtr->Alloc(size);
//    }
//    else
//    {
//        pfs = System::MemoryManager::StaticAlloc(size);
//    }
//#else
//    pfs = malloc( size );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::MemoryData data;
//    data.mAddress = ( unsigned int )pfs;
//    System::StringCopy( data.mName, sizeof(data.mName), name );
//    data.mFile = file;
//    data.mLineNumber = lineNum;
//    data.mSize = static_cast<int>( size );
//
//    System::gMemoryLog.Insert( pfs, data );
//#else
//    UNUSED_ALWAYS( name );
//    UNUSED_ALWAYS( file );
//    UNUSED_ALWAYS( lineNum );
//#endif
//
//    return pfs;
//}

//===========================================================================

//void operator delete( void* m, const char*, const char *, int ) 
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        System::gMemoryManagerPtr->Free(m);
//    }
//    else
//    {
//        System::MemoryManager::StaticFree(m);
//    }
//#else
//    free( m );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::gMemoryLog.Remove( m );
//#endif
//
//}

//===========================================================================

//void operator delete[]( void* m, const char*, const char *, int ) 
//{
//    std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
//
//#if defined USE_MEMORY_HEAPS
//    if (System::gMemoryManagerPtr)
//    {
//        System::gMemoryManagerPtr->Free(m);
//    }
//    else
//    {
//        System::MemoryManager::StaticFree(m);
//    }
//
//#else
//    free( m );
//#endif
//
//#if defined ENABLE_MEMORY_LOG
//    System::gMemoryLog.Remove( m );
//#endif
//
//}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
    void Memory::Create()
    {
#if defined USE_MEMORY_HEAPS
        std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
        System::gMemoryManagerPtr = new(System::gMemoryManagerStaticMem) System::MemoryManager();
        Assert(System::gMemoryManagerPtr != NULL, "MemoryInitialize() failed.");
#endif
    }

    //===========================================================================

    void Memory::Destroy()
    {
#if defined USE_MEMORY_HEAPS
        std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );

        if (System::gMemoryManagerPtr != NULL)
        {
            System::gMemoryManagerPtr->~MemoryManager();
            System::gMemoryManagerPtr = NULL;
            memset(System::gMemoryManagerStaticMem, 0, sizeof(System::gMemoryManagerStaticMem));
        }
#endif
    }


    //===========================================================================

    void Memory::CheckMemoryLog()
    {
#if defined ENABLE_MEMORY_LOG
        //std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
        //System::gMemoryLog.CheckMemoryLog();
#endif
    }

    //===========================================================================

    void Memory::DumpMemoryLog()
    {
#if defined ENABLE_MEMORY_LOG
        //std::lock_guard<std::mutex> lock( System::gMemoryManagerMutex );
        //System::gMemoryLog.DumpMemory();
#endif
    }
}