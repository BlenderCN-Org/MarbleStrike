//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/variablefloat.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

VariableFloat::VariableFloat()
	:	Variable( Variable::VariableType_Float )
{
	mValue = 0.0f;
}

//===========================================================================

VariableFloat::VariableFloat( float value )
	:	Variable( Variable::VariableType_Float )
{
	mValue = value;
}

//===========================================================================

VariableFloat::~VariableFloat()
{
}

//===========================================================================

float VariableFloat::GetValue()
{
	return mValue;
}

//===========================================================================

void VariableFloat::SetValue( float value )
{
	mValue = value;
}