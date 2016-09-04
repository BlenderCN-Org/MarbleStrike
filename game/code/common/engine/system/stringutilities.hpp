#ifndef STRING_UTILITIES_HPP
#define STRING_UTILITIES_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <windows.h>
#endif

#include <stdlib.h>

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace System
{
	void StringCopy( char* dst, int dstSize, const char* src );
	void StringNCopy( char* dst, int dstSize, const char* src, int numToCopy );
	void StringConcat( char* dst, int dstSize, const char* src );
	char* StringToken( char* str, const char* delim, char** nextToken );
	int StringICmp( const char* str1, const char* str2 );
	int StringHash( const char* str );
	void ConvertToWChar( const char* input, wchar_t* output, size_t outputSize );
}

#endif
