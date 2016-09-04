///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __textureuvdialoggui__
#define __textureuvdialoggui__

#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define UV_PROPERTIES_WIDTH 1000
#define UV_PROPERTIES_HEIGHT 1001
#define UV_PROPERTIES_POSITION_X 1002
#define UV_PROPERTIES_POSITION_Y 1003

///////////////////////////////////////////////////////////////////////////////
/// Class TextureUVDialogGui
///////////////////////////////////////////////////////////////////////////////
class TextureUVDialogGui : public wxDialog 
{
	private:
	
	protected:
		wxButton* m_button4;
		wxButton* m_button5;
		wxPanel* mImagePanel;
		wxStaticText* m_staticText9;
		wxTextCtrl* mTextUVWidth;
		wxTextCtrl* mTextUVHeight;
		wxStaticText* m_staticText11;
		wxTextCtrl* mTextUVX;
		wxTextCtrl* mTextUVY;
		wxButton* m_button1;
		wxButton* m_button2;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnInitDialog( wxInitDialogEvent& event ){ event.Skip(); }
		virtual void OnButtonClickMagnified( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonClickMinified( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEraseBackground( wxEraseEvent& event ){ event.Skip(); }
		virtual void OnKeyDownImagePanel( wxKeyEvent& event ){ event.Skip(); }
		virtual void OnKeyUpImagePanel( wxKeyEvent& event ){ event.Skip(); }
		virtual void OnLeftDownImagePanel( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnLeftUpImagePanel( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnMotionImagePanel( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnPaint( wxPaintEvent& event ){ event.Skip(); }
		virtual void OnTextEnter( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonClickOK( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonClickCancel( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		TextureUVDialogGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxPoint( 30,30 ), const wxSize& size = wxSize( 554,520 ), long style = wxDEFAULT_DIALOG_STYLE );
		~TextureUVDialogGui();
	
};

#endif //__textureuvdialoggui__
