#ifndef DEBUG_VARIABLE_HELPER_HPP
#define DEBUG_VARIABLE_HELPER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariableint.hpp"
#include "common/engine/system/debug/debugvariablefloat.hpp"
#include "common/engine/system/debug/debugvariablebool.hpp"
#include "common/engine/system/debug/debugvariablestring.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

#define CVARInt( directory, name, value, minValue, maxValue ) static System::Debug::DebugVariableInt name( directory, #name, value, minValue, maxValue )
#define CVARBool( directory, name, value ) static System::Debug::DebugVariableBool name( directory, #name, value )
#define CVARFloat( directory, name, value, minValue, maxValue ) static System::Debug::DebugVariableFloat name( directory, #name, value, minValue, maxValue )
#define CVARString( name, value ) static System::Debug::DebugVariableString name( #name, value )

#endif 
