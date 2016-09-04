#ifndef SYSTEM_UTILITIES_HPP
#define SYSTEM_UTILITIES_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include "common/engine/math/vector3.hpp"
#include "common/engine/system/fixedstring.hpp"
#include <vector>
#include <string>

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <windows.h>
#endif

//////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////

namespace System
{
	//
	// Print string out into the debug window
	//
	void PrintToConsole( const char* fmt, ... );

	void EndianSwap( int &value );
	void EndianSwap( unsigned int &value );  
	void EndianSwap( unsigned short &value );  
	
	void WriteString( FILE* file, const char* myString, bool isBinary );
	void WriteInteger( FILE* file, int myInt, bool isBinary );
	void WriteBool( FILE* file, bool myBool, bool isBinary );
	void WriteFloat( FILE* file, float myFloat, bool isBinary );
	void WriteVector3( FILE* file, const Math::Vector3& myVector, bool isBinary );
	void WriteByte( FILE* file, signed char &myByte, bool isBinary );

	void ReadString( FILE* file, char* myString, int bufferSize, bool isBinary );
	void ReadInteger( FILE* file, int &myInt, bool isBinary );
	void ReadBool( FILE* file, bool &myBool, bool isBinary );
	void ReadFloat( FILE* file, float &myFloat, bool isBinary );
	void ReadVector3( FILE* file, Math::Vector3& myVector, bool isBinary );
	void ReadByte( FILE* file, signed char &myByte, bool isBinary );

	void ReadString( char* &data, char* myString, int bufferSize, bool isBinary );
	void ReadInteger( char* &data, int &myInt, bool isBinary );
	void ReadBool( char* &data, bool &myBool, bool isBinary );
	void ReadFloat( char* &data, float &myFloat, bool isBinary );
	void ReadVector3( char* &data, Math::Vector3& myVector, bool isBinary );
	void ReadByte( char* &data, signed char &myByte, bool isBinary );

	FILE* OpenFile( const char* filename, const char* mode );
	int GetFileSize( FILE* file );	

	void Tokenize( std::vector<std::string> &tokens, char* str, const std::string& delimiters );
}

#endif
