#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLES_CONTEXT_HPP
#define OGLES_CONTEXT_HPP

#if defined( PLAT_PC ) || defined( PLAT_ANDROID )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC
#include <windows.h>
#include "opengles2/include/egl/egl.h"
#endif

#if defined PLAT_ANDROID
#include <android_native_app_glue.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#endif

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLESContext
{
public:

	OGLESContext();
	~OGLESContext();


#if defined( PLAT_ANDROID )
	void Initialize( ANativeWindow* window );
	void DeInitialize();
	void ReInitialize( ANativeWindow* window );
#endif

#if defined( PLAT_PC )
	void Initialize( HWND hWnd );
#endif
    
    void Terminate();
    void SwapFrameBuffers();

	EGLConfig SelectEGLConfiguration();

    void SetMainContext();
    void SetThreadContext();
    void SetEmptyContext();

private:
    
	void Reset();

#if defined PLAT_PC
	HWND mHWnd;
	HDC	mHDC;
	HGLRC mHRC;
#endif

	EGLSurface mEglWindow;
	EGLContext mEglContextMain;
    EGLContext mEglContextThread;
	EGLDisplay mEglDisplay;
	EGLint mMajorVersion;
    EGLint mMinorVersion;
	EGLConfig mEglConfig;
    EGLSurface mAuxSurface;
};

#endif

#endif

#endif