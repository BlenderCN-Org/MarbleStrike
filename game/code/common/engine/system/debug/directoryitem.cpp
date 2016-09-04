//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/directoryitem.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	DirectoryItem::DirectoryItem(  DirectoryType type )
	{
		mType = type;
	}

	//===========================================================================

	DirectoryItem::~DirectoryItem()
	{
	}

	//===========================================================================

	DirectoryType DirectoryItem::GetType()
	{
		return mType;
	}

	//===========================================================================

	void DirectoryItem::SetName( const char* name )
	{
		System::StringCopy( mName, 32, name );
	}

	//===========================================================================

	const char* DirectoryItem::GetName()
	{
		return mName;
	}

}