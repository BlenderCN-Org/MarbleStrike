///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __particleeditorgui__
#define __particleeditorgui__

class OGLCanvas;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/slider.h>
#include <wx/clrpicker.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/button.h>
#include <wx/statbox.h>
#include <wx/filepicker.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define MENU_RENDERER_OPENGL 1000
#define MENU_RENDERER_D3D 1001
#define EDITOR_MENU_ANIMATION_PLAY 1002
#define EDITOR_MENU_ANIMATION_STOP 1003
#define TEXT_NAME 1004
#define TEXT_MAX_PARTICLES 1005
#define SLIDER_MAX_PARTICLES 1006
#define TEXT_EMIT_TIME 1007
#define TEXT_EMISSION_MIN 1008
#define TEXT_EMISSION_MAX 1009
#define TEXT_SIZE_MIN 1010
#define TEXT_SIZE_MAX 1011
#define TEXT_SHRINK_MIN 1012
#define TEXT_SHRINK_MAX 1013
#define TEXT_VELOCITY_X_MIN 1014
#define TEXT_VELOCITY_X_MAX 1015
#define TEXT_VELOCITY_Y_MIN 1016
#define TEXT_VELOCITY_Y_MAX 1017
#define TEXT_VELOCITY_Z_MIN 1018
#define TEXT_VELOCITY_Z_MAX 1019
#define TEXT_LIFE_MIN 1020
#define TEXT_LIFE_MAX 1021
#define TEXT_DECAY_MIN 1022
#define TEXT_DECAY_MAX 1023
#define COLOR_PICKER_MIN 1024
#define COLOR_PICKER_MAX 1025
#define SLIDER_ALPHA_MIN 1026
#define SLIDER_ALPHA_MAX 1027
#define LIST_BOX_FORCES 1028
#define BUTTON_ADD_FORCE 1029
#define BUTTON_DELETE_FORCE 1030
#define TEXT_FORCE_NAME 1031
#define TEXT_FORCE_X 1032
#define TEXT_FORCE_Y 1033
#define TEXT_FORCE_Z 1034
#define FILE_PICK_TEXTURE 1035
#define BUTTON_CLEAR_TEXTURE 1036
#define BUTTON_CREATE_UV 1037
#define BUTTON_ID_ANIMATION_PLAY 1038
#define BUTTON_ID_ANIMATION_STOP 1039

///////////////////////////////////////////////////////////////////////////////
/// Class ParticleEditorGui
///////////////////////////////////////////////////////////////////////////////
class ParticleEditorGui : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mMenuBar;
		wxMenu* mMenuFile;
		wxMenu* mMenuRenderer;
		wxMenu* mMenuAnimation;
		wxScrolledWindow* mParticleProperties;
		wxStaticText* m_staticText38;
		wxTextCtrl* mTextName;
		wxStaticText* m_staticText41;
		wxStaticText* m_staticText381;
		wxTextCtrl* mTextMaxParticles;
		wxSlider* mSliderMaxParticles;
		wxStaticText* m_staticText51;
		wxTextCtrl* mTextEmitTime;
		wxStaticText* m_staticText52;
		wxStaticText* m_staticText39;
		wxTextCtrl* mTextEmissionMin;
		wxTextCtrl* mTextEmissionMax;
		wxStaticText* m_staticText43;
		wxTextCtrl* mTextSizeMin;
		wxTextCtrl* mTextSizeMax;
		wxStaticText* m_staticText44;
		wxTextCtrl* mTextShrinkMin;
		wxTextCtrl* mTextShrinkMax;
		wxStaticText* m_staticText45;
		wxTextCtrl* mTextVelocityXMin;
		wxTextCtrl* mTextVelocityXMax;
		wxStaticText* m_staticText46;
		wxTextCtrl* mTextVelocityYMin;
		wxTextCtrl* mTextVelocityYMax;
		wxStaticText* m_staticText47;
		wxTextCtrl* mTextVelocityZMin;
		wxTextCtrl* mTextVelocityZMax;
		wxStaticText* m_staticText48;
		wxTextCtrl* mTextLifeMin;
		wxTextCtrl* mTextLifeMax;
		wxStaticText* m_staticText49;
		wxTextCtrl* mTextDecayMin;
		wxTextCtrl* mTextDecayMax;
		wxStaticText* m_staticText35;
		wxColourPickerCtrl* mColorPickerButtonMin;
		wxColourPickerCtrl* mColorPickerButtonMax;
		wxStaticText* m_staticText211;
		wxSlider* mSliderAlphaMin;
		wxSlider* mSliderAlphaMax;
		wxListBox* mListBoxForces;
		wxButton* mButtonAddForce;
		wxButton* mButtonDeleteForce;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText20;
		wxStaticText* m_staticText21;
		wxTextCtrl* mTextForceName;
		wxTextCtrl* mTextForceX;
		wxTextCtrl* mTextForceY;
		wxTextCtrl* mTextForceZ;
		wxStaticText* m_staticText36;
		wxFilePickerCtrl* mFilePickTexture;
		wxButton* mClearTexture;
		wxButton* mCreateUV;
		wxScrolledWindow* mClearPanel;
		wxPanel* m_panel5;
		wxButton* mButtonPlay;
		wxButton* mButtonStop;
		OGLCanvas* mGraphicsPanel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuSelectionNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionRenderer( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKillFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSliderScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnListBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnButtonClickAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKeyDownGraphicsPanel( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnKeyUpGraphicsPanel( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnLeftDownGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeftUpGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMotionGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnSizeGraphicsPanel( wxSizeEvent& event ) { event.Skip(); }
		
	
	public:
		
		ParticleEditorGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Particle Editor"), const wxPoint& pos = wxPoint( 150,30 ), const wxSize& size = wxSize( 900,900 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~ParticleEditorGui();
	
};

#endif //__particleeditorgui__
