#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "pc/winmenu.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/database/database.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

WindowsMenu::WindowsMenu()
:	mMenuHandle( NULL )
,	mRendererMenuHandle( NULL )
,	mAppMenuHandle( NULL )
,	mCurrentRendererMenuItem( MENU_ID_RENDERER_OPENGL )
,	mCurrentRenderer( RENDERER_OPENGL )
{

}

//===========================================================================

WindowsMenu::~WindowsMenu()
{

}

//===========================================================================

void WindowsMenu::Create( HWND hWnd, RendererType rendererType, GameApp::GameType gameType )
{
    mMenuHandle = CreateMenu();

    HMENU subMenu;
    subMenu = CreatePopupMenu();
    
    AppendMenu( subMenu, MF_STRING, MENU_ID_EXIT, L"E&xit");
    AppendMenu( mMenuHandle, MF_STRING | MF_POPUP, (UINT)subMenu, L"&File");

#if defined RENDER_PLAT_OPENGL || defined RENDER_PLAT_D3D9
    mRendererMenuHandle = CreatePopupMenu();
    AppendMenu( mRendererMenuHandle, MF_STRING, MENU_ID_RENDERER_OPENGL, L"OpenGL");
	AppendMenu( mRendererMenuHandle, MF_STRING, MENU_ID_RENDERER_D3D9, L"Direct3D");		
	AppendMenu( mMenuHandle, MF_STRING | MF_POPUP, (UINT)mRendererMenuHandle, L"&Renderer");

	mGameMenuHandle = CreatePopupMenu();
	AppendMenu( mGameMenuHandle, MF_STRING, MENU_ID_PUZZLE_GAME, L"Puzzle");
	AppendMenu( mGameMenuHandle, MF_STRING, MENU_ID_NETWORK_GAME, L"Network");
	AppendMenu( mGameMenuHandle, MF_STRING, MENU_ID_NETWORK_GAME, L"Research");		
	AppendMenu( mMenuHandle, MF_STRING | MF_POPUP, (UINT)mGameMenuHandle, L"&Game");
#endif

	SetMenu( hWnd, mMenuHandle );

	mCurrentRenderer = rendererType;
	switch( mCurrentRenderer )
	{
	case RENDERER_OPENGL:
		mCurrentRendererMenuItem = MENU_ID_RENDERER_OPENGL;
		break;
	case RENDERER_D3D9:
		mCurrentRendererMenuItem = MENU_ID_RENDERER_D3D9;
		break;	
	}

	CheckMenuItem( mRendererMenuHandle, mCurrentRendererMenuItem, MF_CHECKED );

	mCurrentGameType = gameType;
	switch( mCurrentGameType )
	{
	case GameApp::GAME_TYPE_PUZZLE:
		mCurrentGameMenuItem = MENU_ID_PUZZLE_GAME;
		break;
	case GameApp::GAME_TYPE_NETWORK:
		mCurrentGameMenuItem = MENU_ID_NETWORK_GAME;
		break;
	case GameApp::GAME_TYPE_RESEARCH:
		mCurrentGameMenuItem = MENU_ID_RESEARCH;
		break;
	}

	CheckMenuItem( mGameMenuHandle, mCurrentGameMenuItem, MF_CHECKED );
}

//===========================================================================

void WindowsMenu::ProcessEvents( int event, HWND hWnd )
{
    bool createApp = true;

    switch( event )
    {
    case MENU_ID_RENDERER_OPENGL:				
		CheckMenuItem( mRendererMenuHandle, mCurrentRendererMenuItem, MF_UNCHECKED );
		mCurrentRenderer = RENDERER_OPENGL;
		mCurrentRendererMenuItem = static_cast<MenuID>( event );
		CheckMenuItem( mRendererMenuHandle, mCurrentRendererMenuItem, MF_CHECKED );
        break;
	case MENU_ID_RENDERER_D3D9:
		CheckMenuItem( mRendererMenuHandle, mCurrentRendererMenuItem, MF_UNCHECKED );
		mCurrentRenderer = RENDERER_D3D9;
		mCurrentRendererMenuItem = static_cast<MenuID>( event );
		CheckMenuItem( mRendererMenuHandle, mCurrentRendererMenuItem, MF_CHECKED );
		break;	
	case MENU_ID_PUZZLE_GAME:
		CheckMenuItem( mGameMenuHandle, mCurrentGameMenuItem, MF_UNCHECKED );
		mCurrentGameType = GameApp::GAME_TYPE_PUZZLE;
		mCurrentGameMenuItem = static_cast<MenuID>( event );
		CheckMenuItem( mGameMenuHandle, mCurrentGameMenuItem, MF_CHECKED );
		break;
	case MENU_ID_NETWORK_GAME:
		CheckMenuItem( mGameMenuHandle, mCurrentGameMenuItem, MF_UNCHECKED );
		mCurrentGameType = GameApp::GAME_TYPE_NETWORK;
		mCurrentGameMenuItem = static_cast<MenuID>( event );
		CheckMenuItem( mGameMenuHandle, mCurrentGameMenuItem, MF_CHECKED );		
		break;
	case MENU_ID_RESEARCH:
		CheckMenuItem( mGameMenuHandle, mCurrentGameMenuItem, MF_UNCHECKED );
		mCurrentGameType = GameApp::GAME_TYPE_RESEARCH;
		mCurrentGameMenuItem = static_cast<MenuID>( event );
		CheckMenuItem( mGameMenuHandle, mCurrentGameMenuItem, MF_CHECKED );		
		break;
    case MENU_ID_EXIT:
        PostQuitMessage(WM_QUIT);	
        createApp = false;		
        break;
    }

    if ( createApp )
    {   
		if ( GameApp::Get() != NULL )
		{
			GameApp::Get()->Shutdown();
			GameApp::Destroy();
		}

		if ( Database::Get() != NULL )
		{
			Database::Terminate();
		}
		
		Database::Create();
        Database::Get()->SetBackBufferWidth( mResolutionConfig.mFrameBufferWidth );
	    Database::Get()->SetBackBufferHeight( mResolutionConfig.mFrameBufferHeight );

		Database::Get()->SetTouchScreenWidth( mResolutionConfig.mFrameBufferWidth );
		Database::Get()->SetTouchScreenHeight( mResolutionConfig.mFrameBufferHeight );

		GameApp::Create();
		GameApp* gameApp = GameApp::Get();
        gameApp->SetScreenWidth( mResolutionConfig.mGameScreenWidth );
        gameApp->SetScreenHeight( mResolutionConfig.mGameScreenHeight );
		gameApp->SetHWND( hWnd );
		gameApp->SetRendererType( mCurrentRenderer );
		gameApp->CreateRenderer();
		gameApp->Initialize( mCurrentGameType );

		//
		// Adjust screen dimensions
		//
		RECT rcClient;
		RECT rcWindow;
		POINT ptDiff;
		GetClientRect( hWnd, &rcClient );
		GetWindowRect( hWnd, &rcWindow );
		ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
		ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
		MoveWindow(
			hWnd,
			rcWindow.left, 
			rcWindow.top, 
			gameApp->GetScreenWidth() + ptDiff.x, 
			gameApp->GetScreenHeight() + ptDiff.y, 
			TRUE
			);
	}
}

//===========================================================================

void WindowsMenu::SetResolutionConfig( const ResolutionConfig &resolutionConfig )
{
    mResolutionConfig = resolutionConfig;
}

#endif