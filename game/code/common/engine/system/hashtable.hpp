#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/array.hpp"
#include "common/engine/system/list.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/stringutilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

template<class T> class HashTable 
{
public:
	
	HashTable();	
	~HashTable();

	void Insert( unsigned int index, const T &item );
	bool GetItem( unsigned int index, T* item );
	T Get( unsigned int index );
	void Remove( unsigned int index );
	List<T> GetValueSet();
	int GetSize()
	{
		return mSize;
	}

private:

	struct EntryItem
	{
		T mItem;
		unsigned int mIndex;
	};

	unsigned int mMaxSize;
	int mSize;

	Array< List<EntryItem> > mHashTable;
};


//===========================================================================

template<class T> HashTable<T>::HashTable() 
{           
	mMaxSize = 20;
	mSize = 0;

	mHashTable.SetSize( mMaxSize );

	for ( unsigned int i = 0; i < mMaxSize; i++ )
	{
		List<EntryItem> listItem;
		mHashTable.PushBack( listItem );
	}
}

//===========================================================================

template<class T> HashTable<T>::~HashTable() 
{
}

//===========================================================================

template<class T> void HashTable<T>::Insert( unsigned int index, const T &item )
{
	unsigned int i = index % mMaxSize;

	EntryItem entryItem;
	entryItem.mItem = item;
	entryItem.mIndex = index;

	mHashTable[i].InsertBack( entryItem );
	mSize++;
}

//===========================================================================

template<class T> bool HashTable<T>::GetItem( unsigned int index, T* item )
{
	unsigned int i = index % mMaxSize;

	assert( i < mMaxSize );
	if ( mHashTable[i].GetSize() == 0 )
	{
		return false;
	}
	else
	{		
		typename List<EntryItem>::Iterator iter = mHashTable[i].CreateIterator();		
		while ( iter.HasNext() )
		{
			EntryItem entryItem = iter.Next();
			
			if ( entryItem.mIndex == index )
			{
				*item = entryItem.mItem;
				return true;
			}
		}

		return false;
	}	
}

//===========================================================================

template<class T> T HashTable<T>::Get( unsigned int index )
{    
	unsigned int i = index % mMaxSize;

	assert( i < mMaxSize );
	if ( mHashTable[i].GetSize() == 0 )
	{
		return NULL;
	}
	else
	{		
		typename List<EntryItem>::Iterator iter = mHashTable[i].CreateIterator();		
		while ( iter.HasNext() )
		{
			EntryItem entryItem = iter.Next();
			
			if ( entryItem.mIndex == index )
			{
				return entryItem.mItem;
			}
		}

		return NULL;
	}	
} 

//===========================================================================

template<class T> void HashTable<T>::Remove( unsigned int index )
{    
	unsigned int i = index % mMaxSize;

	assert( i < mMaxSize );
	if ( mHashTable[i].GetSize() == 0 )
	{
		Assert( false, "" );
	}
	else
	{
		bool removed = false;

		typename List<EntryItem>::Iterator iter = mHashTable[i].CreateIterator();
		while ( iter.HasNext() )
		{
			EntryItem entryItem = iter.Next();
			if ( entryItem.mIndex == index )
			{
				iter.Remove();
				removed = true;
				mSize--;

				break;
			}
		}
		
		if ( removed == false )
		{
			Assert( false, "" );
		}
	}		
} 

//===========================================================================

template<class T> List<T> HashTable<T>::GetValueSet()
{
	List<T> valueList;

	for ( unsigned int i = 0; i < mMaxSize; ++i )
	{
		if ( mHashTable[i].GetSize() > 0 )
		{
			typename List<EntryItem>::Iterator iter = mHashTable[i].CreateIterator();
			while ( iter.HasNext() )
			{
				EntryItem entryItem = iter.Next();
				valueList.InsertBack( entryItem.mItem );
			}
		}
	}

	return valueList;
}


#endif
