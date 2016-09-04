#ifndef LIST_HPP
#define	LIST_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include <assert.h>
#include <stdlib.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

template<class T> class List;
template<class T> class ListNode;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////
template<class T> class ConstListIterator
{
public:

	ConstListIterator()
		:	mList( NULL )
		,	mNext( NULL )
	{
	}

	ConstListIterator( const List<T>* list )
	{
		Assert( list != NULL, "" );

		mList = list;
		mNext = mList->Front();
	}

	bool HasNext() const
	{		
		if ( mNext != NULL )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	T Next()
	{
		mCurrent = mNext;
		T currentData = mCurrent->GetData();
		mNext = mCurrent->GetNext();
		return currentData;
	}

private:

	const List<T>* mList;
	ListNode<T>* mCurrent;
	ListNode<T>* mNext;
};

template<class T> class ListIterator
{
public:

	ListIterator()
		:	mList( NULL )
		,	mNext( NULL )
	{
	}

	ListIterator( List<T>* list )
	{
		Assert( list != NULL, "" );

		mList = list;
		mNext = mList->Front();
	}

	bool HasNext()
	{		
		if ( mNext != NULL )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Remove()
	{
		ListNode<T>* previous = mCurrent->GetPrevious();
		mList->Remove( mCurrent );
		mCurrent = previous;
	}

	T Next()
	{
		mCurrent = mNext;
		T currentData = mCurrent->GetData();
		mNext = mCurrent->GetNext();
		return currentData;
	}

private:

	List<T>* mList;
	ListNode<T>* mCurrent;
	ListNode<T>* mNext;
};

//===========================================================================

template<class T> class ListNode
{
public:

	ListNode()
		:	mNext( NULL )
		,	mPrevious( NULL )
	{
	}

	~ListNode()
	{
	}

	inline void SetData( const T& item ) 
	{ 
		data = item; 
	} 
	inline T& GetData() 
	{ 
		return data; 
	}

	inline ListNode<T>* GetNext() 
	{ 
		return mNext; 
	} 	

	inline ListNode<T>* GetPrevious() 
	{ 
		return mPrevious; 
	} 

	inline void SetNext( ListNode<T>* n ) 
	{ 
		mNext = n; 
	} 

	inline void SetPrevious( ListNode<T>* p )
	{ 
		mPrevious = p; 
	} 

private:
	T data;
	ListNode<T>* mNext;
	ListNode<T>* mPrevious;
};

//===========================================================================

template<class T> class List
{
public:

	typedef ListIterator<T> Iterator;
	typedef ConstListIterator<T> ConstIterator;

	List();        
	List( const List<T>& otherList );
	~List();

	void InsertBack( const T& item ); 
	void InsertFront( const T& item ); 
	void InsertBefore( ListNode<T>* node, const T& item );
	void Pop();
	Iterator Find( const T& item );
	void Remove( ListNode<T>* node );
	void Clear(); 
	void Sort( bool(*cmpFunc)(T, T) = NULL );
	
	inline unsigned int GetSize() const 
	{ 
		return mNumItems; 
	}

	List<T>& operator= ( const List<T>& rhs );

	inline ListNode<T>* Front() const  
	{ 
		return mHead; 
	}

	inline ListNode<T>* Back() const  
	{ 
		return mTail; 
	}

	Iterator CreateIterator()
	{
		return Iterator( this );
	}

	ConstIterator CreateConstIterator() const
	{
		return ConstIterator( this );
	}

private:
	
	ListNode<T>* TakeList( int n, ListNode<T>* &list );
	ListNode<T>* MergeList( ListNode<T>* &output, ListNode<T>* A, ListNode<T>* B, bool(*cmpFunc)(T, T) );	
	void Copy( const List<T>& otherList ); 

	ListNode<T>* mHead;
	ListNode<T>* mTail;        
	unsigned int mNumItems;        
};

//===========================================================================

template<class T> List<T>::List() 
:	mHead( NULL )
,	mTail( NULL )
,	mNumItems ( 0 )
{   
}

//===========================================================================

template<class T> List<T>::List( const List<T>& otherList )  
:	mHead( NULL )
,	mTail( NULL )
,	mNumItems ( 0 )
{
	Copy( otherList );
}

//===========================================================================

template<class T> void List<T>::Copy( const List<T>& otherList )
{
	mNumItems = 0;

	//
	// Delete current List    
	//
	Clear();

	//
	// Set up a NEW_PTR List that is a copy of otherList        
	//
	ListNode<T>* cur = mHead;
	cur = otherList.mHead;

	while ( cur != NULL )
	{
		InsertBack( cur->GetData() );
		cur = cur->GetNext();
	}        
}

//===========================================================================

template<class T> List<T>::~List()
{ 
	Clear();
} 

//===========================================================================

template<class T> void List<T>::Clear()
{
	ListNode<T>* cur = mHead;
	ListNode<T>* next = mHead;
	while ( cur != NULL )
	{
		next = cur->GetNext();
		DELETE_PTR( cur );
		cur = next;
	}

	mHead = NULL;
	mTail = NULL;

	mNumItems = 0;
}

//===========================================================================

template<class T> ListNode<T>* List<T>::TakeList( int n, ListNode<T>* &list )
{
	if ( list == NULL )
	{
		return NULL;
	}

	ListNode<T>* output = list;
	ListNode<T>* curNode = list;
	ListNode<T>* current = list;
	for ( int i = 0; i < n; i++ )
	{		
		if ( current->GetNext() == NULL )
		{
			curNode = current;
			current = NULL;
			break;
		}

		curNode = current;
		current = current->GetNext();
	}

	list = current;
	if ( current != NULL )
	{
		current->SetPrevious( NULL );
	}
	curNode->SetNext( NULL );

	return output;
}

//===========================================================================

template<class T> ListNode<T>* List<T>::MergeList( ListNode<T>* &output, ListNode<T>* A, ListNode<T>* B, bool(*cmpFunc)(T, T) )
{	
	ListNode<T>* current = NULL;
	ListNode<T>* tempOutput = NULL;
	ListNode<T>* currentA = A;
	ListNode<T>* currentB = B;

	while ( currentA != NULL && currentB != NULL )
	{
		bool result = false;
		if ( cmpFunc != NULL )
		{
			result = cmpFunc( currentA->GetData(), currentB->GetData() );
		}
		else
		{
			if ( currentA->GetData() <= currentB->GetData() )
			{
				result = true;
			}
			else
			{
				result = false;
			}
		}
		
		if ( result )
		{
			if ( tempOutput == NULL )
			{
				tempOutput = currentA;
				current = tempOutput;
			}
			else
			{
				current->SetNext( currentA );
				currentA->SetPrevious( current );
				current = currentA;
			}

			currentA = currentA->GetNext();
		}
		else
		{
			if ( tempOutput == NULL )
			{
				tempOutput = currentB;
				current = tempOutput;
			}
			else
			{
				current->SetNext( currentB );
				currentB->SetPrevious( current );
				current = currentB;
			}

			currentB = currentB->GetNext();
		}
	}

	while ( currentA != NULL )
	{
		if ( tempOutput == NULL )
		{
			tempOutput = currentA;
			current = tempOutput;
		}
		else
		{
			current->SetNext( currentA );
			currentA->SetPrevious( current );
			current = currentA;
		}

		currentA = currentA->GetNext();		
	}

	while ( currentB != NULL )
	{
		if ( tempOutput == NULL )
		{
			tempOutput = currentB;
			current = tempOutput;
		}
		else
		{
			current->SetNext( currentB );
			currentB->SetPrevious( current );
			current = currentB;
		}

		currentB = currentB->GetNext();		
	}

	ListNode<T>* tempCur = output;

	if ( tempCur == NULL )
	{
		output = tempOutput;		
	}

	while ( tempCur != NULL )
	{
		if ( tempCur->GetNext() == NULL )
		{
			tempCur->SetNext( tempOutput );
			if ( tempOutput != NULL )
			{
				tempOutput->SetPrevious( tempCur );
			}
			break;
		}

		tempCur = tempCur->GetNext();
	}

	return output;
}

//===========================================================================

template<class T> void List<T>::Sort( bool(*cmpFunc)(T, T) )
{	
	bool hasMerges = true;
	if ( mNumItems > 1 )
	{		
		ListNode<T>* list = mHead;

		int n = 1;
		while ( hasMerges )
		{
			int merges = 0;
			bool loop = true;
			ListNode<T>* output = NULL;
			while ( loop )
			{
				ListNode<T>* A = TakeList( n, list );
				ListNode<T>* B = TakeList( n, list );

				if ( A != NULL )
				{					
					merges++;	
					output = MergeList( output, A, B, cmpFunc );
				}
				else
				{
					loop = false;
				}
			}

			list = output;
			if ( merges <= 1 )
			{
				mHead = list;
				hasMerges = false;
			}

			n *= 2;
		}
	}

	ListNode<T>* current = mHead;
	while ( current != NULL )
	{
		if ( current->GetNext() == NULL )
		{
			mTail = current;
			break;
		}

		current = current->GetNext();
	}
}

//===========================================================================

template<class T> void List<T>::InsertBack( const T& item )
{   
	ListNode<T>* newNode = NEW_PTR( "List Node" ) ListNode<T>;
	newNode->SetData( item );
	newNode->SetNext( NULL );

	if ( mHead == NULL )
	{
		mHead = newNode;
		mTail = mHead;
	}
	else
	{
		mTail->SetNext( newNode );
		newNode->SetPrevious( mTail );
		mTail = newNode;
	}

	mNumItems++;
}

//===========================================================================

template<class T> void List<T>::InsertFront( const T& item )
{   
	ListNode<T>* newNode = NEW_PTR( "List Node" ) ListNode<T>;
	newNode->SetData( item );
	newNode->SetNext( NULL );

	if ( mHead == NULL )
	{
		mHead = newNode;
		mTail = mHead;
	}
	else
	{
		mHead->SetPrevious( newNode );
		newNode->SetNext( mHead );
		mHead = newNode;
	}

	mNumItems++;
}

//===========================================================================

template<class T> void List<T>::InsertBefore( ListNode<T>* node, const T& item )
{   
	ListNode<T>* newNode = NEW_PTR( "List Node" ) ListNode<T>;
	newNode->SetData( item );

	if ( node == NULL )
	{
		mHead = newNode;
		mTail = mHead;
		newNode->SetPrevious( NULL );
		newNode->SetNext( NULL );
	}
	else
	{
		if ( node->GetPrevious() != NULL )
		{
			node->GetPrevious()->SetNext( newNode );              
		}
		else
		{
			mHead = newNode;
		}

		newNode->SetPrevious( node->GetPrevious() );
		newNode->SetNext( node );

		node->SetPrevious( newNode );
	}

	mNumItems++;
}

//===========================================================================

template<class T> void List<T>::Pop()
{
	if ( mHead != NULL )
	{
		ListNode<T>* headNode = mHead;
		if ( mHead->GetNext() != NULL )
		{
			mHead->GetNext()->SetPrevious( NULL );			
		}

		mHead = mHead->GetNext();
		DELETE_PTR( headNode );

		mNumItems--;
	}

	ListNode<T>* current = mHead;
	while ( current != NULL )
	{
		if ( current->GetNext() == NULL )
		{
			mTail = current;
			break;
		}

		current = current->GetNext();
	}
}

//===========================================================================

template<class T> List<T>& List<T>::operator=( const List<T>& rhs ) 
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

template<class T> typename List<T>::Iterator List<T>::Find( const T& item )
{
	typename List<T>::Iterator iter = CreateIterator();

	while ( iter.HasNext() )
	{
		T currentItem = iter.Next();
		if ( currentItem == item )
		{
			return iter;
		}
	}

	typename List<T>::Iterator emptyIter;
	return emptyIter;
}

//===========================================================================

template<class T> void List<T>::Remove( ListNode<T>* node )
{
	if ( node->GetPrevious() != NULL )
	{
		node->GetPrevious()->SetNext( node->GetNext() );
	}
	else
	{
		mHead = node->GetNext();
	}

	if ( node->GetNext() != NULL )
	{
		node->GetNext()->SetPrevious( node->GetPrevious() );
	}
	else
	{
		mTail = node->GetPrevious();
	}

	ListNode<T>* next = node->GetNext();

	DELETE_PTR( node );

	node = next;

	mNumItems--;
}

#endif
