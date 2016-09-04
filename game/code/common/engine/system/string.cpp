//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/string.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/stringutilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	String::String() 
		:	mData( NULL )
		, 	mNumCharacters( 0 )
	{	
	}

	//===========================================================================

	String::String( const char* text ) 
		:	mData( NULL ) 
		,	mNumCharacters( 1 )
	{ 
		int length = static_cast<int>( strlen( text ) );    
		mNumCharacters = length + 1;

		mData = NEW_PTR( "String Data" ) char[mNumCharacters];		

		System::StringCopy( mData, mNumCharacters, text );
	}

	//===========================================================================

	String::String( const String &otherString ) 
		:	mData( NULL )
		,	mNumCharacters( 1 )		
	{
		Copy( otherString );
	}

	//===========================================================================

	String::~String()
	{
		Clean();
	}

	//===========================================================================

	String& String::operator= ( const String &otherString )
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

	String& String::operator= ( const char* text )
	{    
		int length = static_cast<int>( strlen( text ) );

		//
		// Make sure that the current string data is not the same as the text data
		//
		if ( mData == text )
		{
			Assert( mData != text, "Self Assignment" );
		}

		Clean();

		mNumCharacters = length + 1;
		mData = NEW_PTR( "String Data" ) char[mNumCharacters];
		System::StringCopy( mData, mNumCharacters, text );		

		return *this;
	}

	//===========================================================================

	String& String::operator+= ( const char* text )
	{
		int length = static_cast<int>( strlen( text ) );
		Grow( length );

		mNumCharacters += length;
		System::StringConcat( mData, mNumCharacters, text );

		return *this;
	}

	//===========================================================================

	String& String::operator+= ( const String& otherString )
	{
		int length = otherString.GetSize();
		Grow( length );		

		mNumCharacters += length;
		System::StringConcat( mData, mNumCharacters, otherString );

		return *this;
	}

	//===========================================================================

	bool String::operator==( const char* text ) const
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

	bool String::operator==( const String& otherString ) const
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

	bool String::operator!=( const char* text ) const
	{
		if ( strcmp( mData, text ) != 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//===========================================================================

	bool String::operator!=( const String& otherString ) const
	{
		if ( strcmp( mData, otherString ) != 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//===========================================================================

	char String::operator[]( const int index ) const
	{
		return mData[index];
	}

	//===========================================================================

	char& String::operator[]( const int index )
	{
		return mData[index];
	}

	//===========================================================================

	String::operator char*()
	{
		return mData;
	}

	//===========================================================================

	String::operator char*() const
	{
		return mData;
	}

	//===========================================================================

	int String::GetSize() const 
	{
		return mNumCharacters - 1; 
	}

	//===========================================================================

	const char* String::GetRawData()
	{
		return mData;
	}

	//===========================================================================

	void String::Copy( const String &otherString )
	{
		Clean();

		mData = NEW_PTR( "String Data" ) char[otherString.GetSize() + 1];        		
		mNumCharacters = otherString.mNumCharacters;
		System::StringCopy( mData, otherString.GetSize() + 1, otherString );
	}

	//===========================================================================

	void String::Clean()
	{
		DELETE_PTR_ARRAY( mData );
		mNumCharacters = 0;
	}

	//===========================================================================

	void String::Grow( int size )
	{
		int newLength = mNumCharacters + size;

		char* tempData = NULL;
		tempData = NEW_PTR( "String Temp Data" ) char[newLength];

		for ( int i = 0; i < mNumCharacters; i++ )
		{
			tempData[i] = mData[i];
		}

		DELETE_PTR_ARRAY( mData );
		mData = tempData;    
	}

	//===========================================================================

	bool String::Replace( const char* match, const char* str )
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

	void String::ReplaceAll( const char* match, const char* str )
	{
		bool replaceSuccess = false;
		do
		{
			replaceSuccess = Replace( match, str );
		}
		while( replaceSuccess == true );
	}
}