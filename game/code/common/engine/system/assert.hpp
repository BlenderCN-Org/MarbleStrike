#ifndef ASSERT_HPP
#define ASSERT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <windows.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <wchar.h>
#include "common/engine/system/systemdefines.hpp"
#include "common/engine/system/stringutilities.hpp"

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

#if defined ( _DEBUG )
	inline bool CustomAssertFunction( bool exp, const char* description, int line, const char* filename )
	{
		if ( exp == false )
		{    
			wchar_t stringBuffer[1024];

			wchar_t filenameWCHAR[256];
			System::ConvertToWChar( filename, filenameWCHAR, 256 );

			wchar_t descriptionWCHAR[256];
			System::ConvertToWChar( description, descriptionWCHAR, 256 );
			
			swprintf( stringBuffer, 1024, L"%s: %i - %s\n", descriptionWCHAR, line, filenameWCHAR );

	#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			OutputDebugStringW( stringBuffer );
	#endif

	#if defined PLAT_PS3 || defined PLAT_IOS
			wprintf( L"%s", stringBuffer );
	#endif
			return true;
		}
		else
		{
			return false;
		}
	}

	#if defined PLAT_PC
		#define Assert( exp, description ) \
			if ( CustomAssertFunction( (int)(exp), description, __LINE__, \
			__FILE__ ) )\
		{ _asm { int 3 } }

	#elif defined PLAT_IOS
		#define Assert( exp, description ) \
		if ( CustomAssertFunction( (bool)(exp), description, __LINE__, \
		__FILE__ ) )\
		{ assert( false ); }
	#else

		#if defined PLAT_PS3

			#define Assert( exp, description )\
				if ( CustomAssertFunction( (int)(exp), description, __LINE__, __FILE__ ) )\
			{ int* null = 0; *null = 0; }

		#else

			#define Assert( exp, description )\
				if ( CustomAssertFunction( (int)(exp), description, __LINE__, __FILE__ ) )\
			{ assert( false ); }

		#endif

	#endif

#else

	#define Assert( exp, description )

#endif

#endif
