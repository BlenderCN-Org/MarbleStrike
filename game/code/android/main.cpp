//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include "common/engine/system/memory.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "android/input/androidinputmanager.hpp"
#include "common/engine/game/resolutionconfigs.hpp"
#include "android/androidassetmanager.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

#if defined ( _DEBUG )
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))
#else
#define LOGI(...)
#define LOGW(...)
#endif

static bool gDoReInitialize = false;
static bool gDoInitialize = false;
static bool gDoShutdown = false;
static bool gDoLoop = false;
static bool gWindowTerminate = false;
static bool gIsPaused = false;
static bool gInFocus = false;
static bool gIsInitialized = false;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// GLOBAL FUNCTIONS
//////////////////////////////////////////////////////

static int32_t engine_handle_input(struct android_app* app, AInputEvent* event) 
{
	if ( AInputEvent_getType( event ) == AINPUT_EVENT_TYPE_MOTION )
	{
		int32_t action = AMotionEvent_getAction( event );

		if (
			 ( action & AMOTION_EVENT_ACTION_MASK ) == AMOTION_EVENT_ACTION_DOWN ||
			 ( action & AMOTION_EVENT_ACTION_MASK ) == AMOTION_EVENT_ACTION_POINTER_DOWN
		   )
		{
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
				float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
				float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
				float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

				size_t pointerCount = AMotionEvent_getPointerCount( event );
				size_t pointerIndex = ( action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK ) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int32_t pointerId = AMotionEvent_getPointerId( event, pointerIndex );
				float x = AMotionEvent_getX( event, pointerIndex );
				float y = AMotionEvent_getY( event, pointerIndex );

				float xFloat = ( appScreenWidth / touchScreenWidth ) * x;
				float yFloat = ( appScreenHeight / touchScreenHeight ) * y;
				int xInt = static_cast<int>( xFloat );
				int yInt = static_cast<int>( yFloat );

				LOGI( "Received down event from pointer %i %i %i: (%.2f, %.2f)", pointerIndex, pointerId, pointerCount, x, y );

				gameApp->GetInputManager()->GetInputManagerPlat()->TouchDown( pointerId, xInt, yInt );
			}
		}
		else if (
				( action & AMOTION_EVENT_ACTION_MASK ) == AMOTION_EVENT_ACTION_UP ||
				( action & AMOTION_EVENT_ACTION_MASK ) == AMOTION_EVENT_ACTION_POINTER_UP
				)
		{
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
				float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
				float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
				float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

				size_t pointerCount = AMotionEvent_getPointerCount( event );
				size_t pointerIndex = ( action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK ) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
				int32_t pointerId = AMotionEvent_getPointerId( event, pointerIndex );
				float x = AMotionEvent_getX( event, pointerIndex );
				float y = AMotionEvent_getY( event, pointerIndex );

				float xFloat = ( appScreenWidth / touchScreenWidth ) * x;
				float yFloat = ( appScreenHeight / touchScreenHeight ) * y;
				int xInt = static_cast<int>( xFloat );
				int yInt = static_cast<int>( yFloat );

#if defined ( _DEBUG )
				LOGI( "Received up event from pointer %i %i %i: (%.2f, %.2f)", pointerIndex, pointerId, pointerCount, x, y );
#endif

				gameApp->GetInputManager()->GetInputManagerPlat()->TouchUp( pointerId, xInt, yInt );
			}
		}
		else if ( ( action & AMOTION_EVENT_ACTION_MASK ) == AMOTION_EVENT_ACTION_MOVE )
		{
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				float appScreenWidth = static_cast<float>( Database::Get()->GetAppScreenWidth() );
				float appScreenHeight = static_cast<float>( Database::Get()->GetAppScreenHeight() );
				float touchScreenWidth = static_cast<float>( Database::Get()->GetTouchScreenWidth() );
				float touchScreenHeight = static_cast<float>( Database::Get()->GetTouchScreenHeight() );

				size_t pointerCount = AMotionEvent_getPointerCount( event );
				for ( size_t i = 0; i < pointerCount; ++i )
				{
					int32_t pointerId = AMotionEvent_getPointerId( event, i );
					float x = AMotionEvent_getX( event, i );
					float y = AMotionEvent_getY( event, i );

					float xFloat = ( appScreenWidth / touchScreenWidth ) * x;
					float yFloat = ( appScreenHeight / touchScreenHeight ) * y;
					int xInt = static_cast<int>( xFloat );
					int yInt = static_cast<int>( yFloat );

#if defined ( _DEBUG )
					LOGI( "Received move event from pointer %i %i %i: (%.2f, %.2f)", i, pointerId, pointerCount, x, y );
#endif

					gameApp->GetInputManager()->GetInputManagerPlat()->TouchMove( pointerId, xInt, yInt );
				}
			}
		}

		return 1;
	}
	else if ( AKeyEvent_getKeyCode(event) == AKEYCODE_BACK )
	{
		int32_t action = AMotionEvent_getAction( event );

		if ( ( action & AMOTION_EVENT_ACTION_MASK ) == AMOTION_EVENT_ACTION_DOWN )
		{
			// actions on back key
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				if ( !gameApp->IsExitable() )
				{
					gameApp->OnBackButton();
					return 1; // <-- prevent default handler
				}
			}
		}
    }
	//else if ( AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY )
	{
		LOGW("Received key event: %d", AKeyEvent_getKeyCode(event));

		/*
		if ( AKeyEvent_getKeyCode(event) == 24 )
		{
			//
			// Up volume
			//
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				float volume = Database::Get()->GetVolume();
				volume += 0.2;
				Database::Get()->SetVolume( volume );
				GameApp::Get()->SetVolume();
			}
		}
		else if ( AKeyEvent_getKeyCode(event) == 25 )
		{
			//
			// Down volume
			//
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				float volume = Database::Get()->GetVolume();
				volume -= 0.2;
				Database::Get()->SetVolume( volume );
				GameApp::Get()->SetVolume();
			}
		}
		*/

		//return 1;
	}

	return 0;
}

