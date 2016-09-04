#ifndef ARRAY_HPP
#define	ARRAY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "common/engine/system/memory.hpp"

#if defined PLAT_XBOX360
#include <memory.h>
#endif

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

template<class T> class Array
{
public:
	//
	// Default constructor
	//
	Array( unsigned int max = DEFAULT_MAX_SIZE );        

	//
	// Copy constructor
	//
	Array( const Array<T>& otherArray );

	//
	// Destructor
	//
	~Array();

	//
	// Insert an item into the array
	//
	void PushBack( const T& item ); 
	void Insert( int index, const T& item );

	//
	// Set the size of the array
	// Will erase all data in the array
	//
	void SetSize( unsigned int size );

	//
	// Retrieve number of items in array
	//
	int GetSize() const; 

	//
	// Operator [] overload for read
	//
	T& operator[] ( unsigned int i ) const; 

	//
	// Operator [] overload for write
	//
	T& operator[] ( unsigned int i ); 

	//
	// Operator = overload
	//
	Array<T>& operator= ( const Array<T>& rhs ); 

	//
	// Find a particular item in the Array
	//
	T* Find( const T& item ); 

	//
	// Find an item and return the index
	//
	unsigned int FindIndex( const T& item ); 

	//
	// Deletes an item based on the index
	//
	void Delete( unsigned int i ); 

	//
	// Clear all the contents in the array
	//
	void Clear(); 

	//
	// Remove unused memory
	//
	void Compact(); 

private:        
	void Copy( const Array<T>& otherArray ); // Copy an array to another
	void Grow(); // Grows the array

	T* mData;
	unsigned int mMaxItems;
	unsigned int mNumItems;

	static const unsigned int DEFAULT_MAX_SIZE;
};

//===========================================================================

template<class T> const unsigned int Array<T>::DEFAULT_MAX_SIZE = 15;

//===========================================================================

//
// Implementation of template Array class
//
template<class T> Array<T>::Array( unsigned int max ) 
: mData( NULL ), mMaxItems( max ), mNumItems ( 0 )
{           
	if ( mData == NULL )
	{
		mData = NEW_PTR( "Array Data" ) T[mMaxItems];
	}
}

//===========================================================================

template<class T> Array<T>::Array( const Array<T>& otherArray ) 
: mData( NULL ), mMaxItems( DEFAULT_MAX_SIZE ), mNumItems ( 0 )
{
	Copy( otherArray );
}

//===========================================================================

template<class T> void Array<T>::Copy( const Array<T>& otherArray )
{
	mNumItems = otherArray.mNumItems;
	mMaxItems = otherArray.mMaxItems;

	//
	// Delete current array and then Set up a NEW_PTR one
	//
	if ( mData != NULL ) 
	{
		DELETE_PTR_ARRAY( mData );
		mData = NULL;
	}

	if ( mData == NULL )
	{
		mData = NEW_PTR( "Array Data" ) T[mMaxItems];

		for ( unsigned int i = 0; i < mNumItems; i++ )
		{
			mData[i] = otherArray.mData[i];
		}
	}
}

//===========================================================================

template<class T> Array<T>::~Array()
{    
	if ( mData != NULL )
	{
		DELETE_PTR_ARRAY( mData );
		mData = NULL;
	}
} 

//===========================================================================

template<class T> void Array<T>::Clear()
{
	mNumItems = 0;
	mMaxItems = DEFAULT_MAX_SIZE;
	if ( mData != NULL )
	{
		DELETE_PTR_ARRAY( mData );
		mData = NULL;
	}

	if ( mData == NULL )
	{
		mData = NEW_PTR( "Array Data" ) T[DEFAULT_MAX_SIZE];        
	}
}

//===========================================================================

template<class T> void Array<T>::PushBack( const T& item )
{        
	if ( mNumItems >= mMaxItems )
	{
		Grow();
	}

	mData[mNumItems] = item;
	mNumItems++;        
}

//===========================================================================

template<class T> void Array<T>::Insert( int index, const T& item )
{        
	if ( index >= mMaxItems )
	{
		Grow();
	}

	mData[index] = item;
	mNumItems++;        
}

//===========================================================================

template<class T> void Array<T>::SetSize( unsigned int size )
{    
	if ( mData != NULL )
	{
		DELETE_PTR_ARRAY( mData );
		mData = NULL;
	}

	mNumItems = 0;
	mMaxItems = size;

	if ( mData == NULL )
	{
		mData = NEW_PTR( "Array Data" ) T[mMaxItems];
		memset( mData, 0, sizeof( T ) * mMaxItems );
	}
} 

//===========================================================================

template<class T> int Array<T>::GetSize() const
{    
	return mNumItems;
} 

//===========================================================================

template<class T> void Array<T>::Grow()
{
	mMaxItems *= 2;

	T* newArray = NEW_PTR( "Array Grow" ) T[mMaxItems];
	for ( unsigned int i = 0; i < mNumItems; i++ )
	{
		newArray[i] = mData[i];
	}

	DELETE_PTR_ARRAY( mData );
	mData = NULL;
	mData = newArray;
}

//===========================================================================

template<class T> T& Array<T>::operator[] ( unsigned int i )
{    
	assert( i < mNumItems );
	return mData[i];
} 

//===========================================================================

template<class T> T& Array<T>::operator[] ( unsigned int i ) const
{    
	assert( i < mNumItems );            
	return mData[i];
} 

//===========================================================================

template<class T> Array<T>& Array<T>::operator=( const Array<T>& rhs ) 
{        
	//
	// Check for self assignment
	//
	if ( this != &rhs )
	{
		Copy( rhs );
	}
	return *this;
}

//===========================================================================

template<class T> T* Array<T>::Find( const T& item )
{
	for ( unsigned int i = 0; i < mNumItems; i++ )
	{
		if ( mData[i] == item )
		{
			return &mData[i];
		}
	}

	return NULL;
}

//===========================================================================

template<class T> unsigned int Array<T>::FindIndex( const T& item )
{
	for ( unsigned int i = 0; i < mNumItems; i++ )
	{
		if ( mData[i] == item )
		{
			return i;
		}
	}

	return 999999;
}

//===========================================================================

template<class T> void Array<T>::Delete( unsigned int index )
{      
	assert( index < mNumItems );

	for ( unsigned int i = index; i < mNumItems - 1; i++ )
	{
		mData[i] = mData[i + 1];
	}
	mNumItems--;
}

//===========================================================================

template<class T> void Array<T>::Compact()
{
	T* newArray = NEW_PTR( "Grow Compact" ) T[mNumItems];

	for ( unsigned int i = 0; i < mNumItems; i++ )
	{
		newArray[i] = mData[i];
	}

	mMaxItems = mNumItems;
	DELETE_PTR_ARRAY( mData );

	mData = newArray;
}

#endif
