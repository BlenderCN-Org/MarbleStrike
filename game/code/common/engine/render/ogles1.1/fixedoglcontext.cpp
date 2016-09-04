#if defined( RENDER_PLAT_OGLES1_1)

#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglcontext.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLContext::FixedOGLContext()
:	mEglDisplay( NULL )
,	mEglWindow( NULL )
,	mEglContext( NULL )
,	mEglConfig( NULL )
,	mMajorVersion( 0 )
,	mMinorVersion( 0 )
,	mPowerManagementSupported( false )
{
	Reset();
}

//===========================================================================

FixedOGLContext::~FixedOGLContext()
{
	Terminate();
}

//===========================================================================

void FixedOGLContext::Initialize( HWND hWnd )
{	
	HDC hDC = GetDC( hWnd );
	NativeDisplayType ndt = (NativeDisplayType)hDC;
	mEglDisplay = eglGetDisplay( ndt );

	if ( mEglDisplay == EGL_NO_DISPLAY)
	{
		mEglDisplay = eglGetDisplay((NativeDisplayType)EGL_DEFAULT_DISPLAY);
	}

	if ( !eglInitialize( mEglDisplay, &mMajorVersion, &mMinorVersion ) )
	{
		assert( false );			
	}

	// Check Extension availability after EGL initialization
	if ( mMajorVersion > 1 || ( mMajorVersion == 1 && mMinorVersion >= 1 ) )
	{
		mPowerManagementSupported = true;
	}
	else
	{
		//PVRShellIsExtensionSupported(gEglDisplay,"EGL_IMG_power_management");
		mPowerManagementSupported = false;
	}

	// Find an EGL config
	mEglConfig = SelectEGLConfiguration();

	// Destroy the context if we already created one
	if ( mEglContext )
	{
		eglDestroyContext( mEglDisplay, mEglContext );
	}

	// Attempt to create a context
	EGLint ai32ContextAttribs[32];
	int	i = 0;
	ai32ContextAttribs[i] = EGL_NONE;
	mEglContext = eglCreateContext( mEglDisplay, mEglConfig, NULL, ai32ContextAttribs );

	assert( mEglContext != EGL_NO_CONTEXT );

	EGLint attrib_list[16];
	i = 0;

	// Terminate the attribute list with EGL_NONE
	attrib_list[i] = EGL_NONE;

	NativeWindowType nwt = (NativeWindowType)hWnd;

	mEglWindow = eglCreateWindowSurface( mEglDisplay, mEglConfig, nwt, attrib_list);

	// If we have failed to create a surface then try using Null
	if ( mEglWindow == EGL_NO_SURFACE )
	{
		mEglWindow = eglCreateWindowSurface(mEglDisplay, mEglConfig, NULL, attrib_list);
	}

	if ( mEglWindow == EGL_NO_SURFACE )
	{
		assert( false );
	}

	if (!eglMakeCurrent(mEglDisplay, mEglWindow, mEglWindow, mEglContext))
	{
		assert( false );

	}

	//	eglQuerySurface( mEglDisplay, mEglWindow,
	//			EGL_WIDTH,  (EGLint*)&m_pShell->m_pShellData->nShellDimX
	//		);
	//	eglQuerySurface(gEglDisplay, gEglWindow,
	//			EGL_HEIGHT, (EGLint*)&m_pShell->m_pShellData->nShellDimY
	//		);
	//
	//	/*
	//		Done - activate requested features
	//	*/
	//	ApiActivatePreferences();
	eglSwapInterval( mEglDisplay, 1 );

	////
	//// Remember the window handle (HWND)
	////
	//mHWnd = hWnd;

	////
	//// Get the device context (DC)
	////
	//mHDC = GetDC( mHWnd );


	////
	//// Set the pixel format for the DC
	////
	//PIXELFORMATDESCRIPTOR pfd;
	//ZeroMemory( &pfd, sizeof( pfd ) );
	//pfd.nSize = sizeof( pfd );
	//pfd.nVersion = 1;
	//pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	//pfd.iPixelType = PFD_TYPE_RGBA;
	//pfd.cColorBits = 24;
	//pfd.cDepthBits = 16;
	//pfd.iLayerType = PFD_MAIN_PLANE;
	//int format = ChoosePixelFormat( mHDC, &pfd );
	//SetPixelFormat( mHDC, format, &pfd );

	////
	//// Create the render context (RC)
	////
	//mHRC = wglCreateContext( mHDC );

	////
	//// Make it the current render context
	////
	//wglMakeCurrent( mHDC, mHRC );
}

//===========================================================================

void FixedOGLContext::Terminate()
{
	Reset();
}

//===========================================================================

void FixedOGLContext::SwapFrameBuffers()
{
	eglSwapBuffers( mEglDisplay, mEglWindow );
}

//===========================================================================

EGLConfig FixedOGLContext::SelectEGLConfiguration()
{
	EGLint num_config;
	EGLint conflist[32];
	EGLConfig conf;
	int i;

	i = 0;

	/* Select default configuration */
	conflist[i++] = EGL_LEVEL;
	conflist[i++] = 0;

	conflist[i++] = EGL_NATIVE_RENDERABLE;
	conflist[i++] =  EGL_FALSE;

	conflist[i++] = EGL_BUFFER_SIZE;
	conflist[i++] = 0;

	conflist[i++] = EGL_DEPTH_SIZE;
	conflist[i++] = 16;

	//conflist[i++] = EGL_STENCIL_SIZE;
	//conflist[i++] = 8;

	conflist[i++] = EGL_SURFACE_TYPE;
	conflist[i] = EGL_WINDOW_BIT;

	//	if(pData->bNeedPbuffer)
	//	{
	//		conflist[i] |= EGL_PBUFFER_BIT;
	//	}

	i++;

	// Append number of number buffers depending on FSAA mode selected
	int test = 0;
	switch( test )
	{
	case 1:
		conflist[i++] = EGL_SAMPLE_BUFFERS;
		conflist[i++] = 1;
		conflist[i++] = EGL_SAMPLES;
		conflist[i++] = 2;
		break;

	case 2:
		conflist[i++] = EGL_SAMPLE_BUFFERS;
		conflist[i++] = 1;
		conflist[i++] = EGL_SAMPLES;
		conflist[i++] = 4;
		break;

	default:
		conflist[i++] = EGL_SAMPLE_BUFFERS;
		conflist[i++] = 0;
	}

	// Terminate the list with EGL_NONE
	conflist[i++] = EGL_NONE;

	// Return null config if config is not found
	if( !eglChooseConfig( mEglDisplay, conflist, &conf, 1, &num_config) || num_config != 1 )
	{
		return 0;
	}

	// Return config index
	return conf;
}

//===========================================================================

void FixedOGLContext::Reset()
{

}

#endif

#endif