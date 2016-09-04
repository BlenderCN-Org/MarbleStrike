//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/resolutionconfigs.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "common/engine/system/utilities.hpp"
#include "pc/input/pcinputmanager.hpp"
#include "winphoneapp.h"

//////////////////////////////////////////////////////
// NAMESPACE
//////////////////////////////////////////////////////

using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Windows::Phone::UI::Input;

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//#define ENABLE_INPUT_PRINT

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

WinPhoneApp::WinPhoneApp()
{
    m_windowClosed = false;
    m_windowVisible = true;
}

//===========================================================================

void WinPhoneApp::Initialize(CoreApplicationView^ applicationView)
{
	applicationView->Activated +=
		ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &WinPhoneApp::OnActivated);

	CoreApplication::Suspending +=
		ref new EventHandler<SuspendingEventArgs^>(this, &WinPhoneApp::OnSuspending);

	CoreApplication::Resuming +=
		ref new EventHandler<Platform::Object^>(this, &WinPhoneApp::OnResuming);

	HardwareButtons::BackPressed += 
		ref new EventHandler<BackPressedEventArgs^>(this, &WinPhoneApp::OnBackButtonPressed);   
}

//===========================================================================

void WinPhoneApp::SetWindow(CoreWindow^ window)
{
	window->Closed += 
		ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &WinPhoneApp::OnWindowClosed);

    window->VisibilityChanged +=
        ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &WinPhoneApp::OnVisibilityChanged);

	window->PointerPressed +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinPhoneApp::OnPointerPressed);

	window->PointerReleased +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinPhoneApp::OnPointerReleased);

	window->PointerMoved +=
		ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &WinPhoneApp::OnPointerMoved);
    
	Windows::Storage::StorageFolder^ storageFolder = Package::Current->InstalledLocation;
	Platform::String^ resourcePath = storageFolder->Path;
	resourcePath += "\\resources\\";
    Windows::Storage::StorageFolder^ localFolder = Windows::Storage::ApplicationData::Current->LocalFolder;
    Platform::String^ libraryPath = localFolder->Path;	
    libraryPath += "\\";

	float dpi = DisplayProperties::LogicalDpi;
	float resolutionScale = dpi / 96.0f;
	float backBufferWidth = resolutionScale * window->Bounds.Width;
	float backBufferHeight = resolutionScale * window->Bounds.Height;

    int intWidth = static_cast<int>( backBufferWidth );
    int intHeight = static_cast<int>( backBufferHeight );

    ResolutionConfig resolutionConfig = gWindowsPhone8;
    if ( intWidth == 480 && intHeight == 800 )
    {
        resolutionConfig = gWindowsPhone8_LowCost;
    }

	Database::Create();

#if defined PLAT_WINDOWS_PHONE
	Database::Get()->SetIsLandscape( true );
    Database::Get()->SetBackBufferWidth( static_cast<int>( backBufferHeight ) );
	Database::Get()->SetBackBufferHeight( static_cast<int>( backBufferWidth ) );
#endif

#if defined PLAT_WINDOWS_8
	Database::Get()->SetIsLandscape( false );
    Database::Get()->SetBackBufferWidth( static_cast<int>( backBufferWidth ) );
	Database::Get()->SetBackBufferHeight( static_cast<int>( backBufferHeight ) );
#endif
	
	Database::Get()->SetTouchScreenWidth( static_cast<int>( window->Bounds.Width ) );
	Database::Get()->SetTouchScreenHeight( static_cast<int>( window->Bounds.Height ) );
	char tempString[1024] = {0};
	size_t returnValue = 0;
	wcstombs_s( &returnValue, tempString, resourcePath->Data(), wcslen(resourcePath->Data())+1);
	Database::Get()->SetResourcePath( tempString );
    wcstombs_s( &returnValue, tempString, libraryPath->Data(), wcslen(libraryPath->Data())+1);
    Database::Get()->SetLibraryPath( tempString );
	GameApp::Create();
	GameApp* gameApp = GameApp::Get();
    gameApp->SetScreenWidth( resolutionConfig.mGameScreenWidth );
	gameApp->SetScreenHeight( resolutionConfig.mGameScreenHeight );
	gameApp->SetCoreWindow( window );
	gameApp->SetRendererType( RENDERER_D3D11 );
	gameApp->CreateRenderer();	
	gameApp->Initialize( GameApp::GAME_TYPE_BOWLER );
}

//===========================================================================

void WinPhoneApp::Load(Platform::String^ entryPoint)
{
}

//===========================================================================

void WinPhoneApp::Run()
{
	while (!m_windowClosed)
	{
        if ( m_windowVisible )
        {
		    CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
		    GameApp::Get()->Update();
        }
        else
        {
            CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
        }
	}
}

//===========================================================================

void WinPhoneApp::Uninitialize()
{
	Database::Terminate();
}

//===========================================================================

void WinPhoneApp::OnVisibilityChanged( CoreWindow^ sender, VisibilityChangedEventArgs^ args )    
{
    m_windowVisible = args->Visible;

    if ( !m_windowVisible )
    {
        GameApp* gameApp = GameApp::Get();
        if ( gameApp )
        {
            gameApp->SetHide();
        }
    }
}

