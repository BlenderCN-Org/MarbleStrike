#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/stringutilities.hpp"
#include "winapp.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

WindowsApplication::WindowsApplication( HINSTANCE hInst, wchar_t* className, WNDPROC wndPrc, LPCTSTR menuName )
{
	//
	// Initializing the application using the application member variable
	//
	mWindowsClassEx.cbSize        = sizeof( WNDCLASSEX );
	mWindowsClassEx.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
	mWindowsClassEx.lpfnWndProc   = wndPrc;
	mWindowsClassEx.cbClsExtra    = 0;
	mWindowsClassEx.cbWndExtra    = 0;
	mWindowsClassEx.hInstance     = hInst;
	mWindowsClassEx.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
	mWindowsClassEx.hCursor       = LoadCursor( NULL, IDC_ARROW );
	mWindowsClassEx.hbrBackground = static_cast<HBRUSH>( GetStockObject( WHITE_BRUSH ) );
	mWindowsClassEx.lpszMenuName  = menuName;
	mWindowsClassEx.lpszClassName = className;
	mWindowsClassEx.hIconSm       = LoadIcon( NULL, IDI_APPLICATION );
}

//===========================================================================

WindowsApplication::~WindowsApplication()
{

}

//===========================================================================

void WindowsApplication::Register()
{
	RegisterClassEx( &mWindowsClassEx );
}

#endif