#ifndef WINDOW_HPP
#define WINDOW_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <windows.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Window
{
public:

	//
	// We will use a default constructor to declare a window
	//
	Window();

	//
	// The Create() method will be used to initialize a window
	//
	HWND Create( 
		HINSTANCE hinst,
		LPCTSTR className,
		LPCTSTR windowName,
		HWND parent   = NULL,
		DWORD dStyle  = WS_OVERLAPPEDWINDOW,
		DWORD dXStyle = 0L,
		int x         = 200,
		int y         = 120,
		int width     = CW_USEDEFAULT,
		int height    = CW_USEDEFAULT
		);

	//
	// This method will be used to display the window
	//
	bool Show( int dCmdShow = SW_SHOWNORMAL );

	//
	// Because each window is of type HWND, we will need a way
	// to recognize the window handle when used in our application
	//
	operator HWND();

	//
	// Set the size of the window
	//
	void SetSize( int width, int height );

private:

	//
	// This will be a global handle available to this and other windows
	//
	HWND mHwnd;
};

#endif