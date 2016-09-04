//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////


#include "common/engine/database/database.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "ios/input/iosinputmanager.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

iOSInputManager::iOSInputManager()
{
}

//===========================================================================

iOSInputManager::~iOSInputManager()
{
}

//===========================================================================

void iOSInputManager::Update()
{	
    std::vector<int> mDeleteIDs;
    
    TouchDataMap::iterator iter;
    for ( iter = mTouchDataMap.begin(); iter != mTouchDataMap.end(); ++iter )
    {
        TouchData touchData = (*iter).second;
        if ( touchData.mTouchDataType == TOUCH_DATA_TYPE_UP )
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
    
    while ( !mTouchQueueData.empty() )
	{
        TouchData data = mTouchQueueData.front();
		mTouchQueueData.pop();

        mOldTouchDataMap[data.mID] = mTouchDataMap[data.mID];
        mTouchDataMap[data.mID] = data;

#if defined ( _DEBUG )
        System::PrintToConsole( "id: %i, old: %i, new: %i, x: %i, y: %i\n", 
            data.mID, 
            mOldTouchDataMap[data.mID].mTouchDataType, 
            mTouchDataMap[data.mID].mTouchDataType, 
            mTouchDataMap[data.mID].mX, 
            mTouchDataMap[data.mID].mY 
            );
#endif
    }
}

//===========================================================================

void iOSInputManager::TouchDown( int ID, int x, int y )
{	
	TouchData data;
    data.mID = ID;
	data.mTouchDataType = TOUCH_DATA_TYPE_DOWN;	
	data.mX = x;
	data.mY = y;
	mTouchQueueData.push( data );
}

//===========================================================================

void iOSInputManager::TouchUp( int ID, int x, int y )
{
	TouchData data;
    data.mID = ID;
	data.mTouchDataType = TOUCH_DATA_TYPE_UP;	
	data.mX = x;
	data.mY = y;
	mTouchQueueData.push( data );
}

//===========================================================================

void iOSInputManager::TouchMove( int ID, int x, int y )
{    
	TouchData data;
    data.mID = ID;
	data.mTouchDataType = TOUCH_DATA_TYPE_MOVE;	
	data.mX = x;
	data.mY = y;
	mTouchQueueData.push( data );
}

//===========================================================================

void iOSInputManager::TouchZoom( int ID, float z )
{	
	TouchData data;
    data.mID = ID;
	data.mTouchDataType = TOUCH_DATA_TYPE_ZOOM;
	data.mZoom = z;
	mTouchQueueData.push( data );
}

//===========================================================================

void iOSInputManager::Accelerometer( double x, double y, double z )
{
	mInputDataState.mAccelerometerX = x;
	mInputDataState.mAccelerometerY = y;
	mInputDataState.mAccelerometerZ = z;
}

//===========================================================================

void iOSInputManager::SetNumTouches( int touches )
{
	mInputDataState.mNumTouches = touches;
}

//===========================================================================

void iOSInputManager::SetSwipeDirection( int ID, SwipeDirection direction )
{
    TouchData data;
    data.mID = ID;
    data.mTouchDataType = TOUCH_DATA_TYPE_SWIPE;
	data.mSwipeDirection = direction;
	mTouchQueueData.push( data );
}