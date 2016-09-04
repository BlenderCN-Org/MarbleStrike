#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "oglcontext.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLContext::OGLContext()
{
	Reset();
}

//===========================================================================

OGLContext::~OGLContext()
{
	Terminate();
}

//===========================================================================

void OGLContext::Initialize( HWND hWnd )
{
	//
	// Remember the window handle (HWND)
	//
	mHWnd = hWnd;

	//
	// Get the device context (DC)
	//
	mHDC = GetDC( mHWnd );

	//
	// Set the pixel format for the DC
	//
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat( mHDC, &pfd );
	BOOL returnValue = SetPixelFormat( mHDC, format, &pfd );
	Assert( returnValue == 1, "" );
    UNUSED_ALWAYS( returnValue );

	//
	// Create the render context (RC)
	//
	mHRC = wglCreateContext( mHDC );
}

//===========================================================================

void OGLContext::Terminate()
{
	if ( mHRC )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( mHRC );
	}
	if ( mHWnd && mHDC )
	{
		ReleaseDC( mHWnd, mHDC );
	}

	Reset();
}

//===========================================================================

void OGLContext::SwapFrameBuffers()
{
	SwapBuffers( mHDC );
}

//===========================================================================

void OGLContext::SetCurrent()
{
	//
	// Make it the current render context
	//
	wglMakeCurrent( mHDC, mHRC );
}

//===========================================================================

void OGLContext::Reset()
{
	mHWnd = NULL;
	mHDC = NULL;
	mHRC = NULL;
}

//===========================================================================

HGLRC OGLContext::GetContext()
{
    return mHRC;
}

#endif