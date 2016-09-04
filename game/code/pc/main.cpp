#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#define CRTDBG_MAP_ALLOC
#define _WIN32_WINNT 0x0400
#include <stdlib.h>
#include <crtdbg.h>
#include <windows.h>
#include <WinBase.h>
#include <intrin.h>
#include <CommCtrl.h>

#include "common/engine/system/memory.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/keyboard.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/render/vertexshader.hpp"
#include "pc/input/pcinputmanager.hpp"
#include "pc/winapp.hpp"
#include "pc/window.hpp"
#include "pc/winmenu.hpp"

#include "common/engine/system/timer.hpp"
#include "common/engine/system/taskmanager.hpp"
#include "common/engine/system/task.hpp"

#include "common/engine/game/resolutionconfigs.hpp"

WindowsMenu* gMenu = NULL;
static bool gStartedRunning = false;

#if defined RENDER_PLAT_OGLES2
RendererType gRendererType = RENDERER_OPENGLES2;
GameApp::GameType gGameType = GameApp::GAME_TYPE_BOWLER;
#endif

#if defined RENDER_PLAT_OGLES1_1
RendererType gRendererType = RENDERER_OPENGLES1_1;
GameApp::GameType gGameType = GameApp::GAME_TYPE_PUZZLE;
#endif

#if defined RENDER_PLAT_OPENGL || defined RENDER_PLAT_D3D9 || defined RENDER_PLAT_D3D11
RendererType gRendererType = RENDERER_OPENGL;
GameApp::GameType gGameType = GameApp::GAME_TYPE_BOWLER;
#endif

#define WM_MOUSEWHEEL 0x020A

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINITIONS
//////////////////////////////////////////////////////

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
void CPUInfoOutput();

//////////////////////////////////////////////////////
// Unit Test
//////////////////////////////////////////////////////
extern "C" void RunUnitTest();

//////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////

void* TestRun( void* data, int threadID )
{
	int* myData = reinterpret_cast<int*>( data );
	for ( int i = 0; i < 10; ++i )
	{
		System::PrintToConsole( "ThreadID: %i, Testing.... %i\n", threadID, *myData );
		Sleep( 100 );
	}

	return NULL;
}

void* TestRun2( void* data, int threadID )
{
	int* myData = reinterpret_cast<int*>( data );
	for ( int i = 0; i < 30; ++i )
	{
		System::PrintToConsole( "ThreadID: %i, Testing.... %i\n", threadID, *myData );
		Sleep( 100 );
	}

	return NULL;
}


INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow )
{
    RunUnitTest();

	UNUSED_ALWAYS( nCmdShow );
	UNUSED_ALWAYS( lpCmdLine );
	UNUSED_ALWAYS( hPrevInst );

	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	
	System::Memory::Create();

    //
	// Setup base path for data files
	//    
	wchar_t filename[ _MAX_PATH ];
	wchar_t exedir[ _MAX_PATH ];
	wchar_t drive[ _MAX_DRIVE ];
	wchar_t path[ _MAX_PATH ];

	::GetModuleFileName( GetModuleHandle( NULL ), filename, _MAX_PATH );   
	::_wsplitpath_s( filename, drive, _MAX_DRIVE, path, _MAX_PATH, NULL, NULL, NULL, NULL );
	::_wmakepath_s( exedir, _MAX_PATH, drive, path, NULL, NULL );
	::SetCurrentDirectory( exedir );

	CPUInfoOutput();	

    ResolutionConfig resolutionConfig = giPhoneiPodConfig;

	if ( strcmp( lpCmdLine, "iphone4retina" ) == 0 )
	{
		resolutionConfig = giPhoneiPodRetinaConfig;
	}
	else if ( strcmp( lpCmdLine, "iphone5" ) == 0 )
	{
        resolutionConfig = giPhoneiPodWideRetinaConfig;
    }
	else if ( strcmp( lpCmdLine, "windowsphone8" ) == 0 )
	{
		resolutionConfig = gWindowsPhone8;
	}
	else if ( strcmp( lpCmdLine, "ipad" ) == 0 )
	{
		resolutionConfig = giPadConfig;
	}
	else if ( strcmp( lpCmdLine, "ipadretina" ) == 0 )
	{
		resolutionConfig = giPadRetinaConfig;
	}

	MSG msg;
	wchar_t* className  = L"Windows Class";
	wchar_t* windowName = L"Game App";

	// 
	// Initialize the application class
	//
	WindowsApplication WinApp( hInstance, className, MainWndProc );
	WinApp.Register();

	//
	// Create the main window
	//
	Window window;
	window.Create( hInstance, className, windowName );	

	//
	// Create menu bar
	//   
	gMenu = NEW_PTR( "gMenu" ) WindowsMenu;
	gMenu->Create( window, gRendererType, gGameType );
    gMenu->SetResolutionConfig( resolutionConfig );

    RECT winRect;
    GetWindowRect(window, &winRect);

	//
	// Database initialization
	//
	Database::Create();
    Database::Get()->SetBackBufferWidth( resolutionConfig.mFrameBufferWidth );
    Database::Get()->SetBackBufferHeight( resolutionConfig.mFrameBufferHeight );

	Database::Get()->SetTouchScreenWidth( resolutionConfig.mFrameBufferWidth );
	Database::Get()->SetTouchScreenHeight( resolutionConfig.mFrameBufferHeight );
	
	//
	// Create application
	//	
	GameApp::Create();
	GameApp* gameApp = GameApp::Get();

    gameApp->SetScreenWidth( resolutionConfig.mGameScreenWidth );
	gameApp->SetScreenHeight( resolutionConfig.mGameScreenHeight );
	gameApp->SetIsLanscape( false );
	
	gameApp->SetHWND( window );
	gameApp->SetRendererType( gRendererType );
	gameApp->CreateRenderer();
	gameApp->Initialize( gGameType );

	//
	// Adjust screen dimensions
	//
	RECT rcClient;
	POINT ptDiff;
	RECT rcWindow;
	GetClientRect( window, &rcClient );
	GetWindowRect( window, &rcWindow );
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	MoveWindow(
		window,
		rcWindow.left, 
		rcWindow.top, 
		Database::Get()->GetBackBufferWidth() + ptDiff.x, 
		Database::Get()->GetBackBufferHeight() + ptDiff.y, 
		TRUE
		);

	//System::TaskManager* taskManager = GameApp::Get()->GetTaskManager();
	//System::Task task[3];

	//task[0].SetEntryPoint( TestRun );
	//int testID = 980;
	//task[0].SetArguments( (void*)&testID );
	//taskManager->AddTask( &task[0] );

	//task[1].SetEntryPoint( TestRun2 );
	//int testID2 = 70;
	//task[1].SetArguments( (void*)&testID2 );
	//taskManager->AddTask( &task[1] );

	//task[2].SetEntryPoint( TestRun );
	//int testID3 = 11;
	//task[2].SetArguments( (void*)&testID3 );
	//taskManager->AddTask( &task[2] );

	//taskManager->WaitForAllTasks();

	//
	// Display the main window
	//
	window.Show();

	//
	// Process the main window's messages
	//
	bool run = true;
	while ( run )
	{
        gStartedRunning = true;

		//
		// Check for messages
		//
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )  )
		{
			//
			// Exit loop
			//
			if ( msg.message == WM_QUIT ) 
			{
				run = false;
			} 
			else 
			{
				//
				// Handle or dispatch messages
				//
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}            
		} 
		else 
		{
			GameApp::Get()->Update();
		}
	}

	//
	// Clean up 
	//	
	GameApp::Get()->Shutdown();
	GameApp::Destroy();	
	Database::Terminate();

	DELETE_PTR( gMenu );

	System::Memory::CheckMemoryLog();
	System::Memory::Destroy();

	return 0;	
}

