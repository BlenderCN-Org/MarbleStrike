#ifndef DEBUG_VARIABLE_STRING_HPP
#define DEBUG_VARIABLE_STRING_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariable.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	namespace Debug
	{
		class DebugVariableString : public DebugVariable
		{
		public:

			DebugVariableString( const char* name, const char* value );
			~DebugVariableString();

			VarType GetType();
			void SetValue( const char* valueString );
			FixedString<256> GetValue();		
			void KeyboardDown( unsigned char key );
			void SetString( const char* value );

			operator const char*()
			{
				return mValue;
			}		

		private:

			char mValue[256];
		};
	}
}

#endif 
