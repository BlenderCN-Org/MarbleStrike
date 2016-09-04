//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/game.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/resourcemanager.hpp"
#include "common/engine/game/scriptmanager.hpp"
#include "common/engine/game/timescheduler.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include "common/engine/network/networkmanager.hpp"
#include "common/engine/render/blenderassetmanager.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/rendertarget.hpp"
#include "common/engine/render/renderutilities.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/system/debug/console.hpp"
#include "common/engine/system/debug/debugmenu.hpp"
#include "common/engine/system/debug/debugvariablehelper.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/memory/memorylog.hpp"
#include "common/engine/system/profiler.hpp"
#include "common/engine/system/profilermanager.hpp"
#include "common/engine/system/taskmanager.hpp"
#include "common/engine/system/timer.hpp"
#include "common/game/bowlergame/bowlergame.hpp"
#include "common/game/guieditor/guieditorapp.hpp"
#include "common/game/networkgame/networkgame.hpp"
#include "common/game/particleeditor/particleeditorapp.hpp"
#include "common/game/puzzlegame/puzzlegame.hpp"
#include "common/game/research/research.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

CVARBool( "GameApp", gShowFPS, true );
CVARBool( "GameApp", gShowMem, true );
CVARBool( "GameApp", gDumpMemoryLog, false );

GameApp* GameApp::mInstance = NULL;

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

GameApp::GameApp()
    :	mInputManager( NULL )
    ,	mEffectManager( NULL )
    ,	mMessageHandler( NULL )
    ,	mBlenderAssetManager( NULL )
    ,	mRenderStateManager( NULL )
    ,	mTimer( NULL )
    ,	mRendererType( RENDERER_OPENGL )
    ,	mScriptManager( NULL )
    ,	mSimpleDraw( NULL )
    ,	mTextureManager( NULL )
    ,	mShaderManager( NULL )
#if defined PLAT_PC
    ,	mNetworkManager( NULL )    
#endif
    ,	mTaskManager( NULL )
    ,	mRenderQueue( NULL )
    ,	mRenderState( RENDER_STATE_INITIALIZE )
    ,	mResourceManager( NULL )
	,	mTimeScheduler( NULL )
    ,   mIsShow( true )
	,	mSimulationTime( 0 )
{	
	mScreenWidth = 0;
	mScreenHeight = 0;
	mIsLandscape = false;
}

//===========================================================================

GameApp::~GameApp()
{
}

//===========================================================================

void GameApp::KeyboardDown( unsigned char key )
{
    InputManager* inputManager = GameApp::Get()->GetInputManager();

    switch ( inputManager->GetInputStateType() )
    {
    case INPUT_GAME_STATE:		
        break;
    case INPUT_CONSOLE_STATE:
        {
            System::Debug::Console::Get()->KeyboardDown( key );			
        }
        break;
    case INPUT_DEBUG_MENU_STATE:
        {
            System::Debug::DebugMenu::Get()->KeyboardDown( key );
        }
        break;
    default:
        break;
    }
}

//===========================================================================

void GameApp::KeyboardChar( unsigned char key )
{
    InputManager* inputManager = GameApp::Get()->GetInputManager();

    switch ( inputManager->GetInputStateType() )
    {
    case INPUT_GAME_STATE:
        {
            if ( key == '`' || key == '~' )
            {			
                inputManager->SetInputState( INPUT_CONSOLE_STATE );			
            }

            if ( key == 'm' )
            {			
                inputManager->SetInputState( INPUT_DEBUG_MENU_STATE );
            }
        }
        break;
    case INPUT_CONSOLE_STATE:
        {
            System::Debug::Console::Get()->KeyboardChar( key );

            if ( key == '`' || key == '~' )
            {		
                inputManager->SetInputState( INPUT_GAME_STATE );
            }		
        }
        break;
    case INPUT_DEBUG_MENU_STATE:
        {
            System::Debug::DebugMenu::Get()->KeyboardChar( key );

            if ( key ==  'm' )
            {			
                inputManager->SetInputState( INPUT_GAME_STATE );
            }
        }
        break;
    default:
        break;
    }
}

//===========================================================================

void GameApp::CreateRenderer()
{

#if defined PLAT_IOS
#if defined RENDER_PLAT_OGLES1_1
    mRendererType = RENDERER_OPENGLES1_1;
#endif
#if defined RENDER_PLAT_OGLES2
    mRendererType = RENDERER_OPENGLES2;
#endif
#endif

#if defined PLAT_XBOX360
    mRendererType = RENDERER_XBOX360;
#endif

#if defined PLAT_PS3
    mRendererType = RENDERER_PS3;
#endif

#if defined RENDER_PLAT_D3D11
    mRendererType = RENDERER_D3D11;
#endif

    Renderer::Create( mRendererType );
};

