#if defined PLAT_PC

#ifndef WINDOWS_APPLICATION_HPP
#define WINDOWS_APPLICATION_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <windows.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class WindowsApplication
{
public:

	//
	// This constructor will initialize the application
	//
	WindowsApplication( HINSTANCE hInst, wchar_t* className, WNDPROC wndPrc, LPCTSTR menuName = NULL );

    //
    // Destructor
    //
    ~WindowsApplication();

	//
	// Class Registration
	//
	void Register();

private:

	//
	// Global variable that holds the application
	//
	WNDCLASSEX mWindowsClassEx;
};

#endif

#endif