//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/input/inputmanager.hpp"
#include "common/engine/system/keyboard.hpp"
#include "common/engine/system/memory.hpp"

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
#include "pc/input/pcinputmanager.hpp"
#endif

#if defined( PLAT_ANDROID )
#include "android/input/androidinputmanager.hpp"
#endif

#if defined( PLAT_XBOX360 )
#include "xbox360/input/xbox360inputmanager.hpp"
#endif

#if defined( PLAT_PS3 )
#include "ps3/input/ps3inputmanager.hpp"
#endif

#if defined( PLAT_IOS )
#include "ios/input/iosinputmanager.hpp"
#endif

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

InputManager* InputManager::CreateInputManager()
{
	return NEW_PTR( "Input Manager" ) InputManagerPlat;
}

//===========================================================================

InputManager::InputManager()
:	mInputState( INPUT_GAME_STATE )
{
	for ( int i = 0; i < 255; ++i )
	{
		mKeys[i] = false;
	}
}

//===========================================================================

InputManager::~InputManager()
{
}

//===========================================================================

void InputManager::KeyboardChar( unsigned char key )
{
	std::list<KeyboardCharCallback>::iterator iter = mKeyboardCharCallBack.begin();
	while ( iter != mKeyboardCharCallBack.end() )
	{
		(*iter)( key );
		iter++;
	}
}

//===========================================================================

void InputManager::KeyboardDown( unsigned char key )
{
	mKeys[key] = true;

	std::list<KeyboardDownCallback>::iterator iter = mKeyboardDownCallBack.begin();
	while ( iter != mKeyboardDownCallBack.end() )
	{
		(*iter)( key );
		iter++;
	}
}

//===========================================================================

void InputManager::KeyboardUp( unsigned char key )
{
	mKeys[key] = false;
}