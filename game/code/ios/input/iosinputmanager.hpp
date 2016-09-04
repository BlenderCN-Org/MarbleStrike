#ifndef IOS_INPUTMANAGER_HPP
#define IOS_INPUTMANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/input/inputmanager.hpp"
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

enum SwipeDirection
{
    SWIPE_DIR_NONE,
    SWIPE_DIR_LEFT,
    SWIPE_DIR_RIGHT,
    SWIPE_DIR_UP,
    SWIPE_DIR_DOWN
};

enum TouchDataType
{
    TOUCH_DATA_TYPE_MOVE,
    TOUCH_DATA_TYPE_DOWN,
    TOUCH_DATA_TYPE_UP,
    TOUCH_DATA_TYPE_ZOOM,
    TOUCH_DATA_TYPE_SWIPE
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class iOSInputManager : public InputManager
{
public:
    
    struct TouchData
	{
		TouchDataType mTouchDataType;
        int mID;
		int mX;
		int mY;
		float mZoom;
        SwipeDirection mSwipeDirection;
	};

    typedef std::map<int, TouchData> TouchDataMap;

	struct InputDataState
	{		
		float mAccelerometerX;
		float mAccelerometerY;
		float mAccelerometerZ;

		int mNumTouches;
	};
	
	iOSInputManager();
	~iOSInputManager();

	void Update();	
	void TouchDown( int ID, int x, int y );
	void TouchUp( int ID, int x, int y );
	void TouchMove( int ID, int x, int y );
	void TouchZoom( int ID, float z );
	void Accelerometer( double x, double y, double z );
	void SetNumTouches( int touches );
    void SetSwipeDirection( int ID, SwipeDirection direction );

	InputDataState* GetInputDataState()
	{
		return &mInputDataState;
	}
	
	iOSInputManager* GetInputManagerPlat()
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
	std::queue<TouchData> mTouchQueueData;
    TouchDataMap mTouchDataMap;
    TouchDataMap mOldTouchDataMap;
	
};

#endif