static void engine_handle_cmd( struct android_app* app, int32_t cmd ) 
{
	switch ( cmd ) 
	{
	case APP_CMD_SAVE_STATE:
		LOGW( "APP_CMD_SAVE_STATE" );
		break;
	case APP_CMD_INIT_WINDOW:
		LOGW( "APP_CMD_INIT_WINDOW" );

		if ( gDoLoop && app->window != NULL)
		{
			gDoReInitialize = true;
		}

		if ( !gDoLoop && app->window != NULL )
		{
			gDoInitialize = true;
		}

		break;
	case APP_CMD_TERM_WINDOW:
		LOGW( "APP_CMD_TERM_WINDOW" );
		gWindowTerminate = true;
		break;
	case APP_CMD_GAINED_FOCUS:
		{
			LOGW( "APP_CMD_GAINED_FOCUS" );
			gInFocus = true;
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				GameApp::Get()->SetShow();
			}
		}
		break;
	case APP_CMD_LOST_FOCUS:
		{
			LOGW( "APP_CMD_LOST_FOCUS" );
			gInFocus = false;
			GameApp* gameApp = GameApp::Get();
			if ( gameApp )
			{
				GameApp::Get()->SetHide();
				Database::Get()->SetWasSuspended( true );
			}
		}
		break;
	case APP_CMD_INPUT_CHANGED:
		LOGW( "APP_CMD_INPUT_CHANGED" );
		break;
	case APP_CMD_WINDOW_RESIZED:
		LOGW( "APP_CMD_WINDOW_RESIZED" );
		break;
	case APP_CMD_WINDOW_REDRAW_NEEDED:
		LOGW( "APP_CMD_WINDOW_REDRAW_NEEDED" );
		break;
	case APP_CMD_CONTENT_RECT_CHANGED:
		LOGW( "APP_CMD_CONTENT_RECT_CHANGED" );
		break;
	case APP_CMD_CONFIG_CHANGED:
		LOGW( "APP_CMD_CONFIG_CHANGED" );
		break;
	case APP_CMD_LOW_MEMORY:
		LOGW( "APP_CMD_LOW_MEMORY" );
		break;
	case APP_CMD_START:
		LOGW( "APP_CMD_START" );
		break;
	case APP_CMD_RESUME:
		{
			LOGW( "APP_CMD_RESUME" );
			gIsPaused = false;
		}
		break;
	case APP_CMD_PAUSE:
		{
			LOGW( "APP_CMD_PAUSE" );
			gIsPaused = true;
		}
		break;
	case APP_CMD_STOP:
		LOGW( "APP_CMD_STOP" );
		break;
	case APP_CMD_DESTROY:
		LOGW( "APP_CMD_DESTROY" );
		gDoShutdown = true;
		break;
	}
}

