#ifndef FIXED_STRING_HPP
#define FIXED_STRING_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <string.h>
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/systemdefines.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{	
	template <unsigned length>
	class FixedString 
	{
	public:
		
		FixedString();
		~FixedString();
		FixedString( const char* text );
		FixedString( const FixedString& otherString );		
		FixedString& operator=( const char* text );		
		FixedString& operator=( const FixedString& otherString );		
		FixedString& operator+=( const char* text );		
		FixedString& operator+=( const FixedString& otherString );
		bool operator==( const char* text ) const;
		bool operator==( const FixedString& otherString ) const;
		char operator[]( const int index ) const;
		char& operator[]( const int index );
		operator char*();
		operator const char*() const;
		int SetSize( int size );
		int GetSize() const;
		bool Replace( const char* match, const char* str );
		void ReplaceAll( const char* match, const char* str );
		
		const char* GetRawData() const
		{
			return mData;
		}		

	private:
		
		void Copy( const FixedString& otherString );		
		void Clean();
		
		char mData[length];
		int mNumCharacters;
	};

	template <unsigned length>
	FixedString<length>::FixedString() 
		: 	mNumCharacters( 1 )
	{	
		mData[0] = '\0';
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>::FixedString( const char* text ) 		
		:	mNumCharacters( 1 )
	{ 
		int stringLength = static_cast<int>( strlen( text ) );
		mNumCharacters = stringLength + 1;

		Assert( mNumCharacters < length,  "" );
		System::StringCopy( mData, mNumCharacters, text );
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>::FixedString( const FixedString &otherString ) 		
		:	mNumCharacters( 1 )		
	{
		Copy( otherString );
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>::~FixedString()
	{
		Clean();
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>& FixedString<length>::operator= ( const FixedString &otherString )
	{        
		//
		// Check for self assignment
		//
		if ( this != &otherString )
		{
			Copy( otherString );
		}

		return *this;
	}

	//===========================================================================
	
	template <unsigned length>
	FixedString<length>& FixedString<length>::operator= ( const char* text )
	{    
		int stringLength = static_cast<int>( strlen( text ) );

		//
		// Make sure that the current string data is not the same as the text data
		//
		if ( mData == text )
		{
			Assert( mData != text, "Self Assignment" );
		}

		Clean();

		mNumCharacters = stringLength + 1;
		Assert( mNumCharacters < length,  "" );
		System::StringCopy( mData, mNumCharacters, text );		

		return *this;
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>& FixedString<length>::operator+= ( const char* text )
	{
		int stringLength = static_cast<int>( strlen( text ) );				
		mNumCharacters += stringLength;
		Assert( mNumCharacters < length,  "" );
		System::StringConcat( mData, mNumCharacters, text );

		return *this;
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>& FixedString<length>::operator+= ( const FixedString& otherString )
	{
		int stringLength = otherString.GetSize();				
		mNumCharacters += stringLength;
		Assert( mNumCharacters < length,  "" );
		System::StringConcat( mData, mNumCharacters, otherString );

		return *this;
	}

	//===========================================================================

	template <unsigned length>
	bool FixedString<length>::operator==( const char* text ) const
	{
		if ( strcmp( mData, text ) == 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//===========================================================================

	template <unsigned length>
	bool FixedString<length>::operator==( const FixedString<length>& otherString ) const
	{
		if ( strcmp( mData, otherString ) == 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//===========================================================================

	template <unsigned length>
	char FixedString<length>::operator[]( const int index ) const
	{
		return mData[index];
	}

	//===========================================================================

	template <unsigned length>
	char& FixedString<length>::operator[]( const int index )
	{
		return mData[index];
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>::operator char*()
	{
		return mData;
	}

	//===========================================================================

	template <unsigned length>
	FixedString<length>::operator const char*() const
	{		
		return mData;
	}

	//===========================================================================

	template <unsigned length>
	void FixedString<length>::Copy( const FixedString &otherString )
	{
		Clean();

		mNumCharacters = otherString.mNumCharacters;
		Assert( mNumCharacters < length,  "" );
		System::StringCopy( mData, otherString.GetSize() + 1, otherString );
	}

	//===========================================================================

	template <unsigned length>
	void FixedString<length>::Clean()
	{		
		mNumCharacters = 1;
		mData[0] = '\0';
	}

	//===========================================================================

	template <unsigned length>
	int FixedString<length>::GetSize() const
	{
		return mNumCharacters - 1;
	}

	//===========================================================================

	template <unsigned length>
	bool FixedString<length>::Replace( const char* match, const char* str )
	{
		char* indexChar = 0;
		indexChar = strstr( mData, match );
		if ( indexChar )
		{
			int index = static_cast<int>( indexChar - mData );
			char newString[256];			
			int dataLength = static_cast<int>( strlen( mData ) );
			UNUSED_ALWAYS( dataLength );
			Assert( ( dataLength + 1 ) < 256, "" );
			int strLength = static_cast<int>( strlen( str ) );
			UNUSED_ALWAYS( strLength );
			Assert( ( strLength + 1 ) < 256, "" );
			System::StringNCopy( newString, 256, mData, index );
			newString[index] = '\0';
			System::StringConcat( newString, 256, str );
			int matchLength = static_cast<int>( strlen( match ) );
			System::StringConcat( newString, 256, &mData[index + matchLength] );

			*this = newString;

			return true;
		}
		else
		{
			return false;
		}
	}

	//===========================================================================

	template <unsigned length>
	void FixedString<length>::ReplaceAll( const char* match, const char* str )
	{
		bool replaceSuccess = false;
		do
		{
			replaceSuccess = Replace( match, str );
		}
		while( replaceSuccess == true );
	}

}

#endif
