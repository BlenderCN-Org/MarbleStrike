#ifndef DEBUG_VARIABLE_HPP
#define DEBUG_VARIABLE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedstring.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	namespace Debug
	{
		enum VarType
		{
			VAR_INT,
			VAR_BOOL,
			VAR_FLOAT,
			VAR_STRING
		};

		class DebugVariable
		{
		public:
			DebugVariable();
			virtual ~DebugVariable();

			virtual VarType GetType() = 0;
			virtual void SetValue( const char* valueString ) = 0;
			virtual FixedString<256> GetValue() = 0;
			virtual void KeyboardDown( unsigned char key ) = 0;		

			const char* GetName()
			{
				return mName;
			}

			const char* GetDirectory()
			{
				return mDirectory;
			}

			DebugVariable* GetNextVar()
			{
				return mNext;
			}

			static DebugVariable* GetReferenceList()
			{
				return mReferenceList;
			}

		protected:

			char mName[256];
			char mDirectory[256];
			DebugVariable* mNext;

			static DebugVariable* mReferenceList;
		};
	}
}

#endif 
