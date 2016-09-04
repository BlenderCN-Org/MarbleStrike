#ifndef GAMEAPP_HPP
#define GAMEAPP_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

#if defined PLAT_PC || defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <windows.h>
#endif

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
#include <agile.h>
#endif

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BlenderAssetManager;
class EffectManager;
class Font;
class FontManager;
class Game;
class InputManager;
class MessageHandler;
#if defined PLAT_PC
class NetworkManager;
#endif
class PCInputManager;
class ScriptManager;
class ShaderManager;
class SimpleDraw;
class RenderStateManager;
class TextureManager;
class RenderQueue;
class ResourceManager;
class TimeScheduler;

namespace System
{
	class Timer;	
	class TaskManager;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class GameApp
{
public:

	enum RenderState
	{
		RENDER_STATE_INITIALIZE,
		RENDER_STATE_MAIN
	};

	enum GameType
	{
		GAME_TYPE_PUZZLE,
		GAME_TYPE_NETWORK,
		GAME_TYPE_RESEARCH,		
		GAME_TYPE_GUIEDITOR,
		GAME_TYPE_PARTICLEEDITOR,
		GAME_TYPE_BOWLER
	};
	
	static void Create()
	{		
		Assert( mInstance == NULL, "" );
		mInstance = NEW_PTR( "GameApp" ) GameApp();		
	}

	static void Destroy()
	{
		Assert( mInstance != NULL, "" );
		DELETE_PTR( mInstance );
	}

	static GameApp* Get()
	{		
		return mInstance;
	}

	static void KeyboardChar( unsigned char key );
	static void KeyboardDown( unsigned char key );

    //===========================================================================
    // Application functions
    //===========================================================================    
	void CreateRenderer();
	void Initialize( GameType gameType );
	void Shutdown();
	void SetScreenWidth( int width );
	int GetScreenWidth();	
	void SetScreenHeight( int height );
	int GetScreenHeight();	
	void SetIsLanscape( bool value );
	bool GetIsLandscape();	
    void Resize( int width, int height );
	void Update();
	void Render();
    void SetHide();
    void SetShow();
	void SetVolume();
	void OnBackButton();
	bool IsExitable();

	InputManager* GetInputManager()
	{
		Assert( mInputManager != NULL, "" );
		return mInputManager;
	}	

	BlenderAssetManager* GetBlenderAssetManager()
	{
		Assert( mBlenderAssetManager != NULL, "" );
		return mBlenderAssetManager;
	}

	EffectManager* GetEffectManager()
	{
		Assert( mEffectManager != NULL, "" );
		return mEffectManager;
	}

	FontManager* GetFontManager()
	{
		Assert( mFontManager != NULL, "" );
		return mFontManager;
	}

	MessageHandler* GetMessageHandler()
	{
		Assert( mMessageHandler != NULL, "" );
		return mMessageHandler;			
	}

	RenderStateManager* GetRenderStateManager()
	{
		Assert( mRenderStateManager != NULL, "" );
		return mRenderStateManager;
	}

	SimpleDraw* GetSimpleDraw()
	{
		Assert( mSimpleDraw != NULL, "" );
		return mSimpleDraw;
	}

	ScriptManager* GetScriptManager()
	{
		Assert( mScriptManager != NULL, "" );
		return mScriptManager;
	}

	TextureManager* GetTextureManager()
	{
		Assert( mTextureManager != NULL, "" );
		return mTextureManager;
	}

	ShaderManager* GetShaderManager()
	{
		Assert( mShaderManager != NULL, "" );
		return mShaderManager;
	}

#if defined PLAT_PC
	NetworkManager* GetNetworkManager()
	{
		Assert( mNetworkManager != NULL, "" );
		return mNetworkManager;
	}
#endif

	System::TaskManager* GetTaskManager()
	{
		Assert( mTaskManager != NULL, "" );
		return mTaskManager;
	}

	RenderQueue* GetRenderQueue()
	{
		Assert( mRenderQueue != NULL, "" );
		return mRenderQueue;
	}

	ResourceManager* GetResourceManager()
	{
		Assert( mResourceManager != NULL, "" );
		return mResourceManager;
	}

	TimeScheduler* GetTimeScheduler()
	{
		Assert( mTimeScheduler != NULL, "" );
		return mTimeScheduler;
	}

	Game* GetGame()
	{
		Assert( mGame != NULL, "" );
		return mGame;
	}

	RendererType GetRendererType()
	{
		return mRendererType;
	}

	void SetRendererType( RendererType type )
	{
		mRendererType = type;
	}

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
	void SetCoreWindow( Windows::UI::Core::CoreWindow^ window )
	{
		mCoreWindow = window;
	}
#endif

#if defined PLAT_PC
	void SetHWND( HWND hWnd )
	{
		mHWnd = hWnd;
	}
#endif

#if defined( PLAT_ANDROID )
	void SetWindow( ANativeWindow* window )
	{
		mWindow = window;
	}
#endif

	static const char* GetResolutionString();

private:

	GameApp();
	~GameApp();

	static GameApp* mInstance;	
	
	RenderState mRenderState;	
	
	InputManager* mInputManager;
	BlenderAssetManager* mBlenderAssetManager;
	EffectManager* mEffectManager;
	MessageHandler* mMessageHandler;
	RenderStateManager* mRenderStateManager;
	SimpleDraw* mSimpleDraw;	
	ScriptManager* mScriptManager;	
	TextureManager* mTextureManager;
	ShaderManager* mShaderManager;	
#if defined PLAT_PC
	NetworkManager* mNetworkManager;
#endif
	System::Timer* mTimer;		
	RendererType mRendererType;
	FontManager* mFontManager;
	System::TaskManager* mTaskManager;
	RenderQueue* mRenderQueue;
	ResourceManager* mResourceManager;
	TimeScheduler* mTimeScheduler;

	int mFontIndex;
	Game* mGame;
    bool mIsShow;

#if defined PLAT_WINDOWS_PHONE || defined PLAT_WINDOWS_8
	Platform::Agile<Windows::UI::Core::CoreWindow> mCoreWindow;
#endif

#if defined PLAT_PC
	HWND mHWnd;
#endif

#if defined( PLAT_ANDROID )
	ANativeWindow* mWindow;
#endif

	int mScreenWidth;
	int mScreenHeight;
	bool mIsLandscape;
	float mSimulationTime;
};

#endif