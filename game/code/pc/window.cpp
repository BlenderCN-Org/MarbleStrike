#if defined PLAT_PC

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "window.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Window::Window()
{
    //
    // If we declare a window class with a default constructor,
    // we need to reset the window to a nothing
    //
    mHwnd = NULL;
}

//===========================================================================

HWND Window::Create(
                    HINSTANCE hinst,
                    LPCTSTR className,
                    LPCTSTR windowName,
                    HWND parent,
                    DWORD dStyle,
                    DWORD dXStyle,
                    int x,
                    int y,
                    int width,
                    int height
                    )
{
    //
    // When call the Create() method, we can use it to create a NEW_PTR window
    //
    mHwnd = CreateWindowEx(
        dXStyle, 
        className, 
        windowName, 
        dStyle, 
        x, 
        y, 
        width,
        height, 
        parent, 
        NULL, 
        hinst, 
        NULL
        );

    //
    // We hope everything went alright and the window was created
    //
    if ( mHwnd != NULL )
    {
        return mHwnd;
    }

    //
    // If something went wrong, for example if the window could not
    // be created, return a "nothing" window
    //
    return NULL;
}

//===========================================================================

bool Window::Show( int dCmdShow )
{
    //
    // We will display the main window as a regular object and update it
    //
    if ( ShowWindow( mHwnd, dCmdShow ) && UpdateWindow( mHwnd ) )
    {
        return true;
    }

    return false;
}

//===========================================================================

Window::operator HWND()
{
    //
    // This overloaded operator allows us to use HWND anyway we want
    //
    return mHwnd;
}

//===========================================================================

void Window::SetSize( int width, int height )
{
    SetWindowPos(
        mHwnd,
        0,
        0,
        0,
        width,
        height,
        SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED
        );
}

#endif