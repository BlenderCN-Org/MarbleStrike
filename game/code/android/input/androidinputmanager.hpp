#ifndef ANDROID_INPUTMANAGER_HPP
#define ANDROID_INPUTMANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/input/inputmanager.hpp"
#include "common/engine/system/utilities.hpp"
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

class AndroidInputManager : public InputManager
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
		int mX;
		int mY;
		float mZoom;
	};

    typedef std::map<int, TouchData> TouchDataMap;

	AndroidInputManager();
	~AndroidInputManager();

	void Update();
	void TouchDown( int ID, int x, int y );
	void TouchUp( int ID, int x, int y );
	void TouchMove( int ID, int x, int y );
	void TouchZoom( float zTouch );
	void Accelerometer();

	InputDataState* GetInputDataState()
	{
		return &mInputDataState;
	}

	AndroidInputManager* GetInputManagerPlat()
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