//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/hashtable.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/utilities.hpp"
#include <string.h>

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <tchar.h>
#include <Strsafe.h>
#endif

#if defined ( PLAT_ANDROID )
#include <android/log.h>
#include "android/androidassetmanager.hpp"
#endif

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace System
{
	void PrintToConsole( const char* fmt, ... )
	{

		//
		// Holds our string
		//
		char text[10048];    

		//
		// Pointer to list of arguments
		//
		va_list ap;

		if ( fmt == NULL )
		{
			return;
		}

		//
		// Parses the string for variables
		//
		va_start( ap, fmt );

		//
		// And converts symbols to actual numbers
		//
#if defined PLAT_PS3 || defined PLAT_IOS || defined PLAT_ANDROID
		vsprintf( text, fmt, ap );
#else
		vsprintf_s( text, 10048, fmt, ap );
#endif

		//
		// Results Are Stored In Text
		//
		va_end( ap );

		printf( "%s", text );
		fflush( stdout );

#if defined ( PLAT_ANDROID )
		__android_log_print( ANDROID_LOG_DEBUG, "GAMEENGINE_LOG_TAG", text );
#endif

#if defined PLAT_WINDOWS_PHONE || defined PLAT_PC || defined PLAT_WINDOWS_8
		wchar_t wcstring[1024];
		StringCchPrintf(wcstring, 1024, L"%S", text);
		OutputDebugString( wcstring );		
#endif

	}

	//===========================================================================

	void EndianSwap( int &value )
	{
#if defined PLAT_PS3 || defined PLAT_XBOX360
		int newValue = 0;	  
		newValue |= ( ( value & 0x000000FF ) << 24 );
		newValue |= ( ( value & 0x0000FF00 ) << 8 );
		newValue |= ( ( value & 0x00FF0000 ) >> 8 );
		newValue |= ( ( value & 0xFF000000 ) >> 24 );
		value = newValue;
#else
		UNUSED_ALWAYS( value );
#endif
	}

	//===========================================================================

	void EndianSwap( unsigned int &value )
	{
#if defined PLAT_PS3 || defined PLAT_XBOX360
		unsigned int newValue = 0;
		newValue |= ( ( value & 0x000000FF ) << 24 );
		newValue |= ( ( value & 0x0000FF00 ) << 8 );
		newValue |= ( ( value & 0x00FF0000 ) >> 8 );
		newValue |= ( ( value & 0xFF000000 ) >> 24 );
		value = newValue;
#else
		UNUSED_ALWAYS( value );
#endif
	}

	//===========================================================================

	void EndianSwap( unsigned short &value )
	{
#if defined PLAT_PS3 || defined PLAT_XBOX360
		unsigned short newValue = 0;
		newValue |= ( ( value & 0x00FF ) << 8 );
		newValue |= ( ( value & 0xFF00 ) >> 8 );		
		value = newValue;
#else
		UNUSED_ALWAYS( value );
#endif
	}
	
	//===========================================================================

	void WriteString( FILE* file, const char* myString, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );
		
		int length = static_cast<int>( strlen( myString ) );
		WriteInteger( file, length, isBinary );

		if ( isBinary )
		{			
			fwrite( myString, sizeof( char ), length, file );			
		}
		else
		{		
			fprintf( file, "%s\n", myString );
		}
	}

	//===========================================================================

	void WriteInteger( FILE* file, int myInt, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fwrite( &myInt, sizeof( int ), 1, file );			
		}
		else
		{
			fprintf( file, "%i\n", myInt );
		}
	}

	//===========================================================================

	void WriteBool( FILE* file, bool myBool, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fwrite( &myBool, sizeof( bool ), 1, file );			
		}
		else
		{
			fprintf( file, "%i\n", myBool );
		}
	}

	//===========================================================================

	void WriteFloat( FILE* file, float myFloat, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fwrite( &myFloat, sizeof( float ), 1, file );			
		}
		else
		{
			fprintf( file, "%f\n", myFloat );
		}
	}

	//===========================================================================

	void WriteVector3( FILE* file, const Math::Vector3& myVector, bool isBinary )
	{
		WriteFloat( file, myVector[0], isBinary );
		WriteFloat( file, myVector[1], isBinary );
		WriteFloat( file, myVector[2], isBinary );
	}

	//===========================================================================

	void WriteByte( FILE* file, signed char &myByte, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fwrite( &myByte, sizeof( signed char ), 1, file );			
		}
		else
		{
			fprintf( file, "%i\n", myByte );
		}
	}

	//===========================================================================

	void ReadString( FILE* file, char* myString, int bufferSize, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		int length = 0;
		ReadInteger( file, length, isBinary );

		Assert( bufferSize > length + 1, "Buffer size not big enough." );
		
		if ( isBinary )
		{
			fread( myString, sizeof( char ), length, file );
			myString[length] = '\0';
		}
		else
		{
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			fscanf_s( file, "%s\n", myString, bufferSize );
#else
			fscanf( file, "%s\n", myString );
#endif
		}
	}

	//===========================================================================

	void ReadInteger( FILE* file, int &myInt, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fread( &myInt, sizeof( int ), 1, file );
			EndianSwap( myInt );
		}
		else
		{
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			fscanf_s( file, "%i\n", &myInt );
#else
			fscanf( file, "%i\n", &myInt );
#endif
		}
	}

	//===========================================================================

	void ReadBool( FILE* file, bool &myBool, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fread( &myBool, sizeof( bool ), 1, file );
		}
		else
		{
			int readInteger = 0;
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			fscanf_s( file, "%i\n", &readInteger );
#else
			fscanf( file, "%i\n", &readInteger );
#endif
			myBool = true;
			if ( readInteger == 0 )
			{
				myBool = false;
			}
		}
	}

	//===========================================================================

	void ReadFloat( FILE* file, float &myFloat, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fread( &myFloat, sizeof( float ), 1, file );
			int* intValue = reinterpret_cast<int*>( &myFloat );
			EndianSwap( *( intValue ) );
			myFloat = *( reinterpret_cast<float*>( intValue ) );
		}
		else
		{
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			fscanf_s( file, "%f\n", &myFloat );
#else
			fscanf( file, "%f\n", &myFloat );
#endif
		}
	}

	//===========================================================================

	void ReadVector3( FILE* file, Math::Vector3& myVector, bool isBinary )
	{
		ReadFloat( file, myVector[0], isBinary );
		ReadFloat( file, myVector[1], isBinary );
		ReadFloat( file, myVector[2], isBinary );
	}

	//===========================================================================

	void ReadByte( FILE* file, signed char &myByte, bool isBinary )
	{
		Assert( file != NULL, "Invalid file." );

		if ( isBinary )
		{
			fread( &myByte, sizeof( signed char ), 1, file );
		}
		else
		{			
#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
			fscanf_s( file, "%c\n", &myByte );
#else
			fscanf( file, "%c\n", &myByte );
#endif
		}
	}

	//===========================================================================

	void ReadString( char* &data, char* myString, int bufferSize, bool isBinary )
	{			
		int length = 0;
		ReadInteger( data, length, isBinary );
		System::StringNCopy( myString, bufferSize, data, length );
		myString[length] = '\0';
				
		data += sizeof( char ) * length;
	}

	//===========================================================================

	void ReadInteger( char* &data, int &myInt, bool isBinary )
	{
		UNUSED_ALWAYS( isBinary );

		memcpy( &myInt, data, sizeof( int ) );
		EndianSwap( myInt );
		data += sizeof( int );
	}

	//===========================================================================

	void ReadBool( char* &data, bool &myBool, bool isBinary )
	{
		UNUSED_ALWAYS( isBinary );

		memcpy( &myBool, data, sizeof( bool ) );		
		data += sizeof( bool );
	}

	//===========================================================================

	void ReadFloat( char* &data, float &myFloat, bool isBinary )
	{
		UNUSED_ALWAYS( isBinary );

		memcpy( &myFloat, data, sizeof( float ) );
		int* intValue = reinterpret_cast<int*>( &myFloat );
		EndianSwap( *( intValue ) );
		data += sizeof( float );
	}

	//===========================================================================

	void ReadVector3( char* &data, Math::Vector3& myVector, bool isBinary )
	{
		ReadFloat( data, myVector[0], isBinary );
		ReadFloat( data, myVector[1], isBinary );
		ReadFloat( data, myVector[2], isBinary );
	}

	//===========================================================================

	void ReadByte( char* &data, signed char &myByte, bool isBinary )
	{
		UNUSED_ALWAYS( isBinary );

		memcpy( &myByte, data, sizeof( signed char ) );		
		data += sizeof( signed char );
	}

	//===========================================================================

	FILE* OpenFile( const char* filename, const char* mode )
	{
		FILE* file = NULL;

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
		fopen_s( &file, filename, mode );
#elif defined PLAT_ANDROID
        file = AndroidAssetManager::GetInstance()->OpenFile( filename, mode );
#else
		file = fopen( filename, mode );		
#endif
		return file;
	}

	//===========================================================================

	int GetFileSize( FILE* file )
	{
		Assert( file != NULL, "" );

		size_t beginPos = ftell( file );
		fseek( file, 0, SEEK_END );
		size_t endPos = ftell( file );
		fseek( file, 0, SEEK_SET );
		size_t fileSize = endPos - beginPos;

		return (int)fileSize;
	}

	//===========================================================================

	void Tokenize( std::vector<std::string> &tokens, char* str, const std::string& delimiters )
	{
		char* nextToken = 0;
		char* tokenString = System::StringToken( str, delimiters.c_str(), &nextToken );
		while ( tokenString != NULL )
		{
			tokens.push_back( tokenString );
			tokenString = System::StringToken( NULL, delimiters.c_str(), &nextToken );
		}
	}
}

