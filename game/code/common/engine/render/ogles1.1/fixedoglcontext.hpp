#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_OGL_CONTEXT_HPP
#define FIXED_OGL_CONTEXT_HPP

#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "opengles1.1/include/gles/egl.h"
#include <winsock.h>
#include <windows.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class FixedOGLContext
{
public:
    
	FixedOGLContext();
	~FixedOGLContext();

	void Initialize( HWND hWnd );	
	void Terminate();    
	void SwapFrameBuffers();

private:

	EGLConfig SelectEGLConfiguration();    
	void Reset();

	EGLDisplay mEglDisplay;
	EGLSurface mEglWindow;
	EGLContext mEglContext;
	EGLConfig mEglConfig;
	EGLint mMajorVersion; 
	EGLint mMinorVersion;
	bool mPowerManagementSupported;

	
};

#endif

#endif

#endif