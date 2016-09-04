#if defined( RENDER_PLAT_OGLES2 )

#if defined( PLAT_PC ) || defined( PLAT_ANDROID )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogles2/oglescontext.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

void CheckEGLError()
{
	EGLint eglError = eglGetError();	
	switch ( eglError )
	{
	case EGL_SUCCESS:
		System::PrintToConsole("EGL_SUCCESS\n"); 
		break;
	case EGL_NOT_INITIALIZED:
		System::PrintToConsole("EGL_NOT_INITIALIZED\n"); 
		break;
	case EGL_BAD_ACCESS:
		System::PrintToConsole("EGL_BAD_ACCESS\n"); 
		break;
	case EGL_BAD_ALLOC:
		System::PrintToConsole("EGL_BAD_ALLOC\n"); 
		break;
	case EGL_BAD_ATTRIBUTE:
		System::PrintToConsole("EGL_BAD_ATTRIBUTE\n"); 
		break;
	case EGL_BAD_CONTEXT:
		System::PrintToConsole("EGL_BAD_CONTEXT\n"); 
		break;
	case EGL_BAD_CONFIG:
		System::PrintToConsole("EGL_BAD_CONFIG\n"); 
		break;
	case EGL_BAD_CURRENT_SURFACE:
		System::PrintToConsole("EGL_BAD_CURRENT_SURFACE\n"); 
		break;
	case EGL_BAD_DISPLAY:
		System::PrintToConsole("EGL_BAD_DISPLAY\n"); 
		break;
	case EGL_BAD_SURFACE:
		System::PrintToConsole("EGL_BAD_SURFACE\n"); 
		break;
	case EGL_BAD_MATCH:
		System::PrintToConsole("EGL_BAD_MATCH\n"); 
		break;
	case EGL_BAD_PARAMETER:
		System::PrintToConsole("EGL_BAD_PARAMETER\n"); 
		break;
	case EGL_BAD_NATIVE_PIXMAP:
		System::PrintToConsole("EGL_BAD_NATIVE_PIXMAP\n"); 
		break;
	case EGL_BAD_NATIVE_WINDOW:
		System::PrintToConsole("EGL_BAD_NATIVE_WINDOW\n"); 
		break;
	case EGL_CONTEXT_LOST:
		System::PrintToConsole("EGL_CONTEXT_LOST\n"); 
		break;
	default:
		break;
	}
}

OGLESContext::OGLESContext()
{
	Reset();
}

//===========================================================================

OGLESContext::~OGLESContext()
{
	Terminate();
}

//===========================================================================

EGLConfig OGLESContext::SelectEGLConfiguration()
{
	EGLint num_config;
	EGLint	conflist[32];
	EGLConfig conf;
	int  i = 0;

	conflist[i++] = EGL_LEVEL;
	conflist[i++] = 0;

	conflist[i++] = EGL_BUFFER_SIZE;
	conflist[i++] = 0;

	conflist[i++] = EGL_DEPTH_SIZE;
	conflist[i++] = 16;
	
	conflist[i++] = EGL_STENCIL_SIZE;
	conflist[i++] = 8;
	
	conflist[i++] = EGL_SURFACE_TYPE;
	conflist[i++] = EGL_WINDOW_BIT;
		
	conflist[i++] = EGL_RENDERABLE_TYPE;
	conflist[i++] = EGL_OPENGL_ES2_BIT;

	conflist[i++] = EGL_SAMPLE_BUFFERS;
	conflist[i++] = 0;
	
	conflist[i++] = EGL_NONE;

	if ( !eglChooseConfig( mEglDisplay, conflist, &conf, 1, &num_config ) || num_config != 1 )
	{
		CheckEGLError();
		Assert( false, "" );
		return 0;
	}

	return conf;
}

//===========================================================================

#if defined( PLAT_ANDROID )
void OGLESContext::Initialize( ANativeWindow* window )
{
	EGLint w;
	EGLint h;
	EGLint dummy;
	EGLint format;
	EGLint numConfigs;
	EGLContext context;

	mEglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );

	eglInitialize( mEglDisplay,  &mMajorVersion, &mMinorVersion  );	

	if ( !eglBindAPI( EGL_OPENGL_ES_API ) )
	{
		Assert( false, "" );
	}

	//eglChooseConfig( mEglDisplay, attribs, &mEglConfig, 1, &numConfigs );
	//eglGetConfigAttrib( mEglDisplay, mEglConfig, EGL_NATIVE_VISUAL_ID, &format );
	mEglConfig = SelectEGLConfiguration();

	ANativeWindow_setBuffersGeometry( window, 0, 0, format );

	mEglWindow = eglCreateWindowSurface(mEglDisplay, mEglConfig, window, NULL);

	EGLint contextAttribs[] = 
	{ 
		EGL_CONTEXT_CLIENT_VERSION, 
		2, 
		EGL_NONE 
	};

	mEglContextMain = eglCreateContext( mEglDisplay, mEglConfig, EGL_NO_CONTEXT, contextAttribs );
	Assert( eglGetError() == EGL_SUCCESS, "" );
	mEglContextThread = eglCreateContext( mEglDisplay, mEglConfig, mEglContextMain, contextAttribs);
	Assert( eglGetError() == EGL_SUCCESS, "" );

	EGLBoolean contextResults = eglMakeCurrent(mEglDisplay, mEglWindow, mEglWindow, mEglContextMain);	
	Assert( contextResults != 0, "" );

	eglQuerySurface(mEglDisplay, mEglWindow, EGL_WIDTH, &w );
	eglQuerySurface(mEglDisplay, mEglWindow, EGL_HEIGHT, &h );

	Database::Get()->SetBackBufferWidth( w );
	Database::Get()->SetBackBufferHeight( h );
	Database::Get()->SetTouchScreenWidth( w );
	Database::Get()->SetTouchScreenHeight( h );
}
#endif

