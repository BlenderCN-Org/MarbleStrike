//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariablefloat.hpp"
#include "common/engine/system/keyboard.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	namespace Debug
	{
		DebugVariableFloat::DebugVariableFloat( 
			const char* directory, 
			const char* name, 
			float value, 
			float minValue, 
			float maxValue 
			) 
		{
			System::StringCopy( mName, 256, name );
			System::StringCopy( mDirectory, 256, directory );
			mValue = value;
			mMinValue = minValue;
			mMaxValue = maxValue;
		}

		//===========================================================================

		DebugVariableFloat::~DebugVariableFloat()
		{
		}

		//===========================================================================

		VarType DebugVariableFloat::GetType()
		{
			return VAR_FLOAT;
		}

		//===========================================================================

		void DebugVariableFloat::SetValue( const char* valueString )
		{
			if ( valueString )	
			{
				float value = static_cast<float>( atof( valueString ) );
				SetFloat( value );
			}
		}

		//===========================================================================

		FixedString<256> DebugVariableFloat::GetValue()
		{
			char buffer[256];		
			snprintf( buffer, 256, "%f", mValue );

			FixedString<256> valueString = buffer;
			return valueString;
		}

		//===========================================================================

		void DebugVariableFloat::KeyboardDown( unsigned char key )
		{
			if ( key == System::GetKeyValue( KB_LEFT_ARROW ) )
			{
				float value = mValue;
				value--;
				SetFloat( value );

			}
			else if ( key == System::GetKeyValue( KB_RIGHT_ARROW ) )
			{
				float value = mValue;
				value++;
				SetFloat( value );
			}
		}

		//===========================================================================

		void DebugVariableFloat::SetFloat( float value )
		{
			if ( value < mMinValue )
			{
				value = mMinValue;
			}
			else if ( value > mMaxValue )
			{
				value = mMaxValue;
			}
			else
			{
				mValue = value;
			}
		}
	}
}