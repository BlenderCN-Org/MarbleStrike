//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "keyboard.hpp"
#include "systemdefines.hpp"

#if defined PLAT_PC
#include <windows.h>
#endif

#if defined PLAT_XBOX360
#include <xtl.h>
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

#if defined PLAT_PC
static int gKeyboardType[KEYBOARD_TYPE_MAX] = {
#define KEYBOARD_TYPE_ENTRY( ENUM, VIRTUALKEY, ASCII_KEY ) VIRTUALKEY,
	KEYBOARD_TYPE_TUPLE
#undef KEYBOARD_TYPE_ENTRY
};
#endif

#if defined PLAT_PS3 || defined PLAT_XBOX360
static int gKeyboardType[KEYBOARD_TYPE_MAX] = {
#define KEYBOARD_TYPE_ENTRY( ENUM, VIRTUALKEY, ASCII_KEY ) ASCII_KEY,
	KEYBOARD_TYPE_TUPLE
#undef KEYBOARD_TYPE_ENTRY
};
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

namespace System
{
	char GetKeyValue( KeyboardVirtualKeyType keyType )
	{
	#if defined PLAT_PC
		return static_cast<char>( gKeyboardType[keyType] );
	#else
        UNUSED_ALWAYS( keyType );
		return 0;
	#endif
	}
}