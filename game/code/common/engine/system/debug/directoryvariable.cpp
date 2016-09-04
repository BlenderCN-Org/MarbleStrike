//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/directoryvariable.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	namespace Debug
	{
		DirectoryVariable::DirectoryVariable()
			:	DirectoryItem( VARIABLE_TYPE )
		{			
		}

		//===========================================================================

		DirectoryVariable::~DirectoryVariable()
		{

		}

		//===========================================================================

		void DirectoryVariable::SetVariable( DebugVariable* variable )
		{
			mVariable = variable;
		}

		//===========================================================================

		DebugVariable* DirectoryVariable::GetVariable()
		{
			return mVariable;
		}
	}
}