#if defined( RENDER_PLAT_OPENGL )

#ifndef OGL_CONTEXT_HPP
#define OGL_CONTEXT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <windows.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLContext
{
public:

	OGLContext();
	~OGLContext();

    void Initialize( HWND hWnd );
	void Terminate();
	void SwapFrameBuffers();
    void SetCurrent();
    HGLRC GetContext();

private:

	void Reset();

	HWND	mHWnd;
	HDC		mHDC;
	HGLRC	mHRC;
};

#endif

#endif