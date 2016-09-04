//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariable.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace System
{
	namespace Debug
	{
		DebugVariable* DebugVariable::mReferenceList = NULL;
	}
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	namespace Debug
	{
		DebugVariable::DebugVariable()
		{
			mNext = mReferenceList;
			mReferenceList = this;
		}

		//===========================================================================

		DebugVariable::~DebugVariable()
		{
		}
	}
}