//===========================================================================

void WinPhoneApp::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
{
    m_windowClosed = true;
}

//===========================================================================

void WinPhoneApp::OnActivated(CoreApplicationView^ applicationView, IActivatedEventArgs^ args)
{
	CoreWindow::GetForCurrentThread()->Activate();
}

//===========================================================================

void WinPhoneApp::OnSuspending(Platform::Object^ sender, SuspendingEventArgs^ args)
{
	// Save application state after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations.
	// Be aware that a deferral may not be held indefinitely. After about five
	// seconds, the application will be forced to exit.
	SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

	deferral->Complete();
}

//===========================================================================

void WinPhoneApp::OnResuming(Platform::Object^ sender, Platform::Object^ args)
{
    GameApp* gameApp = GameApp::Get();
	if ( gameApp )
	{
        gameApp->SetShow();
    }
}

//===========================================================================

void WinPhoneApp::OnPointerPressed(CoreWindow^ sender, PointerEventArgs^ args)
{
	Windows::Foundation::Point point = args->CurrentPoint->Position;
	
	GameApp* gameApp = GameApp::Get();
	if ( gameApp )
	{
		float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
		float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
		float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
		float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

#if defined PLAT_WINDOWS_PHONE
		float x = ( appScreenWidth / touchScreenHeight ) * point.Y;
		float y = appScreenHeight - ( appScreenHeight / touchScreenWidth ) * point.X;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
#endif

#if defined PLAT_WINDOWS_8
        float x = ( appScreenWidth / touchScreenWidth ) * point.X;
		float y = ( appScreenHeight / touchScreenHeight ) * point.Y;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
#endif
		
#if defined( _DEBUG ) && defined( ENABLE_INPUT_PRINT )
		System::PrintToConsole( "ID: %i, Down: x: %f, y: %f\n\n", args->CurrentPoint->PointerId, x, y );
#endif
		gameApp->GetInputManager()->GetInputManagerPlat()->TouchDown( args->CurrentPoint->PointerId, xInt, yInt, BUTTON_TYPE_LEFT );
	}
}

//===========================================================================

void WinPhoneApp::OnPointerReleased(CoreWindow^ sender, PointerEventArgs^ args)
{
    Windows::Foundation::Point point = args->CurrentPoint->Position;

	GameApp* gameApp = GameApp::Get();
	if ( gameApp )
	{
		float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
		float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
		float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
		float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

#if defined PLAT_WINDOWS_PHONE
		float x = ( appScreenWidth / touchScreenHeight ) * point.Y;
		float y = appScreenHeight - ( appScreenHeight / touchScreenWidth ) * point.X;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
#endif

#if defined PLAT_WINDOWS_8
        float x = ( appScreenWidth / touchScreenWidth ) * point.X;
		float y = ( appScreenHeight / touchScreenHeight ) * point.Y;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
#endif

#if defined( _DEBUG ) && defined( ENABLE_INPUT_PRINT )
		System::PrintToConsole( "ID: %i Up: x: %f, y: %f\n\n", args->CurrentPoint->PointerId, x, y );
#endif
		gameApp->GetInputManager()->GetInputManagerPlat()->TouchUp( args->CurrentPoint->PointerId, xInt, yInt, BUTTON_TYPE_LEFT );
	}
}

//===========================================================================

void WinPhoneApp::OnPointerMoved(CoreWindow^ sender, PointerEventArgs^ args)
{
	Windows::Foundation::Point point = args->CurrentPoint->Position;

	GameApp* gameApp = GameApp::Get();
	if ( gameApp )
	{
		float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
		float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
		float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
		float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

#if defined PLAT_WINDOWS_PHONE
		float x = ( appScreenWidth / touchScreenHeight ) * point.Y;
		float y = appScreenHeight - ( appScreenHeight / touchScreenWidth ) * point.X;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
#endif

#if defined PLAT_WINDOWS_8
        float x = ( appScreenWidth / touchScreenWidth ) * point.X;
		float y = ( appScreenHeight / touchScreenHeight ) * point.Y;
		int xInt = static_cast<int>( x );
		int yInt = static_cast<int>( y );
#endif

#if defined( _DEBUG ) && defined( ENABLE_INPUT_PRINT )
		System::PrintToConsole( "ID: %i, Move: x: %f, y: %f\n\n", args->CurrentPoint->PointerId, x, y );
#endif
		gameApp->GetInputManager()->GetInputManagerPlat()->TouchMove( args->CurrentPoint->PointerId, xInt, yInt );
	}
}

//===========================================================================

void WinPhoneApp::OnBackButtonPressed(Platform::Object^ sender, Windows::Phone::UI::Input::BackPressedEventArgs^ args)
{
	GameApp* gameApp = GameApp::Get();
	if ( gameApp )
	{
		if ( !gameApp->IsExitable() )
		{
			args->Handled = true;
			gameApp->OnBackButton();
		}
	}	
}

//===========================================================================

IFrameworkView^ WinPhoneAppSource::CreateView()
{
	return ref new WinPhoneApp();
}

//===========================================================================

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
	auto direct3DApplicationSource = ref new WinPhoneAppSource();
	CoreApplication::Run(direct3DApplicationSource);
	return 0;
}