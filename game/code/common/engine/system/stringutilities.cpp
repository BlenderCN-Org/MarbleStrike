//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/system/stringutilities.hpp"
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace System
{	

	//===========================================================================

	void StringCopy( char* dst, int dstSize, const char* src )
	{
		int lengthSrc = static_cast<int>( strlen( src ) );
		UNUSED_ALWAYS( lengthSrc );
		Assert( dstSize >= lengthSrc + 1, "" );
#if defined ( PLAT_PC ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		strcpy_s( dst, dstSize, src );
#else
		strcpy( dst, src );
#endif
	}

	//===========================================================================

	void StringNCopy( char* dst, int dstSize, const char* src, int numToCopy )
	{
		Assert( dstSize >= numToCopy, "" );
		
#if defined ( PLAT_PC ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		strncpy_s( dst, dstSize, src, numToCopy );
#else
		strncpy( dst, src, numToCopy );
#endif
	}

	//===========================================================================

	void StringConcat( char* dst, int dstSize, const char* src )
	{
		int lengthDst = static_cast<int>( strlen( dst ) ) ;
		int lengthSrc = static_cast<int>( strlen( src) ) ;
		int totalLength = lengthDst + lengthSrc + 1;
		UNUSED_ALWAYS( totalLength );
		Assert( totalLength <= dstSize, "" );
		
#if defined ( PLAT_PC ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		strcat_s( dst, dstSize, src );
#else
		strcat( dst, src );
#endif
	}

	//===========================================================================

	char* StringToken( char* str, const char* delim, char** nextToken )
	{
#if defined ( PLAT_PC ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		return strtok_s( str, delim, nextToken );
#else
		return strtok( str, delim );
#endif
	}

	//===========================================================================

	int StringICmp( const char* str1, const char* str2 )
	{
#if defined ( PLAT_PC ) || defined ( PLAT_XBOX360 ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		return _stricmp( str1, str2 );
#endif

#if defined( PLAT_PS3 ) || defined( PLAT_IOS ) || defined( PLAT_ANDROID )
		return strcasecmp( str1, str2 );
#endif
	}

	//===========================================================================

	int StringHash( const char* str )
	{
		int hash = 5381;
		int c = *str++;

		while ( c )
		{
			hash = ( ( hash << 5 ) + hash ) + c; /* hash * 33 + c */
            c = *str++;
		}

		return hash;
	}

	//===========================================================================

	void ConvertToWChar( const char* input, wchar_t* output, size_t outputSize )
	{		
		const size_t inputSize = strlen( input ) + 1;		

		int lengthSrc = static_cast<int>( strlen( input ) ) ;
		UNUSED_ALWAYS( lengthSrc );
		Assert( outputSize > inputSize, "" );
		
#if defined PLAT_IOS || defined PLAT_ANDROID
        UNUSED_ALWAYS( outputSize );
        mbstowcs( output, input, inputSize );
#else
        size_t returnChar = 0;
		mbstowcs_s( &returnChar, output, outputSize, input, inputSize );
#endif
	}
}