//===========================================================================

void GameApp::Initialize( GameType gameType )
{
    System::ProfilerManager::Create();

    switch ( gameType )
    {
    case GameApp::GAME_TYPE_PUZZLE:
#if defined PLAT_PC
        mGame = NEW_PTR( "PuzzleGame" ) PuzzleGame;		
#endif
        break;
    case GameApp::GAME_TYPE_NETWORK:
#if defined PLAT_PC
        mGame = NEW_PTR( "NetworkGame" ) NetworkGame;		
#endif			
        break;
    case GameApp::GAME_TYPE_RESEARCH:
#if defined PLAT_PC
        mGame = NEW_PTR( "Research" ) Research;
#endif
        break;
    case GameApp::GAME_TYPE_GUIEDITOR:
#if defined PLAT_PC
        mGame = NEW_PTR( "Gui Editor" ) GuiEditorApp;
#endif
        break;
    case GameApp::GAME_TYPE_PARTICLEEDITOR:
#if defined PLAT_PC
        mGame = NEW_PTR( "Particle Editor" ) ParticleEditorApp;
#endif
        break;
    case GameApp::GAME_TYPE_BOWLER:
        mGame = NEW_PTR( "Bowler" ) BowlerGame;
        break;
    default:
        mGame = NULL;
        Assert( false, "No type of the following found." );
        break;
    }	
    
    Database::Get()->SetAppScreenWidth( mScreenWidth );
    Database::Get()->SetAppScreenHeight( mScreenHeight );
    
    Database::Get()->LoadStartupData( "startup" );
    const StartupData* startupData = Database::Get()->GetStartupData();
    
    gShowFPS.SetBool( startupData->mShowFPS );    
    gShowMem.SetBool( startupData->mShowMem );
    
    mTaskManager = NEW_PTR( "Task Manager" ) System::TaskManager;    
    mTaskManager->Initialize();

#if defined( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 )
	Renderer::Get()->Initialize( mCoreWindow );	
#endif

#if defined( PLAT_PC )
    Renderer::Get()->Initialize( mHWnd );
#endif

#if defined( PLAT_XBOX360 ) || defined( PLAT_PS3 ) || defined( PLAT_IOS )
    Renderer::Get()->Initialize();
#endif

#if defined( PLAT_ANDROID )
	Renderer::Get()->Initialize( mWindow );
#endif

    mResourceManager = NEW_PTR( "Resource Manager" ) ResourceManager;
    mRenderQueue = Renderer::Get()->GetRenderFactory()->CreateRenderQueue();

    mInputManager = InputManager::CreateInputManager();	
    mInputManager->AddKeyboardDownCallback( &GameApp::KeyboardDown );
    mInputManager->AddKeyboardCharCallback( &GameApp::KeyboardChar );

    mShaderManager = NEW_PTR( "Shader Manager" ) ShaderManager;
    mTextureManager = NEW_PTR( "Texture Manager" ) TextureManager;	

    mTimer = NEW_PTR( "Timer" ) System::Timer;
    mTimer->Start();

    mRenderStateManager = NEW_PTR( "State Manager" ) RenderStateManager;

    mBlenderAssetManager = NEW_PTR( "BlenderAssetManager" ) BlenderAssetManager;
    mEffectManager = NEW_PTR( "Effect Manager" ) EffectManager;		

    mFontManager = NEW_PTR( "Font Manager" ) FontManager;
    //cooperblack
    //mFontIndex = mFontManager->AddFont( "borisblackbloxx" );
	mFontIndex = mFontManager->AddFont( "kgthelasttime" );

    mMessageHandler = NEW_PTR( "Message Handler" ) MessageHandler;

    mSimpleDraw = NEW_PTR( "Simple Draw" ) SimpleDraw;

    System::Debug::DebugMenu::Create();

    mScriptManager = NEW_PTR( "Script Manager" ) ScriptManager;
    mScriptManager->Initialize();

	mTimeScheduler = NEW_PTR( "Time Scheduler" ) TimeScheduler;
	mTimeScheduler->Initialize();

#if defined PLAT_PC
    mNetworkManager = NEW_PTR( "Network Manager" ) NetworkManager;
    mNetworkManager->Initialize();	
#endif

    Assert( mGame != NULL, "" );
    mGame->Initialize();

}

//===========================================================================

