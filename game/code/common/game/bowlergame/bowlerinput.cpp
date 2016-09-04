//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/game/bowlergame/bowlerinput.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "common/engine/game/gameapp.hpp"

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
#include "pc/input/pcinputmanager.hpp"
#endif

#if defined( PLAT_IOS )
#include "ios/input/iosinputmanager.hpp"
#endif

#if defined( PLAT_ANDROID )
#include "android/input/androidinputmanager.hpp"
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerInput::BowlerInput()
{
	mCurrentDeltaX = 0;

	for ( int i = 0; i < 256; ++i )
	{
		mKeyState[i] = KEY_UP;
	}
}

//===========================================================================

BowlerInput::~BowlerInput()
{	
}

//===========================================================================

bool BowlerInput::GetToggleKey( unsigned char key )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();

	if ( mKeyState[key] == KEY_UP )
	{
		if ( inputManager->GetKey( key ) )
		{
			mKeyState[key] = KEY_DOWN;
			return true;
		}
	}
	else if ( mKeyState[key] == KEY_DOWN )
	{
		if ( inputManager->GetKey( key ) == false )
		{
			mKeyState[key] = KEY_UP;
		}
	}

	return false;
}

//===========================================================================

bool BowlerInput::GetKey( unsigned char key )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();

	return inputManager->GetKey( key );
}

//===========================================================================

std::vector<int> BowlerInput::GetTouchInput( BowlerInputType inputType )
{
    std::vector<int> results;

	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
        PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();
        PCInputManager::TouchDataMap::iterator iter;        

		switch ( inputType )
		{
		case BOWLER_INPUT_TYPE_DOWN:
			{   
                for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
                {
                    PCInputManager::TouchData mouseData = (*iter).second;
				    if ( mouseData.mTouchDataType & TOUCH_DATA_TYPE_DOWN )
				    {
                        results.push_back( mouseData.mID );
				    }
                }

                return results;
			}
			break;
		case BOWLER_INPUT_TYPE_UP:
			{
				for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
                {
                    PCInputManager::TouchData mouseData = (*iter).second;
                    if ( mouseData.mTouchDataType & TOUCH_DATA_TYPE_UP )
                    {
                        results.push_back( mouseData.mID );
                    }
                }

                return results;
			}
			break;        
		default:
			break;	
		}
#endif

#if defined( PLAT_IOS )
		iOSInputManager* iosInputManager = inputManager->GetInputManagerPlat();
		iOSInputManager::TouchDataMap* touchDataMap = iosInputManager->GetTouchDataMap();
        iOSInputManager::TouchDataMap::iterator iter;
        
        switch ( inputType )
		{
            case BOWLER_INPUT_TYPE_DOWN:
			{   
                for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
                {
                    iOSInputManager::TouchData touchData = (*iter).second;
				    if ( touchData.mTouchDataType & TOUCH_DATA_TYPE_DOWN )
				    {
                        results.push_back( touchData.mID );
				    }
                }
                
                return results;
			}
                break;
            case BOWLER_INPUT_TYPE_UP:
			{
				for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
                {
                    iOSInputManager::TouchData touchData = (*iter).second;
                    if ( touchData.mTouchDataType & TOUCH_DATA_TYPE_UP )
                    {
                        results.push_back( touchData.mID );
                    }
                }
                
                return results;
			}
                break;
		default:
			break;
		}	        
#endif

#if defined( PLAT_ANDROID )
		AndroidInputManager* androidInputManager = inputManager->GetInputManagerPlat();
		AndroidInputManager::TouchDataMap* touchDataMap = androidInputManager->GetTouchDataMap();
        AndroidInputManager::TouchDataMap::iterator iter;       
        
        switch ( inputType )
		{
            case BOWLER_INPUT_TYPE_DOWN:
			{   
                for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
                {
                    AndroidInputManager::TouchData touchData = (*iter).second;
				    if ( touchData.mTouchDataType & TOUCH_DATA_TYPE_DOWN )
				    {
                        results.push_back( touchData.mID );
				    }
                }
                
                return results;
			}
                break;
            case BOWLER_INPUT_TYPE_UP:
			{
				for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
                {
                    AndroidInputManager::TouchData touchData = (*iter).second;
                    if ( touchData.mTouchDataType & TOUCH_DATA_TYPE_UP )
                    {
                        results.push_back( touchData.mID );
                    }
                }
                
                return results;
			}
                break;
		default:
			break;
		}	        
#endif
	}

    return results;
}

//===========================================================================

