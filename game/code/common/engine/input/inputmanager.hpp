#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <list>

#if defined PLAT_XBOX360
#include <xtl.h>
#endif

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
class PCInputManager;
typedef PCInputManager InputManagerPlat;
#endif

#if defined( PLAT_ANDROID )
class AndroidInputManager;
typedef AndroidInputManager InputManagerPlat;
#endif

#if defined( PLAT_XBOX360 )
class Xbox360InputManager;
typedef Xbox360InputManager InputManagerPlat;
#endif

#if defined( PLAT_PS3 )
class PS3InputManager;
typedef PS3InputManager InputManagerPlat;
#endif

#if defined( PLAT_IOS )
class iOSInputManager;
typedef iOSInputManager InputManagerPlat;
#endif

//////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////

typedef void (*KeyboardDownCallback)( unsigned char );
typedef void (*KeyboardCharCallback)( unsigned char );

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum InputStateType
{
	INPUT_GAME_STATE,
	INPUT_CONSOLE_STATE,
	INPUT_DEBUG_MENU_STATE
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class InputManager
{
public:

	static InputManager* CreateInputManager();
	
	InputManager();
	virtual ~InputManager();

	virtual void Update() = 0;
	
	//===========================================================================
	// Keyboard functions
	//===========================================================================
	void KeyboardDown( unsigned char key );
	void KeyboardUp( unsigned char key );		
	void KeyboardChar( unsigned char key );

	bool GetKey( unsigned char key ) const
	{
		return mKeys[key];
	}	
	
	void AddKeyboardDownCallback( KeyboardDownCallback callback )
	{
		mKeyboardDownCallBack.push_back( callback );
	}

	void AddKeyboardCharCallback( KeyboardCharCallback callback )
	{
		mKeyboardCharCallBack.push_back( callback );
	}

	virtual InputManagerPlat* GetInputManagerPlat() = 0;

	void SetInputState( InputStateType state )
	{
		mInputState = state;
	}

	InputStateType GetInputStateType()
	{
		return mInputState;
	}

protected:

	bool mKeys[256];	

private:
	std::list<KeyboardDownCallback> mKeyboardDownCallBack;
	std::list<KeyboardCharCallback> mKeyboardCharCallBack;	

	InputStateType mInputState;

};

#endif
