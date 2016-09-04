#ifndef CONSOLE_HPP
#define CONSOLE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedarray.hpp"
#include "common/engine/system/fixedstring.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

#define MAX_LINE_LENGTH 1024

//////////////////////////////////////////////////////
// FORWARD DELCARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  
	namespace Debug
	{
		class Console  
		{
		public:

			static void Destroy();
			static Console* Get();

			const char* GetCurrentLine( int index );

			void KeyboardDown( unsigned char key );
			void KeyboardChar( unsigned char key );

		private:

			Console();
			~Console();

			static Console* mInstance;

			char mCurrentLine[2][MAX_LINE_LENGTH];

			FixedArray< System::FixedString<256> > mLineHistory;
			int mLineHistoryIndex;

		};
	}
}

#endif 
