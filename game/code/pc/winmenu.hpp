#ifndef WINDOWS_MENU_HPP
#define WINDOWS_MENU_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderer.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/resolutionconfigs.hpp"

//////////////////////////////////////////////////////
// ENUMS
//////////////////////////////////////////////////////

enum MenuID
{
	MENU_ID_EXIT = 1000,    
	MENU_ID_RENDERER_OPENGL = 1001,
	MENU_ID_RENDERER_D3D9 = 1002	,	
	MENU_ID_PUZZLE_GAME = 2000,
	MENU_ID_NETWORK_GAME = 2001,
	MENU_ID_RESEARCH = 2002
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class WindowsMenu
{
public:

	//
	// This constructor will initialize the application
	//
	WindowsMenu();

    //
    // Destructor
    //
    ~WindowsMenu();

    //
    // Create windows menu
    //
    void Create( HWND hWnd, RendererType rendererType, GameApp::GameType gameType );

    //
    // Process events
    //
    void ProcessEvents( int event, HWND hWnd );

    void SetResolutionConfig( const ResolutionConfig &resolutionConfig );
	
private:

    HMENU mMenuHandle;
	HMENU mRendererMenuHandle;
	HMENU mGameMenuHandle;
	HMENU mAppMenuHandle;
	MenuID mCurrentRendererMenuItem;
	MenuID mCurrentGameMenuItem;
	RendererType mCurrentRenderer;
	GameApp::GameType mCurrentGameType;
    ResolutionConfig mResolutionConfig;
};

#endif