//===========================================================================

#if defined( PLAT_ANDROID )
void OGLESContext::DeInitialize()
{
	mEglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	eglInitialize( mEglDisplay,  &mMajorVersion, &mMinorVersion  );	

	EGLBoolean contextResults = eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);	
	Assert( contextResults != 0, "" );
}
#endif

//===========================================================================

#if defined( PLAT_ANDROID )
void OGLESContext::ReInitialize( ANativeWindow* window )
{
	EGLint format;

	mEglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	eglInitialize( mEglDisplay,  &mMajorVersion, &mMinorVersion  );	

	if ( !eglBindAPI( EGL_OPENGL_ES_API ) )
	{
		Assert( false, "" );
	}

	mEglConfig = SelectEGLConfiguration();
	ANativeWindow_setBuffersGeometry( window, 0, 0, format );
	mEglWindow = eglCreateWindowSurface(mEglDisplay, mEglConfig, window, NULL);
	EGLBoolean contextResults = eglMakeCurrent(mEglDisplay, mEglWindow, mEglWindow, mEglContextMain);	
	Assert( contextResults != 0, "" );
}
#endif

//===========================================================================

#if defined( PLAT_PC )
void OGLESContext::Initialize( HWND hWnd )
{
	//
	// Remember the window handle (HWND)
	//
	mHWnd = hWnd;

	//
	// Get the device context (DC)
	//
	mHDC = GetDC( mHWnd );

	EGLNativeDisplayType ndt;
	ndt = (EGLNativeDisplayType)mHDC;
	mEglDisplay = eglGetDisplay(ndt);    

	if ( !eglInitialize( mEglDisplay, &mMajorVersion, &mMinorVersion ) )
	{
		Assert( false, "" );
	}

	if ( !eglBindAPI(EGL_OPENGL_ES_API ) )
	{
		Assert( false, "" );
	}

	mEglConfig = SelectEGLConfiguration();

	EGLint contextAttribs[] = 
	{ 
		EGL_CONTEXT_CLIENT_VERSION, 
		2, 
		EGL_NONE 
	};

	mEglContextMain = eglCreateContext( mEglDisplay, mEglConfig, EGL_NO_CONTEXT, contextAttribs);
	Assert( eglGetError() == EGL_SUCCESS, "" );
	mEglContextThread = eglCreateContext( mEglDisplay, mEglConfig, mEglContextMain, contextAttribs);
	Assert( eglGetError() == EGL_SUCCESS, "" );

	EGLint attrib_list[16];
	int	i = 0;
	attrib_list[i] = EGL_NONE;

	EGLNativeWindowType nwt = (EGLNativeWindowType)hWnd;
	mEglWindow = eglCreateWindowSurface( mEglDisplay, mEglConfig, nwt, attrib_list );
	Assert( eglGetError() == EGL_SUCCESS, "" );

	EGLBoolean contextResults = eglMakeCurrent( mEglDisplay, mEglWindow, mEglWindow, mEglContextMain );
	UNUSED_ALWAYS( contextResults );
	Assert( contextResults != 0, "" );
}
#endif

//===========================================================================

void OGLESContext::Terminate()
{
}

//===========================================================================

void OGLESContext::SwapFrameBuffers()
{
	eglSwapBuffers( mEglDisplay, mEglWindow );
	Assert( eglGetError() == EGL_SUCCESS, "" );
}

//===========================================================================

void OGLESContext::Reset()
{
#if defined PLAT_PC
	mHWnd = NULL;
	mHDC = NULL;
	mHRC = NULL;
#endif

	mEglWindow = 0;
	mEglContextMain = 0;
	mEglContextThread = 0;
	mEglDisplay = 0;
	mMajorVersion = 0;
	mMinorVersion = 0;
	mEglConfig = 0;
	mAuxSurface = 0;
}

//===========================================================================

void OGLESContext::SetMainContext()
{       
	EGLBoolean contextResults = eglMakeCurrent( mEglDisplay, mEglWindow, mEglWindow, mEglContextMain );    
	UNUSED_ALWAYS( contextResults );
	Assert( contextResults != 0, "" );
}

//===========================================================================

void OGLESContext::SetThreadContext()
{       
	EGLBoolean contextResults = eglMakeCurrent( mEglDisplay, mEglWindow, mEglWindow, mEglContextThread );
	UNUSED_ALWAYS( contextResults );
	Assert( contextResults != 0, "" );    
}

//===========================================================================

void OGLESContext::SetEmptyContext()
{       
	eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);    
}

#endif

#endif
