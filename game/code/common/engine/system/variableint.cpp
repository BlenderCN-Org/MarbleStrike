//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/variableint.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

VariableInt::VariableInt()
	:	Variable( Variable::VariableType_Int )
{
	mValue = 0;
}

//===========================================================================

VariableInt::VariableInt( int value )
	:	Variable( Variable::VariableType_Int )
{
	mValue = value;
}

//===========================================================================

VariableInt::~VariableInt()
{
}

//===========================================================================

int VariableInt::GetValue()
{
	return mValue;
}

//===========================================================================

void VariableInt::SetValue( int value )
{
	mValue = value;
}