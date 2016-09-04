//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/variablebool.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

VariableBool::VariableBool()
	:	Variable( Variable::VariableType_Bool )
{
	mValue = false;
}

//===========================================================================

VariableBool::VariableBool( bool value )
	:	Variable( Variable::VariableType_Bool )
{
	mValue = value;
}

//===========================================================================

VariableBool::~VariableBool()
{
}

//===========================================================================

bool VariableBool::GetValue()
{
	return mValue;
}

//===========================================================================

void VariableBool::SetValue( bool value )
{
	mValue = value;
}