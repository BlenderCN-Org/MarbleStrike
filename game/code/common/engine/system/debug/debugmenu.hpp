#ifndef DEBUG_MENU_HPP
#define DEBUG_MENU_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/directorysubdir.hpp"
#include "common/engine/system/list.hpp"
#include <queue>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DELCARATION
//////////////////////////////////////////////////////

class Font;

namespace System
{
	class DirectoryItem;
}

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{  	
	namespace Debug
	{
		class DebugMenu
		{
		public:

			static void Create();
			static void Destroy();
			static DebugMenu* Get();

			void KeyboardDown( unsigned char key );
			void KeyboardChar( unsigned char key );
			void Update();
			void Render();		 

		private:

			DirectorySubDir* FindDirectoryItem( List<DirectoryItem*> &directory, const char* name );
			void DeleteDirectory( List<DirectoryItem*>* directoryItem );
			void SortDirectory( List<DirectoryItem*>* directoryItem );

			DebugMenu();
			~DebugMenu();

			static DebugMenu* mInstance;

			Font* mFont;		
			int mCurrentItemIndex;
			int mStartItemIndex;
			int mEndItemIndex;

			DirectorySubDir mDirectory;
			DirectorySubDir* mCurrentDirectoryItem;

			std::queue<int> mSelectionHistory;
		};        
	}
}

#endif 