void GameApp::Shutdown()
{   
    DELETE_PTR( mGame );

    System::Debug::Console::Destroy();	

#if defined PLAT_PC
    mNetworkManager->Shutdown();
    DELETE_PTR( mNetworkManager );
#endif

    mScriptManager->Shutdown();
    DELETE_PTR( mScriptManager );

    System::Debug::DebugMenu::Destroy();
    DELETE_PTR( mSimpleDraw );

    DELETE_PTR( mMessageHandler );	

    DELETE_PTR( mFontManager );		
    mEffectManager->Clear();
    DELETE_PTR( mEffectManager );	
    DELETE_PTR( mBlenderAssetManager );
    DELETE_PTR( mRenderStateManager );
    DELETE_PTR( mTimer );	
    DELETE_PTR( mTextureManager );
    DELETE_PTR( mShaderManager );
    DELETE_PTR( mInputManager );	
    DELETE_PTR( mRenderQueue );
    DELETE_PTR( mTaskManager );
    DELETE_PTR( mResourceManager );
	DELETE_PTR( mTimeScheduler );

    Renderer::Destroy();	

    System::ProfilerManager::Destroy();
}

//===========================================================================

void GameApp::SetScreenWidth( int width )
{
	mScreenWidth = width;
}

//===========================================================================

int GameApp::GetScreenWidth()
{
    Assert( mScreenWidth != 0, "Invalid Screen Width" );
    return mScreenWidth;
}

//===========================================================================

void GameApp::SetScreenHeight( int height )
{
	mScreenHeight = height;
}

//===========================================================================

int GameApp::GetScreenHeight()
{
    Assert( mScreenHeight != 0, "Invalid Screen Height" );
    return mScreenHeight;
}

//===========================================================================

void GameApp::Resize( int width, int height )
{
    Database::Get()->SetBackBufferWidth( width );
    Database::Get()->SetBackBufferHeight( height );
}

//===========================================================================

void GameApp::SetIsLanscape( bool value )
{
	mIsLandscape = value;
}

//===========================================================================

bool GameApp::GetIsLandscape()
{
	return mIsLandscape;
}

//===========================================================================

//void GameApp::Update()
//{
//    if ( mIsShow == false )
//    {
//        return;
//    }
//
//	float elapsedTime = 1.0f / 60.0f;
//    //float elapsedTime = mTimer->GetElapsedTime();
//
//	mTimeScheduler->Update( elapsedTime);
//
//    mMessageHandler->DispatchMsg();
//    //mNetworkManager->Update();
//
//    mInputManager->Update();
//
//    int index2 = System::ProfilerManager::Get()->BeginProfiling( "GameApp::Update" );
//    
//    int index0 = System::ProfilerManager::Get()->BeginProfiling( "GameApp::Update::Update" );
//    mGame->Update( elapsedTime );    
//    System::ProfilerManager::Get()->EndProfiling( index0 );
//    
//    int index1 = System::ProfilerManager::Get()->BeginProfiling( "GameApp::Update::Render" );
//    Render();
//    System::ProfilerManager::Get()->EndProfiling( index1 );
//
//    System::ProfilerManager::Get()->EndProfiling( index2 );
//    
//#if defined PLAT_PC    
//    if ( gDumpMemoryLog )
//    {
//        System::Memory::DumpMemoryLog();
//        gDumpMemoryLog.SetBool( false );
//    }
//
//    float totalElapsedTime = mTimer->GetElapsedTime();
//    while ( totalElapsedTime < elapsedTime )
//    {
//        totalElapsedTime += mTimer->GetElapsedTime();
//    }
//#endif
//}

void GameApp::Update()
{
	if ( mIsShow == false )
	{
		return;
	}

#if defined( PLAT_PC )
    const float fixedTime = 1.0f / 60.0f;
	float realTime = mTimer->GetElapsedTime();
	mSimulationTime += realTime;
	int loop = 0;
	const int maxLoop = 10;
	while ( mSimulationTime >= fixedTime && loop < maxLoop )
	{		
		mSimulationTime -= fixedTime;
		
		mMessageHandler->DispatchMsg();
		mInputManager->Update();

        mGame->UpdateVariable( fixedTime );
		mGame->Update( fixedTime );                

		loop++;
	}	
#endif
	
#if defined( PLAT_IOS ) || defined( PLAT_ANDROID ) || defined( PLAT_WINDOWS_PHONE )
    const float fixedTime = 1.0f / 60.0f;
	//float realTime = mTimer->GetElapsedTime();
	//mSimulationTime += realTime;
	mGame->UpdateVariable( fixedTime );
	//int loop = 0;
	//const int maxLoop = 10;
	//while ( mSimulationTime >= fixedTime && loop < maxLoop )
	{		
		//mSimulationTime -= fixedTime;
		
		mMessageHandler->DispatchMsg();		
		//mNetworkManager->Update();
		mInputManager->Update();
		mGame->Update( fixedTime );        

		//loop++;
	}	
#endif

	Render();
}

//===========================================================================