//////////////////////////////////////////////////////
// CALLBACKS
//////////////////////////////////////////////////////
inline void DegreeToXY( float angle, float& x, float& y )
{
    angle = angle - floorf(angle / 360.0f) * 360.0f;
    if (angle >= 0.0f && angle < 90.0f)
    {
        angle = Math::DegreeToRadians(90.0f - angle);
        x =  -cos(angle);
        y =  -sin(angle);
    }
    else if (angle >= 90.0f && angle < 180.0f)
    {
        angle = Math::DegreeToRadians(angle - 90.0f);
        x = -cos(angle);
        y =  sin(angle);
    }
    else if (angle >= 180.0f && angle < 270.0f)
    {
        angle = Math::DegreeToRadians(angle - 180.0f);
        x = cos(angle);
        y = sin(angle);
    }
    else if (angle >= 270.0f && angle < 360.0f)
    {
        angle = Math::DegreeToRadians(angle - 270.0f);
        x =  cos(angle);
        y = -sin(angle);
    }
}
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	GameApp* app = GameApp::Get();	

	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage( WM_QUIT );
		return 0;

	case WM_CLOSE:
		PostQuitMessage( 0 );
		return 0;

    case WM_MOVE:
        {
        }
        break;

	case WM_CREATE:
		break;

    case WM_COMMAND:
        gMenu->ProcessEvents( LOWORD( wParam ), hWnd );
        break;

    case WM_SIZE:
        if ( gStartedRunning )
        {
            if ( wParam == SIZE_MINIMIZED )
            {
                if ( app != NULL )
                {
                    app->SetHide();
                }
            }
            else
            {
                if ( app != NULL )
                {
                    app->SetShow();
                }
            }
        }
		break; 

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:	
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		if ( app != NULL )
		{
			float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
			float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
			float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
			float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

			float x = ( appScreenWidth / touchScreenWidth ) * LOWORD( lParam );
			float y = ( appScreenHeight / touchScreenHeight ) * HIWORD( lParam );
			int xInt = static_cast<int>( x );
			int yInt = static_cast<int>( y );

			app->GetInputManager()->GetInputManagerPlat()->TouchDown( 0, xInt, yInt, static_cast<int>( wParam ) );
		}
		break;
	case WM_LBUTTONUP:
		if ( app != NULL )
		{
			float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
			float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
			float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
			float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

			float x = ( appScreenWidth / touchScreenWidth ) * LOWORD( lParam );
			float y = ( appScreenHeight / touchScreenHeight ) * HIWORD( lParam );
			int xInt = static_cast<int>( x );
			int yInt = static_cast<int>( y );

			app->GetInputManager()->GetInputManagerPlat()->TouchUp( 0, xInt, yInt, BUTTON_TYPE_LEFT );
		}
		break;
	case WM_RBUTTONUP:	
		if ( app != NULL )
		{
			float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
			float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
			float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
			float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

			float x = ( appScreenWidth / touchScreenWidth ) * LOWORD( lParam );
			float y = ( appScreenHeight / touchScreenHeight ) * HIWORD( lParam );
			int xInt = static_cast<int>( x );
			int yInt = static_cast<int>( y );

			app->GetInputManager()->GetInputManagerPlat()->TouchUp( 0, xInt, yInt, BUTTON_TYPE_RIGHT );
		}
		break;
	case WM_MOUSEWHEEL:
		if ( app != NULL )
		{
			short zDelta = static_cast<short>( HIWORD( wParam ) );
			app->GetInputManager()->GetInputManagerPlat()->TouchZoom( static_cast<float>( zDelta ) );
		}
		break;
	case WM_MOUSEMOVE:
		if ( app != NULL )
		{
			float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
			float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
			float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
			float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

			float x = ( appScreenWidth / touchScreenWidth ) * LOWORD( lParam );
			float y = ( appScreenHeight / touchScreenHeight ) * HIWORD( lParam );
			int xInt = static_cast<int>( x );
			int yInt = static_cast<int>( y );

			app->GetInputManager()->GetInputManagerPlat()->TouchMove( 0, xInt, yInt );
		}
		break;
	case WM_KEYDOWN:
		if ( app != NULL )
		{
			app->GetInputManager()->KeyboardDown( static_cast<unsigned char>( wParam ) );
		}
		break;
	case WM_KEYUP:
		if ( app != NULL )
		{
			app->GetInputManager()->KeyboardUp( static_cast<unsigned char>( wParam ) );
		}
		break;
	case WM_CHAR:
		if ( app != NULL )
		{
			app->GetInputManager()->KeyboardChar( static_cast<unsigned char>( wParam ) );
		}
		break;
    case WM_HSCROLL :
        {
        }
        break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//////////////////////////////////////////////////////
// CPU Info Output
//////////////////////////////////////////////////////

