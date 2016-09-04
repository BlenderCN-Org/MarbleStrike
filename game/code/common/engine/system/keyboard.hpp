#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

#define KEYBOARD_TYPE_TUPLE \
	KEYBOARD_TYPE_ENTRY(KB_BACKSPACE, VK_BACK, 8 ) \
	KEYBOARD_TYPE_ENTRY(KB_ENTER, VK_RETURN, 10 ) \
	KEYBOARD_TYPE_ENTRY(KB_SPACE, VK_SPACE, 32 ) \
	KEYBOARD_TYPE_ENTRY(KB_A, 'A', 'A' ) \
	KEYBOARD_TYPE_ENTRY(KB_D, 'D', 'D' ) \
	KEYBOARD_TYPE_ENTRY(KB_W, 'W', 'W'  ) \
	KEYBOARD_TYPE_ENTRY(KB_S, 'S', 'S' ) \
	KEYBOARD_TYPE_ENTRY(KB_R, 'R', 'R' ) \
	KEYBOARD_TYPE_ENTRY(KB_Q, 'Q', 'Q' ) \
	KEYBOARD_TYPE_ENTRY(KB_LEFT_ARROW, VK_LEFT, 32848  ) \
	KEYBOARD_TYPE_ENTRY(KB_RIGHT_ARROW, VK_RIGHT, 32847 ) \
	KEYBOARD_TYPE_ENTRY(KB_UP_ARROW, VK_UP, 32850 ) \
	KEYBOARD_TYPE_ENTRY(KB_DOWN_ARROW, VK_DOWN, 32849 ) \
	KEYBOARD_TYPE_ENTRY(KB_TILE, 0xC0, 96 ) \

enum KeyboardVirtualKeyType
{
#define KEYBOARD_TYPE_ENTRY( ENUM, VIRTUALKEY, ASCII_KEY ) ENUM,
	KEYBOARD_TYPE_TUPLE
#undef KEYBOARD_TYPE_ENTRY
	KEYBOARD_TYPE_MAX
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

namespace System
{
	char GetKeyValue( KeyboardVirtualKeyType keyType );
}

#endif