void GameApp::Render()
{
    switch ( mRenderState )
    {
    case RENDER_STATE_INITIALIZE:
        {
            mRenderState = RENDER_STATE_MAIN;
        }		
    case RENDER_STATE_MAIN:
        {
            mGame->RenderUpdate();

            //
            // Draw text
            //
            mTimer->UpdateFPS();

            float height = static_cast<float>( Database::Get()->GetAppScreenHeight() );
            float width = static_cast<float>( Database::Get()->GetAppScreenWidth() );

#if defined ( PLAT_PC ) || defined ( PLAT_IOS ) || defined ( PLAT_WINDOWS_PHONE ) || defined( PLAT_WINDOWS_8 ) || defined( PLAT_ANDROID )
            float x = 20;
#endif
#if defined PLAT_PS3 || defined PLAT_XBOX360
            float x = 50;
#endif
            Font* font = mFontManager->GetFont( mFontIndex );

            font->SetSize( 0.5 );

            if ( gShowFPS )
            {
                float fps = static_cast<float>( mTimer->GetFPS() );			
                font->AddToBuffer( 
                    0, 
                    0, 
                    width, 
                    height, 					
                    TEXT_HORIZONTAL_ALIGN_CENTRE, 
                    TEXT_VERTICAL_ALIGN_TOP,
                    TEXT_COLOR_RED, 
                    "fps: %3.2f", 
                    fps 
                    );
            }

            if ( gShowMem )
            {
#if defined PLAT_PC
                size_t used = System::MemoryLog::GetInstance()->GetMemoryUsed();
                font->AddToBuffer( 
                    0, 
                    0, 
                    width, 
                    height, 
                    TEXT_HORIZONTAL_ALIGN_LEFT, 
                    TEXT_VERTICAL_ALIGN_TOP, 
                    TEXT_COLOR_RED, 
                    "mem: %3.2fMB", 
                    used / (1024.0f * 1024.0f) 
                    );
#endif
            }

            if ( mInputManager->GetInputStateType() == INPUT_CONSOLE_STATE )
            {
                font->AddToBuffer( 
                    x, 
                    55, 
                    500, 
                    55, 
                    TEXT_HORIZONTAL_ALIGN_LEFT, 
                    TEXT_VERTICAL_ALIGN_BOTTOM, 
                    TEXT_COLOR_RED, "> %s_", 
                    System::Debug::Console::Get()->GetCurrentLine( 0 ) 
                    );

                font->AddToBuffer( 
                    x, 
                    35, 
                    500, 
                    55, 
                    TEXT_HORIZONTAL_ALIGN_LEFT, 
                    TEXT_VERTICAL_ALIGN_BOTTOM,
                    TEXT_COLOR_RED, "%s", 
                    System::Debug::Console::Get()->GetCurrentLine( 1 ) 
                    );			
            }

            if ( mInputManager->GetInputStateType() == INPUT_DEBUG_MENU_STATE )
            {
                System::Debug::DebugMenu::Get()->Render();
            }

            float screenWidthSize = static_cast<float>( Database::Get()->GetAppScreenWidth() );
            float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );

            Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection( 
                0,
                0,
                screenWidthSize,
                screenHeightSize,
                -100,
                100
                );
            orthoMatrix = Render::ProjectionOrientation( orthoMatrix );

            Math::Matrix44 transform;
            transform.SetIdentity();

            mFontManager->RenderFonts( orthoMatrix, transform );

            mRenderQueue->Render();

            mSimpleDraw->Clear();            
            mFontManager->ClearBuffers();

            Renderer::Get()->SwapFrameBuffers();
        }
        break;
    default:		
        break;
    }	

}

//===========================================================================

void GameApp::SetHide()
{
    mIsShow = false;
    mGame->Hide();
}

//===========================================================================

void GameApp::SetShow()
{
    mIsShow = true;
    mGame->Show();
}

//===========================================================================

void GameApp::SetVolume()
{
	mGame->SetVolume();
}

//===========================================================================

void GameApp::OnBackButton()
{
	mGame->OnBackButton();
}

//===========================================================================

bool GameApp::IsExitable()
{
	return mGame->IsExitable();
}

//===========================================================================

const char* GameApp::GetResolutionString()
{
    int screenWidth = Database::Get()->GetAppScreenWidth();
	int screenHeight = Database::Get()->GetAppScreenHeight();

	if ( screenWidth == 960 && screenHeight == 640 )
    {
        return "_960x640";
    }
    else if ( screenWidth == 1136 && screenHeight == 640 )
    {
        return "_1136x640";
    }
    else if ( screenWidth == 1280 && screenHeight == 720 )
    {
        return "_1280x720";
    }
	else if ( screenWidth == 1024 && screenHeight == 768 )
    {
        return "_1024x768";
    }
	else if ( screenWidth == 2048 && screenHeight == 1536 )
    {
        return "_2048x1536";
    }
    else
    {
        return "";
    }
}