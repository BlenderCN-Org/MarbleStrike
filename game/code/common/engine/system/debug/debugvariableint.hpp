#ifndef DEBUG_VARIABLE_INT_HPP
#define DEBUG_VARIABLE_INT_HPP

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
		class DebugVariableInt : public DebugVariable
		{
		public:

			DebugVariableInt( 
				const char* directory, 
				const char* name, 
				int value, 
				int minValue, 
				int maxValue 
				);
			~DebugVariableInt();

			VarType GetType();
			void SetValue( const char* valueString );
			FixedString<256> GetValue();
			void KeyboardDown( unsigned char key );
			void SetInt( int value );
			int GetInt();

			operator int()
			{
				return mValue;
			}

		private:

			int mValue;
			int mMinValue;
			int mMaxValue;

		};    
	}
}

#endif 
