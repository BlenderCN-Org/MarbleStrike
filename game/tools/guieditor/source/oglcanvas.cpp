//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "oglcanvas.hpp"
#include <GL/glu.h>
#include "common/engine/game/gameapp.hpp"
#include "common/engine/input/inputmanager.hpp"
#include "pc/input/pcinputmanager.hpp"
#include "common/engine/database/database.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLCanvas::OGLCanvas(
					 wxWindow *parent, 
					 wxWindowID id,
					 const wxPoint& pos, 
					 const wxSize& size, 
					 long style, 
					 const wxString& name )
					 :	wxGLCanvas( parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name )
{
	Connect( wxEVT_SIZE, wxSizeEventHandler( OGLCanvas::OnSize ) );
	Connect( wxEVT_PAINT, wxPaintEventHandler( OGLCanvas::OnPaint ) );
	Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( OGLCanvas::OnEraseBackground ) );
	//Connect( wxEVT_MOTION, wxMouseEventHandler( OGLCanvas::OnMouse ) );
	
	//Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( OGLCanvas::OnMouseButton ) );
	//Connect( wxEVT_LEFT_UP, wxMouseEventHandler( OGLCanvas::OnMouseButton ) );
	//Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( OGLCanvas::OnMouseButton ) );
	//Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( OGLCanvas::OnMouseButton ) );
	//Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( OGLCanvas::OnMouseButton ) );
	//Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( OGLCanvas::OnMouseButton ) );

	//Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( OGLCanvas::OnKeyboard ) );
	//Connect( wxEVT_KEY_UP, wxKeyEventHandler( OGLCanvas::OnKeyboard ) );
	//Connect( wxEVT_CHAR, wxKeyEventHandler( OGLCanvas::OnKeyboard ) );

	Connect( wxEVT_IDLE, wxIdleEventHandler( OGLCanvas::OnIdle ) );
}

//===========================================================================

OGLCanvas::~OGLCanvas()
{
}

//===========================================================================

void OGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    // must always be here
    wxPaintDC dc(this);

	GameApp::Get()->Update();	
}

//===========================================================================

void OGLCanvas::OnSize(wxSizeEvent& event)
{
	UNUSED_ALWAYS( event );

	//Database::Get()->SetScreenWidth( event.m_size.x );
	//Database::Get()->SetScreenHeight( event.m_size.y );

	//GameApp::Get()->GetGameFlow()->OnPreResize();
	//Renderer::Get()->OnResize();
	//GameApp::Get()->GetGameFlow()->OnPostResize();
	
	//wxGLCanvas::OnSize(event);
}

//===========================================================================

void OGLCanvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}

//===========================================================================

//void OGLCanvas::OnMouse(wxMouseEvent& event)
//{
//	GameApp* app = GameApp::Get();
//	if ( app != NULL )
//	{
//		app->GetInputManager()->GetInputManagerPlat()->MouseMove( event.m_x, event.m_y );
//	}
//}

//===========================================================================

//void OGLCanvas::OnMouseButton( wxMouseEvent &event )
//{
//	SetFocus();
//
//	GameApp* app = GameApp::Get();
//
//	MouseButtonType button = MOUSE_NONE;
//
//	switch ( event.GetButton() )
//	{
//	case wxMOUSE_BTN_LEFT:
//		button = MOUSE_LEFT_BUTTON;
//		break;
//	case wxMOUSE_BTN_RIGHT:
//		button = MOUSE_RIGHT_BUTTON;
//		break;
//	case wxMOUSE_BTN_MIDDLE:
//		button = MOUSE_MIDDLE_BUTTON;
//		break;
//	}
//
//	if ( event.ButtonDown() )
//	{
//		if ( app != NULL )
//		{
//			app->GetInputManager()->GetInputManagerPlat()->MouseDown( button, event.m_x, event.m_y );
//		}
//	}
//
//	if ( event.ButtonUp() )
//	{
//		if ( app != NULL )
//		{
//			app->GetInputManager()->GetInputManagerPlat()->MouseUp( button );
//		}	
//	}
//}

//===========================================================================

//void OGLCanvas::OnKeyboard( wxKeyEvent &event )
//{
//	GameApp* app = GameApp::Get();
//
//	if ( app != NULL )
//	{
//		if ( event.GetEventType() == wxEVT_KEY_DOWN )
//		{
//			app->GetInputManager()->KeyboardDown( event.GetKeyCode() );	
//			event.Skip();
//		}
//		else if ( event.GetEventType() == wxEVT_KEY_UP )
//		{
//			app->GetInputManager()->KeyboardUp( event.GetKeyCode() );		
//		}
//		else if ( event.GetEventType() == wxEVT_CHAR )
//		{
//			app->GetInputManager()->KeyboardChar( event.GetKeyCode() );
//		}
//	}
//}

//===========================================================================

void OGLCanvas::OnIdle( wxIdleEvent &event )
{
	UNUSED_ALWAYS( event );

	Refresh( false );
}