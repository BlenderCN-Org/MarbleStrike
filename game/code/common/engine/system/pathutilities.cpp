//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/pathutilities.hpp"
#include "common/engine/database/database.hpp"
#include <stdlib.h>
#include <string>

#if defined PLAT_IOS || defined PLAT_ANDROID
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#endif

#if defined PLAT_PS3
#include <sys/paths.h>
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{	
	namespace Path
	{

		System::FixedString<256> SystemSlash( const char* path )
		{		
#if defined( PLAT_IOS ) || defined( PLAT_ANDROID )
			System::FixedString<256> slashString = path;
			slashString.ReplaceAll( "\\", "/" );

			return slashString;
#else
			return path;
#endif
		}

		//===========================================================================

//		void GetSystemPath( char* path, int size )
//		{
//			System::StringCopy( path, size, "" );
//
//#if defined PLAT_XBOX360
//			System::StringCopy( path, size, "game:\\" );	
//#endif
//
//#if defined PLAT_PS3
//			System::StringCopy( path, size, SYS_APP_HOME );
//			System::StringConcat( path, size, "/" );
//#endif
//
//#if defined PLAT_IOS
//			System::StringCopy( path, size, Database::Get()->GetResourcePath() );
//#endif
//		}

		//===========================================================================

		void GetFilename( const char* path, char* output, int size )
		{
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			//char exedir[_MAX_PATH];
			//char drive[_MAX_DRIVE];
			//char path[_MAX_PATH];
			char fname[_MAX_FNAME];
			char ext[_MAX_EXT];

			::_splitpath_s( path, NULL, NULL, NULL, NULL, fname, _MAX_FNAME, ext, _MAX_EXT );
			//::_splitpath_s( path, drive, _MAX_DRIVE, path, _MAX_PATH, fname, _MAX_FNAME, ext, _MAX_EXT );

			System::StringCopy( output, size, fname );
			System::StringConcat( output, size, ext );
#else
            std::string myString( path );
            size_t slashIndex = myString.find_last_of( '/' );
            if ( slashIndex == std::string::npos )
            {
                slashIndex = myString.find_last_of( '\\' );
                if ( slashIndex == std::string::npos )
                {
                    slashIndex = 0;
                }
            }
            std::string filename = myString.substr( slashIndex + 1, myString.length() - ( slashIndex + 1 ) );
            System::StringCopy( output, size, filename.c_str() );
#endif
		}
		
		//===========================================================================

		void GetFilenameNoExt( const char* path, char* output, int size )
		{
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			char fname[_MAX_FNAME];

			::_splitpath_s( path, NULL, NULL, NULL, NULL, fname, _MAX_FNAME, NULL, NULL );

			System::StringCopy( output, size, fname );		
#else
            std::string myString( path );
            size_t dotIndex = myString.find_last_of( '.' );
            size_t slashIndex = myString.find_last_of( '/' );
            if ( slashIndex == std::string::npos )
            {
                slashIndex = myString.find_last_of( '\\' );
                if ( slashIndex == std::string::npos )
                {
                    slashIndex = 0;
                }
            }
            
            if ( dotIndex > slashIndex )
            {
                std::string filenameNoExt = myString.substr( slashIndex + 1, dotIndex - slashIndex - 1 );
                System::StringCopy( output, size, filenameNoExt.c_str() );
            }
            else
            {
                System::StringCopy( output, size, "" );
            }
#endif
		}
		
		//===========================================================================

		void GetExt( const char* path, char* output, int size )
		{
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			char ext[_MAX_EXT];

			::_splitpath_s( path, NULL, NULL, NULL, NULL, NULL, NULL, ext, _MAX_EXT );

			System::StringCopy( output, size, ext );
#else
            std::string myString( path );
            size_t dotIndex = myString.find_last_of( '.' );
            std::string extension = myString.substr( dotIndex, myString.length() - ( dotIndex ) );
            System::StringCopy( output, size, extension.c_str() );
            
#endif
		}
		
		//===========================================================================

		void GetFilePath( const char* path, char* output, int size )
		{
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			char filepath[_MAX_PATH];			
			::_splitpath_s( path, NULL, NULL, filepath, _MAX_PATH, NULL, NULL, NULL, NULL );

			System::StringCopy( output, size, filepath );
#else
			std::string myString( path );
            
            size_t index0 = myString.find_first_of( ':' );
            if ( index0 == std::string::npos )
            {
                index0 = 0;
            }
            else
            {
                index0 += 2;
            }
            
            size_t index1 = myString.find_last_of( '/' );
            if ( index1 == std::string::npos )
            {
                index1 = myString.find_last_of( '\\' );
                if ( index1 == std::string::npos )
                {
                    index1 = 0;
                }
            }

            if ( index1 > index0 )
            {     
                std::string finalpath = myString.substr( index0, index1 - index0 + 1 );
                System::StringCopy( output, size, finalpath.c_str() );
            }
            else
            {
                System::StringCopy( output, size, "" );
            }
#endif
		}

		//===========================================================================

		void MakeAbsolutePath( const char* input, char* output, int size )
		{
#if defined PLAT_PC
            wchar_t moduleFileName[_MAX_PATH];
			char filename[ _MAX_PATH ];			
			char drive[ _MAX_DRIVE ];
			char path[ _MAX_PATH ];

			::GetModuleFileName( GetModuleHandle( NULL ), moduleFileName, _MAX_PATH );
            wcstombs( filename, moduleFileName, wcslen(moduleFileName) );

			::_splitpath_s( filename, drive, _MAX_DRIVE, path, _MAX_PATH, NULL, NULL, NULL, NULL );
			
			System::StringCopy( output, size, drive );
			System::StringConcat( output, size, path );
			System::StringConcat( output, size, input );
#else
			UNUSED_ALWAYS( input );
			UNUSED_ALWAYS( output );
			UNUSED_ALWAYS( size );
#endif
		}

        //===========================================================================

        void MakeDirectory( const char* input )
        {
#if defined PLAT_PC
#endif
#if defined PLAT_ANDROID || defined PLAT_IOS
            int result = 0;
            result = mkdir( input, 0770 );
            if ( result != 0 )
            {
                Assert( errno == EEXIST, "" );
            }
#endif
#if defined PLAT_WINDOWS_PHONE || defined PLAT_PC              
			wchar_t filenameWCHAR[256];
			System::ConvertToWChar( input, filenameWCHAR, 256 );
            BOOL result = CreateDirectory( filenameWCHAR, NULL );
            if ( result == 0 )
            {
                DWORD lastError = 0;
                lastError = GetLastError();            
                Assert( lastError == ERROR_ALREADY_EXISTS, "" );
            }
#endif
        }
	}

}
