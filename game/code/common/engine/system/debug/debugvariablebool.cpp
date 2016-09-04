//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariablebool.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/keyboard.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{    
	namespace Debug
	{
		DebugVariableBool::DebugVariableBool( const char* directory, const char* name, bool value ) 
		{
			System::StringCopy( mName, 256, name );
			System::StringCopy( mDirectory, 256, directory );
			mValue = value;		
		}

		//===========================================================================

		DebugVariableBool::~DebugVariableBool()
		{
		}

		//===========================================================================

		VarType DebugVariableBool::GetType()
		{
			return VAR_BOOL;
		}

		//===========================================================================

		void DebugVariableBool::SetValue( const char* valueString )
		{
			if ( strcmp( valueString, "true" ) == 0 )
			{			
				mValue = true;
			}
			else if ( strcmp( valueString, "false" ) == 0 )
			{
				mValue = false;
			}				
		}

		//===========================================================================

		FixedString<256> DebugVariableBool::GetValue()
		{
			FixedString<256> valueString;

			if ( mValue == 0 )
			{
				valueString = "false";
			}
			else
			{
				valueString = "true";
			}	

			return valueString;
		}

		//===========================================================================

		void DebugVariableBool::KeyboardDown( unsigned char key )
		{
			if ( key == System::GetKeyValue( KB_LEFT_ARROW ) || key == System::GetKeyValue( KB_RIGHT_ARROW ) )
			{			
				bool value = mValue;
				value = !value;
				SetBool( value );
			}		
		}

		//===========================================================================

		void DebugVariableBool::SetBool( bool value )
		{
			mValue = value;
		}

		//===========================================================================

		bool DebugVariableBool::GetBool()
		{
			return mValue;
		}
	}
}