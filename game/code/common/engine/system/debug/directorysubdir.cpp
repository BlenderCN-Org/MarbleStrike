//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/directorysubdir.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	DirectorySubDir::DirectorySubDir()
		:	DirectoryItem( SUB_DIRECTORY_TYPE )
	{
		mPreviousDirectory = NULL;
	}

	//===========================================================================

	DirectorySubDir::~DirectorySubDir()
	{

	}

	//===========================================================================

	List<DirectoryItem*>* DirectorySubDir::GetDirectoryList()
	{
		return &mDirectory;
	}

	//===========================================================================

	DirectorySubDir* DirectorySubDir::GetPreviousDirectoryItem()
	{
		return mPreviousDirectory;
	}

	//===========================================================================

	void DirectorySubDir::SetPreviousDirectoryItem( DirectorySubDir* directory )
	{
		mPreviousDirectory = directory;
	}
}