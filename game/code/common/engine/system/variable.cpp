//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/variable.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Variable::Variable( Variable::VariableType type ) 	
{	
	mVariableType = type;
}

//===========================================================================

Variable::~Variable()
{
}

//===========================================================================

Variable::VariableType Variable::GetVariableType()
{
	return mVariableType;
}	