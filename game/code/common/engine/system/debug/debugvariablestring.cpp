//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariablestring.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{    
	namespace Debug
	{
		DebugVariableString::DebugVariableString( const char* name, const char* value ) 
		{
			System::StringCopy( mName, 256, name );
			System::StringCopy( mValue, 256, value );
		}

		//===========================================================================

		DebugVariableString::~DebugVariableString()
		{
		}

		//===========================================================================

		VarType DebugVariableString::GetType()
		{
			return VAR_STRING;
		}		

		//===========================================================================

		void DebugVariableString::SetValue( const char* valueString )
		{
			if ( valueString )	
			{
				const char* value = valueString;
				SetString( value );
			}
		}

		//===========================================================================

		FixedString<256> DebugVariableString::GetValue()
		{
			FixedString<256> valueString = mValue;
			return valueString;
		}

		//===========================================================================

		void DebugVariableString::KeyboardDown( unsigned char /*key*/ )
		{	
		}

		//===========================================================================

		void DebugVariableString::SetString( const char* value )
		{
			System::StringCopy( mValue, 256, value );
		}
	}
}