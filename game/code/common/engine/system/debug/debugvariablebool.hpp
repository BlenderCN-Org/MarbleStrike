#ifndef DEBUG_VARIABLE_BOOL_HPP
#define DEBUG_VARIABLE_BOOL_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/debugvariable.hpp"

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
		class DebugVariableBool : public DebugVariable
		{
		public:

			DebugVariableBool( const char* directory, const char* name, bool value );
			~DebugVariableBool();				

			VarType GetType();
			void SetValue( const char* valueString );
			FixedString<256> GetValue();
			void KeyboardDown( unsigned char key );
			void SetBool( bool value );
			bool GetBool();	

			operator bool()
			{
				return mValue;
			}

		private:

			bool mValue;
		};    
	}
}

#endif 