void android_main(struct android_app* androidApp ) 
{
	// Make sure glue isn't stripped.
	app_dummy();
	
	androidApp->onAppCmd = engine_handle_cmd;
	androidApp->onInputEvent = engine_handle_input;

	bool doExit = false;
	while ( !doExit ) 
	{   
		int ident;
		int fdesc;
		int events;
		struct android_poll_source* source;

		while( ( ident = ALooper_pollAll( 0, &fdesc, &events, (void**)&source) ) >= 0 )
		{
			// process this event
			if (source)
			{
				source->process( androidApp, source );
			}
		}

		if ( gIsPaused || !gInFocus )
		{
			if ( gDoShutdown )
			{
				doExit = true;
			}
			continue;
		}

		if ( gWindowTerminate && gIsInitialized )
		{
			Renderer::Get()->DeInitialize();
			gWindowTerminate = false;
		}

		if ( gDoReInitialize )
		{
			Renderer::Get()->ReInitialize( androidApp->window );
			gDoReInitialize = false;
		}

		if ( gDoInitialize && gInFocus )
		{
		    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		    eglInitialize(display, 0, 0);

		    ResolutionConfig resolutionConfig = gAndroid;

            AndroidAssetManager::Create();
            AndroidAssetManager::GetInstance()->SetAndroidApp( androidApp );

		    Database::Create();
		    Database::Get()->SetBackBufferWidth( resolutionConfig.mFrameBufferWidth );
		    Database::Get()->SetBackBufferHeight( resolutionConfig.mFrameBufferHeight );
		    Database::Get()->SetTouchScreenWidth( resolutionConfig.mFrameBufferWidth );
		    Database::Get()->SetTouchScreenHeight( resolutionConfig.mFrameBufferHeight );

		    std::string internalDataPath = androidApp->activity->internalDataPath;
		    internalDataPath += "/";
		    Database::Get()->SetLibraryPath( internalDataPath.c_str() );
		    Database::Get()->SetResourcePath( internalDataPath.c_str() );

		    JNIEnv *env;
		    androidApp->activity->vm->AttachCurrentThread(&env, NULL);
		    jclass activityClass = env->FindClass("android/app/NativeActivity");
		    jmethodID setVolumeControlStream = env->GetMethodID(activityClass,"setVolumeControlStream", "(I)V");
		    jobject returnValue = env->CallObjectMethod(androidApp->activity->clazz, setVolumeControlStream, 3);

		    GameApp::Create();
		    GameApp* gameApp = GameApp::Get();
		    gameApp->SetScreenWidth( resolutionConfig.mGameScreenWidth );
		    gameApp->SetScreenHeight( resolutionConfig.mGameScreenHeight );
		    gameApp->SetWindow( androidApp->window );
		    gameApp->SetRendererType( RENDERER_OPENGLES2 );
		    gameApp->CreateRenderer();
		    gameApp->Initialize( GameApp::GAME_TYPE_BOWLER );

		    gDoLoop = true;
			gDoInitialize = false;
			gIsInitialized = true;
		}

		if ( gDoShutdown )
		{
			gDoLoop = false;

			GameApp::Get()->Shutdown();
			GameApp::Destroy();	
			Database::Terminate();
			//System::Memory::CheckMemoryLog();
			//System::Memory::Destroy();
            AndroidAssetManager::Destroy();

			doExit = true;
		}

		if ( gDoLoop )
		{
			GameApp::Get()->Update();
		}
	}

	ANativeActivity_finish(androidApp->activity);
	exit(0);
}
//END_INCLUDE(all)
