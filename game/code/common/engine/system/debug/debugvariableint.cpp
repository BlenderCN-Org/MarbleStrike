//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariableint.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/keyboard.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{   
	namespace Debug
	{
		DebugVariableInt::DebugVariableInt( 
			const char* directory, 
			const char* name, 
			int value, 
			int minValue, 
			int maxValue 
			) 
		{
			System::StringCopy( mName, 256, name );
			System::StringCopy( mDirectory, 256, directory );
			mValue = value;
			mMinValue = minValue;
			mMaxValue = maxValue;
		}

		//===========================================================================

		DebugVariableInt::~DebugVariableInt()
		{
		}

		//===========================================================================

		VarType  DebugVariableInt::GetType()
		{
			return VAR_INT;
		}

		//===========================================================================

		void DebugVariableInt::SetValue( const char* valueString )
		{
			if ( valueString )	
			{
				int value = atoi( valueString );
				SetInt( value );
			}
		}

		//===========================================================================

		FixedString<256> DebugVariableInt::GetValue()
		{
			char buffer[256];		
			snprintf( buffer, 256, "%i", mValue );

			FixedString<256> valueString = buffer;
			return valueString;
		}

		//===========================================================================

		void DebugVariableInt::KeyboardDown( unsigned char key )
		{
			if ( key == System::GetKeyValue( KB_LEFT_ARROW ) )
			{
				int value = mValue;
				value--;
				SetInt( value );
			}
			else if ( key == System::GetKeyValue( KB_RIGHT_ARROW ) )
			{
				int value = mValue;
				value++;
				SetInt( value );
			}
		}

		//===========================================================================

		void DebugVariableInt::SetInt( int value )
		{
			if ( value < mMinValue )
			{
				mValue = mMinValue;
			}
			else if ( value > mMaxValue )
			{
				mValue = mMaxValue;
			}
			else
			{
				mValue = value;
			}
		}

		//===========================================================================

		int DebugVariableInt::GetInt()
		{
			return mValue;
		}
	}
}