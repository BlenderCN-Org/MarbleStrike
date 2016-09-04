#ifndef STRING_HPP
#define STRING_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <string.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{	
	class String 
	{
	public:

		String();
		~String();
		String( const char* text );    
		String( const String& otherString );
		String& operator=( const char* text );
		String& operator=( const String& otherString );
		String& operator+=( const char* text );
		String& operator+=( const String& otherString );        

		bool operator==( const char* text ) const;
		bool operator==( const String& otherString ) const;
		bool operator!=( const char* text ) const;
		bool operator!=( const String& otherString ) const;
		char operator[]( const int index ) const;
		char& operator[]( const int index );		
		operator char*();
		operator char*() const;

		int GetSize() const;		
		const char* GetRawData();

		bool Replace( const char* match, const char* str );
		void ReplaceAll( const char* match, const char* str );

	private:
		void Copy( const String& otherString );
		void Clean();
		void Grow( int size );

		char* mData;
		int   mNumCharacters;
	};

}

#endif