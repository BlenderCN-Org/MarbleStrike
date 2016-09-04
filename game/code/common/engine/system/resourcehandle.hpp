#ifndef RESOURCE_HANDLE_HPP
#define RESOURCE_HANDLE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <stdio.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

template <class DATA>
class ResourceHandle
{
public:
	ResourceHandle()
	{
		mIndex = 0xFFFFFFFF;
	}

	~ResourceHandle()
	{	
	}

	void SetIndex( unsigned int index )
	{
		mIndex = index;
	}

	unsigned int GetIndex() const
	{
		return mIndex;
	}

	bool IsNull() const
	{
		if ( mIndex == 0xFFFFFFFF )
		{
			return true;
		}

		return false;
	}

	void SetNull()
	{
		mIndex = 0xFFFFFFFF;
	}

	bool operator==( const ResourceHandle<DATA> &other )
	{
		if ( other.mIndex == mIndex )
		{
			return true;
		}

		return false;
	}

	bool operator!=( const ResourceHandle<DATA> &other )
	{	
		return !( *this == other );
	}

private:

	unsigned int mIndex;
};

#endif 
