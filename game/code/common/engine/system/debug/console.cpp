//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/debug/console.hpp"
#include "common/engine/system/debug/debugvariable.hpp"
#include "common/engine/system/keyboard.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

namespace System
{
	namespace Debug
	{
		Console* Console::mInstance = NULL;
	}
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{   
	namespace Debug
	{
		void Console::Destroy()
		{
			DELETE_PTR( mInstance );
		}

		//===========================================================================

		Console* Console::Get()
		{
			if ( mInstance == NULL )
			{
				mInstance = NEW_PTR( "Console Instance" ) Console;
			}

			return mInstance;
		}

		//===========================================================================

		const char* Console::GetCurrentLine( int index )
		{
			return mCurrentLine[index];
		}

		//===========================================================================

		void Console::KeyboardDown( unsigned char key )
		{
#ifndef PLAT_IOS

			if ( key == System::GetKeyValue( KB_UP_ARROW ) )
			{
				if ( mLineHistory.GetSize() > 0 )
				{
					mLineHistoryIndex--;
					if ( mLineHistoryIndex < 0 )
					{
						mLineHistoryIndex = 0;
					}
					System::StringCopy( mCurrentLine[0], 1024, mLineHistory[mLineHistoryIndex] );
				}
			}
			else if ( key == System::GetKeyValue( KB_DOWN_ARROW ) )
			{
				if ( mLineHistory.GetSize() > 0 )
				{
					mLineHistoryIndex++;
					if ( mLineHistoryIndex >= mLineHistory.GetSize() )
					{
						mLineHistoryIndex = mLineHistory.GetSize();
						System::StringCopy( mCurrentLine[0], 1024, "" );
					}
					else
					{
						System::StringCopy( mCurrentLine[0], 1024, mLineHistory[mLineHistoryIndex] );				
					}				
				}
			}        
#endif
		}

		//===========================================================================

		void Console::KeyboardChar( unsigned char key )
		{
#ifndef PLAT_IOS

			if ( key == System::GetKeyValue( KB_BACKSPACE ) )
			{
				int size = static_cast<int>( strlen( mCurrentLine[0] ) );
				if ( size > 0 )
				{
					mCurrentLine[0][size-1] = '\0';
				}
			}		
			else if ( 
				( key >= 'A' && key <= 'Z' ) || 
				( key >= 'a' && key <= 'z' ) ||
				( key == System::GetKeyValue( KB_SPACE ) ) ||
				( key >= '0' && key <= '9' ) ||
				( key == '-' ) ||
				( key == '.' )
				)
			{
				char keyString[2];
				snprintf( keyString, 2, "%c", key );
				System::StringConcat( mCurrentLine[0], 1024, keyString );
			}
			else if ( key == System::GetKeyValue( KB_ENTER ) )
			{			
				mLineHistory.PushBack( mCurrentLine[0] );
				mLineHistoryIndex = mLineHistory.GetSize();

				mCurrentLine[1][0] = '\0';
				char* nextToken = 0;
				char* variableName = System::StringToken( mCurrentLine[0], " ", &nextToken );

				bool isFound = false;

				if ( variableName != NULL )
				{
					DebugVariable* head = DebugVariable::GetReferenceList();
					while ( head != NULL )
					{
						DebugVariable* variable = head;

						bool stringEqual = ( System::StringICmp( variable->GetName(), variableName ) == 0 );
						if ( stringEqual )
						{
							isFound = true;

							char* valueString = System::StringToken( NULL, "\n", &nextToken );
							if ( valueString != NULL )
							{
								variable->SetValue( valueString );
							}

							System::StringCopy( mCurrentLine[1], 1024, variable->GetName() );
							System::StringConcat( mCurrentLine[1], 1024, " " );
							System::StringConcat( mCurrentLine[1], 1024, variable->GetValue() );

							break;
						}

						head = head->GetNextVar();
					}
				}

				mCurrentLine[0][0] = '\0';			

				if ( isFound == false )
				{
					snprintf( mCurrentLine[1], 1024, "Variable not found!" );
				}
			}
#endif
		}

		//===========================================================================

		Console::Console()
		{
			mCurrentLine[0][0] = '\0';
			mCurrentLine[1][0] = '\0';

			mLineHistoryIndex = 0;
			mLineHistory.SetSize( 200 );
		}

		//===========================================================================

		Console::~Console()
		{
		}
	}
}