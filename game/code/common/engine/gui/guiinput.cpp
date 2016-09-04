//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/gui/guiinput.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/database/database.hpp"

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
#include "pc/input/pcinputmanager.hpp"
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

#if defined( PLAT_ANDROID )
#include "android/input/androidinputmanager.hpp"
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

GuiInput::GuiInput()
{
}

//===========================================================================

GuiInput::~GuiInput()
{	
}

//===========================================================================

std::vector<int> GuiInput::GetInput( GuiInput::InputType inputType )
{
	std::vector<int> results;

	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
		PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();
		PCInputManager::TouchDataMap::iterator iter;      

		switch ( inputType )
		{
		case GuiInput::INPUT_TYPE_ONDOWN:
			{
				for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
				{
					PCInputManager::TouchData mouseData = (*iter).second;
					if ( mouseData.mTouchDataType & TOUCH_DATA_TYPE_DOWN  )
					{
						results.push_back( mouseData.mID );
					}
				}

				return results;
			}
			break;		
		case GuiInput::INPUT_TYPE_ONUP:
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

#if defined PLAT_IOS
		iOSInputManager* iPhoneInputManager = inputManager->GetInputManagerPlat();
		iOSInputManager::TouchDataMap* touchDataMap = iPhoneInputManager->GetTouchDataMap();
		iOSInputManager::TouchDataMap::iterator iter;

		switch ( inputType )
		{
		case GuiInput::INPUT_TYPE_ONDOWN:
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
		case GuiInput::INPUT_TYPE_ONUP:
			{
				for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
				{
					iOSInputManager::TouchData touchData = (*iter).second;
					if ( touchData.mTouchDataType & TOUCH_DATA_TYPE_UP )
					{
						results.push_back( touchData.mID );
					}
				}
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
		case GuiInput::INPUT_TYPE_ONDOWN:
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
		case GuiInput::INPUT_TYPE_ONUP:
			{
				for ( iter = touchDataMap->begin(); iter != touchDataMap->end(); ++iter )
				{
					AndroidInputManager::TouchData touchData = (*iter).second;
					if ( touchData.mTouchDataType & TOUCH_DATA_TYPE_UP )
					{
						results.push_back( touchData.mID );
					}
				}
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

int GuiInput::GetInputX( int ID )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
		PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();        
		return (*touchDataMap)[ID].mX;
#endif

#if defined( PLAT_IOS )
		iOSInputManager* iosInputManager = inputManager->GetInputManagerPlat();
		iOSInputManager::TouchDataMap* touchDataMap = iosInputManager->GetTouchDataMap();

		if ( Database::Get()->GetIsLandscape() )
		{
			return (*touchDataMap)[ID].mY;
		}
		return (*touchDataMap)[ID].mX;
#endif

#if defined( PLAT_ANDROID )
		AndroidInputManager* androidInputManager = inputManager->GetInputManagerPlat();
		AndroidInputManager::TouchDataMap* touchDataMap = androidInputManager->GetTouchDataMap();

		if ( Database::Get()->GetIsLandscape() )
		{
			return (*touchDataMap)[ID].mY;
		}
		return (*touchDataMap)[ID].mX;
#endif

	}

	return 0;
}

//===========================================================================

int GuiInput::GetInputY( int ID )
{
	InputManager* inputManager = GameApp::Get()->GetInputManager();
	if ( inputManager->GetInputStateType() == INPUT_GAME_STATE )
	{

#if defined( PLAT_PC ) || defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
		PCInputManager* pcInputManager = inputManager->GetInputManagerPlat();
		PCInputManager::TouchDataMap* touchDataMap = pcInputManager->GetTouchDataMap();        
		return (*touchDataMap)[ID].mY;
#endif

#if defined( PLAT_IOS )
		iOSInputManager* iosInputManager = inputManager->GetInputManagerPlat();
		iOSInputManager::TouchDataMap* touchDataMap = iosInputManager->GetTouchDataMap();

		if ( Database::Get()->GetIsLandscape() )
		{
			float height = static_cast<float>( Database::Get()->GetAppScreenHeight() );
			return static_cast<float>( height - (*touchDataMap)[ID].mX );
		}
		return (*touchDataMap)[ID].mY;
#endif

#if defined( PLAT_ANDROID )
		AndroidInputManager* androidInputManager = inputManager->GetInputManagerPlat();
		AndroidInputManager::TouchDataMap* touchDataMap = androidInputManager->GetTouchDataMap();

		if ( Database::Get()->GetIsLandscape() )
		{
			float height = static_cast<float>( Database::Get()->GetAppScreenHeight() );
			return static_cast<float>( height - (*touchDataMap)[ID].mX );
		}
		return (*touchDataMap)[ID].mY;
#endif

	}

	return 0;
}