//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "pc/input/pcinputmanager.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/scriptmanager.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//#define ENABLE_TOUCH_OUTPUT

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

PCInputManager::PCInputManager()
{
}

//===========================================================================

PCInputManager::~PCInputManager()
{
}

//===========================================================================

void PCInputManager::Update()
{	
    std::vector<int> mDeleteIDs;
    
    TouchDataMap::iterator iter;
    for ( iter = mTouchDataMap.begin(); iter != mTouchDataMap.end(); ++iter )
    {
        TouchData touchData = (*iter).second;
        if ( touchData.mTouchDataType & TOUCH_DATA_TYPE_UP )
        {
            mDeleteIDs.push_back( touchData.mID );
        }
    }
    
    for ( size_t i = 0; i < mDeleteIDs.size(); ++i )
    {
        mOldTouchDataMap.erase( mDeleteIDs[i] );
        mTouchDataMap.erase( mDeleteIDs[i] );
#if defined ( _DEBUG )
        System::PrintToConsole( "erase: %i\n", mDeleteIDs[i] );
#endif
    }

    while ( !mTouchData.empty() )
	{
        TouchData data = mTouchData.front();
		mTouchData.pop();

        mOldTouchDataMap[data.mID] = mTouchDataMap[data.mID];
        mTouchDataMap[data.mID] = data;

        mTouchDataMap[data.mID].mTouchDataType |= mOldTouchDataMap[data.mID].mTouchDataType;
        if ( mTouchDataMap[data.mID].mTouchDataType & TOUCH_DATA_TYPE_UP )
        {
            mTouchDataMap[data.mID].mTouchDataType &= ~TOUCH_DATA_TYPE_DOWN;
            mTouchDataMap[data.mID].mTouchDataType &= ~TOUCH_DATA_TYPE_MOVE;
        }

#if defined ( _DEBUG ) && defined ( ENABLE_TOUCH_OUTPUT )
        System::PrintToConsole( "id: %i, old: %i, new: %i, x: %i, y: %i\n", 
            data.mID, 
            mOldTouchDataMap[data.mID].mTouchDataType, 
            mTouchDataMap[data.mID].mTouchDataType, 
            mTouchDataMap[data.mID].mX, 
            mTouchDataMap[data.mID].mY 
            );
#endif
    }

	memcpy( mInputDataState.mPreviousKeys, mInputDataState.mKeys, sizeof( char ) * 256 );
	memcpy( mInputDataState.mKeys, mKeys, sizeof( char ) * 256 );

	Accelerometer();
}

//===========================================================================

void PCInputManager::TouchDown( int ID, int xTouch, int yTouch, int button )
{
	TouchData data;
    data.mID = ID;
	data.mTouchDataType = TOUCH_DATA_TYPE_DOWN;
	data.mButton = button;
	data.mX = xTouch;
	data.mY = yTouch;
	mTouchData.push( data );
}

//===========================================================================

void PCInputManager::TouchUp( int ID, int xTouch, int yTouch, int button )
{
	TouchData data;
    data.mID = ID;
	data.mTouchDataType = TOUCH_DATA_TYPE_UP;
	data.mButton = button;
    data.mX = xTouch;
	data.mY = yTouch;
	mTouchData.push( data );	
}

//===========================================================================

void PCInputManager::TouchMove( int ID, int xTouch, int yTouch )
{    
	TouchData data;
    data.mID = ID;
	data.mTouchDataType = TOUCH_DATA_TYPE_MOVE;
	data.mX = xTouch;	
	data.mY = yTouch;	
	mTouchData.push( data );
}

//===========================================================================

void PCInputManager::TouchZoom( float zTouch )
{	
	float newZTouch = -zTouch / 120.0f;
	TouchData data;
	data.mTouchDataType = TOUCH_DATA_TYPE_ZOOM;
	data.mZoom = newZTouch;
	mTouchData.push( data );
}

//===========================================================================

void DegreeToXY( float angle, float& x, float& y )
{
	if (angle >= 0.0f && angle < 90.0f)
	{
		float radians = Math::DegreeToRadians(angle);
		x =  -cos( radians );
		y =  sin( radians );
	}
	else if (angle >= 90.0f && angle < 180.0f)
	{
		float radians = Math::DegreeToRadians(angle);
		x = -cos( radians );
		y =  sin( radians );
	}
	else if (angle >= 180.0f && angle < 270.0f)
	{
		float radians = Math::DegreeToRadians( 180.0f - angle );
		x = cos( radians );
		y = sin( radians );
	}
	else if (angle >= 270.0f && angle < 360.0f)
	{
		float radians = Math::DegreeToRadians( 180.0f - angle );
		x =  cos( radians );
		y =  sin( radians );
	}	
}

//===========================================================================

void PCInputManager::Accelerometer()
{	
	if ( GetKey( VK_LEFT ) )
	{
		mInputDataState.mAngle += 2.5f;
	}		

	if ( GetKey( VK_RIGHT ) )
	{
		mInputDataState.mAngle -= 2.5f;
	}		

	if ( mInputDataState.mAngle < 0 )
	{
		mInputDataState.mAngle = 360.0f + mInputDataState.mAngle;
	}
	else if ( mInputDataState.mAngle >= 360 )
	{
		mInputDataState.mAngle -= 360.0f;
	}

	float nx, ny;
	DegreeToXY( mInputDataState.mAngle, nx, ny);

	mInputDataState.mAccelerometerX = nx;
	mInputDataState.mAccelerometerY = ny;
}