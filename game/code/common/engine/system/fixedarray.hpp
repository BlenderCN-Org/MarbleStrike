#ifndef FIXED_ARRAY_HPP
#define	FIXED_ARRAY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"
#include <assert.h>
#include <memory.h>
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

template<class T> class FixedArray
{
public:

	FixedArray();        
	~FixedArray();
	void SetSize( unsigned int size );
	int GetSize() const; 
	void PushBack( const T &data );
	T& operator[] ( unsigned int i ) const; 
	T& operator[] ( unsigned int i ); 

private:        
	FixedArray( const FixedArray<T>& otherArray )
	{
	}

	T* mData;
	unsigned int mMaxSize;
	unsigned int mNumItems;
};

//===========================================================================

template<class T> FixedArray<T>::FixedArray() 
{
	mData = NULL;
	mMaxSize = 0;
	mNumItems = 0;
}

//===========================================================================

template<class T> FixedArray<T>::~FixedArray()
{    
	if ( mData != NULL )
	{
		DELETE_PTR_ARRAY( mData );
		mData = NULL;
	}
} 

//===========================================================================

template<class T> void FixedArray<T>::SetSize( unsigned int size )
{    
	if ( mData != NULL )
	{
		DELETE_PTR_ARRAY( mData );
		mData = NULL;
	}

	mNumItems = 0;
	mMaxSize = size;

	if ( mData == NULL )
	{
		mData = NEW_PTR( "Array Data" ) T[mMaxSize];
	}
} 

//===========================================================================

template<class T> int FixedArray<T>::GetSize() const
{    
	return mNumItems;
} 

//===========================================================================

template<class T> void FixedArray<T>::PushBack( const T &data )
{
	assert( mNumItems < mMaxSize );
	mData[mNumItems] = data;
	mNumItems++;
}

//===========================================================================

template<class T> T& FixedArray<T>::operator[] ( unsigned int i )
{    
	assert( i < mNumItems );
	return mData[i];
} 

//===========================================================================

template<class T> T& FixedArray<T>::operator[] ( unsigned int i ) const
{    
	assert( i < mNumItems );            
	return mData[i];
} 

#endif
