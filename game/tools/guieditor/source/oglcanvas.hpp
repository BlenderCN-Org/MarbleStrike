#ifndef OGLCANVAS_HPP
#define OGLCANVAS_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <wx/wx.h>
#include "wx/glcanvas.h"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLCanvas: public wxGLCanvas
{
public:
    OGLCanvas(
		wxWindow *parent, 
		wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, 
		long style = 0,
        const wxString& name = wxT("TestGLCanvas")
		);

    ~OGLCanvas();

protected:
    void OnPaint( wxPaintEvent &event );
    void OnSize( wxSizeEvent &event );
    void OnEraseBackground( wxEraseEvent &event );
    //void OnMouse(wxMouseEvent &event );
	//void OnMouseButton( wxMouseEvent &event );
	//void OnKeyboard( wxKeyEvent &event );
	void OnIdle( wxIdleEvent &event );

private:
    
};

#endif
