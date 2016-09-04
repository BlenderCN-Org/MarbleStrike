#ifndef PCINPUTMANAGER_HPP
#define PCINPUTMANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/input/inputmanager.hpp"
#include "common/engine/system/utilities.hpp"
#include <windows.h>
#include <queue>
#include <map>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum TouchButtonType
{
    BUTTON_TYPE_NONE = 0,
    BUTTON_TYPE_LEFT = 1,
    BUTTON_TYPE_RIGHT = 2,
    BUTTON_TYPE_MIDDLE = 16
};

enum TouchDataType
{
    TOUCH_DATA_TYPE_MOVE = 0,
    TOUCH_DATA_TYPE_DOWN = 1,
    TOUCH_DATA_TYPE_UP = 2,
    TOUCH_DATA_TYPE_ZOOM = 4
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class PCInputManager : public InputManager
{
public:

	struct InputDataState
	{
		bool mPreviousKeys[256];
		bool mKeys[256];

		float mAccelerometerX;
		float mAccelerometerY;
		float mAccelerometerZ;

		float mAngle;
	};

	struct TouchData
	{
		int mTouchDataType;
        int mID;
		int mButton;
		int mX;
		int mY;
		float mZoom;
	};

    typedef std::map<int, TouchData> TouchDataMap;

	PCInputManager();
	~PCInputManager();

	void Update();
	void TouchDown( int ID, int x, int y, int button );
	void TouchUp( int ID, int x, int y, int button );
	void TouchMove( int ID, int x, int y );
	void TouchZoom( float zTouch );
	void Accelerometer();

	InputDataState* GetInputDataState()
	{
		return &mInputDataState;
	}

	PCInputManager* GetInputManagerPlat()
	{
		return this;
	}	

    TouchDataMap* GetTouchDataMap()
    {
        return &mTouchDataMap;
    }

    TouchDataMap* GetOldTouchDataMap()
    {
        return &mOldTouchDataMap;
    }

private:

	InputDataState mInputDataState;
	std::queue<TouchData> mTouchData;
    TouchDataMap mTouchDataMap;
    TouchDataMap mOldTouchDataMap;
};

#endif