void CPUInfoOutput()
{
	const char* szFeatures[] =
	{
		"x87 FPU On Chip",
		"Virtual-8086 Mode Enhancement",
		"Debugging Extensions",
		"Page Size Extensions",
		"Time Stamp Counter",
		"RDMSR and WRMSR Support",
		"Physical Address Extensions",
		"Machine Check Exception",
		"CMPXCHG8B Instruction",
		"APIC On Chip",
		"Unknown1",
		"SYSENTER and SYSEXIT",
		"Memory Type Range Registers",
		"PTE Global Bit",
		"Machine Check Architecture",
		"Conditional Move/Compare Instruction",
		"Page Attribute Table",
		"Page Size Extension",
		"Processor Serial Number",
		"CFLUSH Extension",
		"Unknown2",
		"Debug Store",
		"Thermal Monitor and Clock Ctrl",
		"MMX Technology",
		"FXSAVE/FXRSTOR",
		"SSE Extensions",
		"SSE2 Extensions",
		"Self Snoop",
		"Hyper-threading Technology",
		"Thermal Monitor",
		"Unknown4",
		"Pend. Brk. EN."
	};

	char CPUString[0x20];
	char CPUBrandString[0x40];
	int CPUInfo[4] = {-1};
	int nSteppingID = 0;
	int nModel = 0;
	int nFamily = 0;
	int nProcessorType = 0;
	int nExtendedmodel = 0;
	int nExtendedfamily = 0;
	int nBrandIndex = 0;
	int nCLFLUSHcachelinesize = 0;
	int nAPICPhysicalID = 0;
	int nFeatureInfo = 0;
	int nCacheLineSize = 0;
	int nL2Associativity = 0;
	int nCacheSizeK = 0;
	unsigned    nIds, nExIds, i;
	bool bSSE3NewInstructions = false;
	bool bMONITOR_MWAIT = false;
	bool bCPLQualifiedDebugStore = false;
	bool bThermalMonitor2 = false;

	// __cpuid with an InfoType argument of 0 returns the number of
	// valid Ids in CPUInfo[0] and the CPU identification string in
	// the other three array elements. The CPU identification string is
	// not in linear order. The code below arranges the information 
	// in a human readable form.
	__cpuid(CPUInfo, 0);
	nIds = CPUInfo[0];
	memset(CPUString, 0, sizeof(CPUString));
	*((int*)CPUString) = CPUInfo[1];
	*((int*)(CPUString+4)) = CPUInfo[3];
	*((int*)(CPUString+8)) = CPUInfo[2];

	// Get the information associated with each valid Id
	for ( i = 0; i <= nIds; ++i )
	{
		__cpuid(CPUInfo, i);
		System::PrintToConsole( "\nFor InfoType %d\n", i ); 
		System::PrintToConsole( "CPUInfo[0] = 0x%x\n", CPUInfo[0] );
		System::PrintToConsole( "CPUInfo[1] = 0x%x\n", CPUInfo[1] );
		System::PrintToConsole( "CPUInfo[2] = 0x%x\n", CPUInfo[2] );
		System::PrintToConsole( "CPUInfo[3] = 0x%x\n", CPUInfo[3] );

		// Interpret CPU feature information.
		if ( i == 1 )
		{
			nSteppingID = CPUInfo[0] & 0xf;
			nModel = (CPUInfo[0] >> 4) & 0xf;
			nFamily = (CPUInfo[0] >> 8) & 0xf;
			nProcessorType = (CPUInfo[0] >> 12) & 0x3;
			nExtendedmodel = (CPUInfo[0] >> 16) & 0xf;
			nExtendedfamily = (CPUInfo[0] >> 20) & 0xff;
			nBrandIndex = CPUInfo[1] & 0xff;
			nCLFLUSHcachelinesize = ((CPUInfo[1] >> 8) & 0xff) * 8;
			nAPICPhysicalID = (CPUInfo[1] >> 24) & 0xff;
			bSSE3NewInstructions = (CPUInfo[2] & 0x1) || false;
			bMONITOR_MWAIT = (CPUInfo[2] & 0x8) || false;
			bCPLQualifiedDebugStore = (CPUInfo[2] & 0x10) || false;
			bThermalMonitor2 = (CPUInfo[2] & 0x100) || false;
			nFeatureInfo = CPUInfo[3];
		}
	}

	// Calling __cpuid with 0x80000000 as the InfoType argument
	// gets the number of valid extended IDs.
	__cpuid( CPUInfo, 0x80000000 );
	nExIds = CPUInfo[0];
	memset( CPUBrandString, 0, sizeof( CPUBrandString ) );

	// Get the information associated with each extended ID.
	for ( i = 0x80000000; i <= nExIds; ++i )
	{
		__cpuid( CPUInfo, i );
		System::PrintToConsole( "\nFor InfoType %x\n", i ); 
		System::PrintToConsole( "CPUInfo[0] = 0x%x\n", CPUInfo[0] );
		System::PrintToConsole( "CPUInfo[1] = 0x%x\n", CPUInfo[1] );
		System::PrintToConsole( "CPUInfo[2] = 0x%x\n", CPUInfo[2] );
		System::PrintToConsole( "CPUInfo[3] = 0x%x\n", CPUInfo[3] );

		// Interpret CPU brand string and cache information.
		if ( i == 0x80000002 )
		{
			memcpy( CPUBrandString, CPUInfo, sizeof( CPUInfo ) );
		}
		else if ( i == 0x80000003 )
		{
			memcpy( CPUBrandString + 16, CPUInfo, sizeof(CPUInfo) );
		}
		else if ( i == 0x80000004 )
		{
			memcpy( CPUBrandString + 32, CPUInfo, sizeof(CPUInfo) );
		}
		else if ( i == 0x80000006 )
		{
			nCacheLineSize = CPUInfo[2] & 0xff;
			nL2Associativity = (CPUInfo[2] >> 12) & 0xf;
			nCacheSizeK = (CPUInfo[2] >> 16) & 0xffff;
		}
	}

	// Display all the information in user-friendly format.
	System::PrintToConsole( "\n\nCPU String: %s\n", CPUString );

	if ( nIds >= 1 )
	{
		if  ( nSteppingID )
		{
			System::PrintToConsole( "Stepping ID = %d\n", nSteppingID );
		}
		if  ( nModel )
		{
			System::PrintToConsole( "Model = %d\n", nModel );
		}
		if  ( nFamily )		
		{ 
			System::PrintToConsole( "Family = %d\n", nFamily );
		}
		if  ( nProcessorType )
		{	
			System::PrintToConsole( "Processor Type = %d\n", nProcessorType );
		}
		if  ( nExtendedmodel )
		{
			System::PrintToConsole( "Extended model = %d\n", nExtendedmodel );
		}
		if  ( nExtendedfamily )
		{
			System::PrintToConsole( "Extended family = %d\n", nExtendedfamily );
		}
		if  ( nBrandIndex )
		{
			System::PrintToConsole( "Brand Index = %d\n", nBrandIndex );
		}
		if  ( nCLFLUSHcachelinesize )
		{	
			System::PrintToConsole( "CLFLUSH cache line size = %d\n", nCLFLUSHcachelinesize );
		}
		if  ( nAPICPhysicalID )
		{
			System::PrintToConsole( "APIC Physical ID = %d\n", nAPICPhysicalID );
		}

		if  (
			nFeatureInfo || 
			bSSE3NewInstructions ||
			bMONITOR_MWAIT || 
			bCPLQualifiedDebugStore ||
			bThermalMonitor2
			)
		{
			System::PrintToConsole( "\nThe following features are supported:\n" );

			if ( bSSE3NewInstructions )
			{
				System::PrintToConsole( "\tSSE3 New Instructions\n" );
			}
			if ( bMONITOR_MWAIT )
			{
				System::PrintToConsole( "\tMONITOR/MWAIT\n" );
			}
			if ( bCPLQualifiedDebugStore )
			{
				System::PrintToConsole( "\tCPL Qualified Debug Store\n" );
			}
			if ( bThermalMonitor2 )
			{	
				System::PrintToConsole( "\tThermal Monitor 2\n" );
			}

			i = 0;
			nIds = 1;
			while ( i < ( sizeof( szFeatures ) / sizeof( const char* ) ) )
			{
				if ( nFeatureInfo & nIds )
				{
					System::PrintToConsole( "\t" );
					System::PrintToConsole( szFeatures[i] );
					System::PrintToConsole( "\n");
				}

				nIds <<= 1;
				++i;
			}
		}
	}

	if ( nExIds >= 0x80000004 )
	{
		System::PrintToConsole( "\nCPU Brand String: %s\n", CPUBrandString );
	}

	if ( nExIds >= 0x80000006 )
	{
		System::PrintToConsole( "Cache Line Size = %d\n", nCacheLineSize );
		System::PrintToConsole( "L2 Associativity = %d\n", nL2Associativity );
		System::PrintToConsole( "Cache Size = %dK\n", nCacheSizeK );
	}
}

#endif