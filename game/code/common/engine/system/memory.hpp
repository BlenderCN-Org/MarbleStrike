#ifndef MEMORY_HPP
#define MEMORY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//#defined USE_MEMORY_HEAPS

#include <new>

//void* operator new( size_t, const char*, const char *, int );
//void* operator new[]( std::size_t, const char*, const char *, int );

//void* operator new( size_t, int , const char*, int );
//void* operator new[]( size_t, int , const char*, int );

//void operator delete( void*, const char*, const char *, int );
//void operator delete[]( void*, const char*, const char *, int );

#include <stdio.h>
#include <stdarg.h>

#if defined PLAT_PC
#include <windows.h>
#endif

#include <exception>
//#include <mutex>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

#if defined _DEBUG
//#define ENABLE_MEMORY_LOG
#endif

#if defined _DEBUG
//#define ENABLE_MEMORY_ALLOCATOR_VALIDATE 1
#endif

//////////////////////////////////////////////////////
// MACROS
//////////////////////////////////////////////////////

#if defined ENABLE_MEMORY_LOG
#define NEW_PTR( name ) new( name, __FILE__, __LINE__ )
#else
#define NEW_PTR( name ) new
#endif

#define DELETE_PTR( ptr )				\
{										\
	delete ptr;							\
	ptr = NULL;							\
}

#define DELETE_PTR_ARRAY( ptr )			\
{										\
	delete [] ptr;						\
	ptr = NULL;							\
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

namespace System
{
	const int MEMORY_DEFAULT_ALIGNMENT = 16;    // set all alignment default to 16 bytes
}

//////////////////////////////////////////////////////
// PUBLIC INTERFACE
//////////////////////////////////////////////////////

namespace System
{
	class Memory
	{
	public:
		static void Create();
		static void Destroy();

		static void CheckMemoryLog();
		static void DumpMemoryLog();
	};	

	class IAllocator
	{
	public:
		virtual ~IAllocator() {};
		virtual void* Alloc(size_t size) = 0;
		virtual void* AllocAlign(size_t size, size_t align) = 0;
		virtual bool Free(void* mem) = 0;
		virtual void Validate() = 0;
	};
}

#endif
