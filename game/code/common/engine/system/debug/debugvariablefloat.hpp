#ifndef DEBUG_VARIABLE_FLOAT_HPP
#define DEBUG_VARIABLE_FLOAT_HPP

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
		class DebugVariableFloat : public DebugVariable
		{
		public:

			DebugVariableFloat( 
				const char* directory, 
				const char* name, 
				float value, 
				float minValue, 
				float maxValue 
				);
			~DebugVariableFloat();			

			VarType GetType();
			void SetValue( const char* valueString );
			FixedString<256> GetValue();
			void KeyboardDown( unsigned char key );
			void SetFloat( float value );

			operator float()
			{
				return mValue;
			}

		private:

			float mValue;
			float mMinValue;
			float mMaxValue;

		};
	}
}

#endif 