bool BowlerInput::GetInput( BowlerInputType inputType )
{	
	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{
#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();

        switch ( inputType )
		{
        case BOWLER_INPUT_TYPE_JUMP_DOWN:
            {
                if ( pcInputManager->GetKey( 'B' ) )
                {
                    return true;
                }
            }
            break;
        case BOWLER_INPUT_TYPE_JUMP_UP:
            {
                if ( pcInputManager->GetKey( 'B' ) == false )
                {
                    return true;
                }
            }
            break;
		default:
			break;	
		}
#endif

//#if defined PLAT_IOS
//		IPhoneInputManager* iPhoneInputManager = inputManager->GetInputManagerPlat();
//		        
//        switch ( inputType )
//		{
//		default:
//			break;
//		}	        
//#endif
	}

	return false;
}

//===========================================================================

float BowlerInput::GetX( int ID )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
        PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();        
        return static_cast<float>( (*touchDataMap)[ID].mX );
#endif

#if defined( PLAT_IOS )
		iOSInputManager* iosInputManager = inputManager->GetInputManagerPlat();
        iOSInputManager::TouchDataMap* touchDataMap = iosInputManager->GetTouchDataMap();
        return static_cast<float>( (*touchDataMap)[ID].mX );
#endif

#if defined( PLAT_ANDROID )
		AndroidInputManager* androidInputManager = inputManager->GetInputManagerPlat();
        AndroidInputManager::TouchDataMap* touchDataMap = androidInputManager->GetTouchDataMap();        
        return static_cast<float>( (*touchDataMap)[ID].mX );
#endif
	}

	return 0;
}

//===========================================================================

float BowlerInput::GetY( int ID )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
        PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();        
        return static_cast<float>( (*touchDataMap)[ID].mY );
#endif

#if defined( PLAT_IOS )
		iOSInputManager* iosInputManager = inputManager->GetInputManagerPlat();
		iOSInputManager::TouchDataMap* touchDataMap = iosInputManager->GetTouchDataMap();
        return static_cast<float>( (*touchDataMap)[ID].mY );
#endif

#if defined( PLAT_ANDROID )
		AndroidInputManager* androidInputManager = inputManager->GetInputManagerPlat();
		AndroidInputManager::TouchDataMap* touchDataMap = androidInputManager->GetTouchDataMap();        
        return static_cast<float>( (*touchDataMap)[ID].mY );
#endif    
	}

	return 0;
}

//===========================================================================

float BowlerInput::GetDeltaX( int ID )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
        PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();
        PCInputManager::TouchDataMap* oldTouchDataMap = pcInputManager->GetOldTouchDataMap();
        mCurrentDeltaX = static_cast<float>( (*touchDataMap)[ID].mX - (*oldTouchDataMap)[ID].mX );
#endif

#if defined( PLAT_IOS )
		iOSInputManager* iosInputManager = inputManager->GetInputManagerPlat();
		iOSInputManager::TouchDataMap* touchDataMap = iosInputManager->GetTouchDataMap();
        iOSInputManager::TouchDataMap* oldTouchDataMap = iosInputManager->GetOldTouchDataMap();
        mCurrentDeltaX = static_cast<float>( (*touchDataMap)[ID].mX - (*oldTouchDataMap)[ID].mX );
#endif

#if defined( PLAT_ANDROID )
		AndroidInputManager* androidInputManager = inputManager->GetInputManagerPlat();
		AndroidInputManager::TouchDataMap* touchDataMap = androidInputManager->GetTouchDataMap();
        AndroidInputManager::TouchDataMap* oldTouchDataMap = androidInputManager->GetOldTouchDataMap();
        mCurrentDeltaX = static_cast<float>( (*touchDataMap)[ID].mX - (*oldTouchDataMap)[ID].mX );
#endif
	}

	return mCurrentDeltaX;
}

//===========================================================================

float BowlerInput::GetDeltaY( int ID )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
        PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();
        PCInputManager::TouchDataMap* oldTouchDataMap = pcInputManager->GetOldTouchDataMap();
        mCurrentDeltaY = static_cast<float>( (*touchDataMap)[ID].mY - (*oldTouchDataMap)[ID].mY );
#endif

#if defined( PLAT_IOS )
		iOSInputManager* iosInputManager = inputManager->GetInputManagerPlat();
        iOSInputManager::TouchDataMap* touchDataMap = iosInputManager->GetTouchDataMap();
        iOSInputManager::TouchDataMap* oldTouchDataMap = iosInputManager->GetOldTouchDataMap();
        mCurrentDeltaY = static_cast<float>( (*touchDataMap)[ID].mY - (*oldTouchDataMap)[ID].mY );
#endif

#if defined( PLAT_ANDROID )
		AndroidInputManager* androidInputManager = inputManager->GetInputManagerPlat();
        AndroidInputManager::TouchDataMap* touchDataMap = androidInputManager->GetTouchDataMap();
        AndroidInputManager::TouchDataMap* oldTouchDataMap = androidInputManager->GetOldTouchDataMap();
        mCurrentDeltaY = static_cast<float>( (*touchDataMap)[ID].mY - (*oldTouchDataMap)[ID].mY );
#endif      
	}	

    return mCurrentDeltaY;
}