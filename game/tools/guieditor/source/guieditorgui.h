///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __guieditorgui__
#define __guieditorgui__

class OGLCanvas;
class TimeLineWidget;

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/radiobox.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/clrpicker.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/treectrl.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/slider.h>
#include <wx/statbox.h>
#include <wx/filepicker.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define EDIT_UNDO 1000
#define EDIT_COPY 1001
#define EDIT_CUT 1002
#define EDIT_PASTE 1003
#define EDIT_COPY_SIZE_DATA 1004
#define EDIT_COPY_POSITION_DATA 1005
#define EDIT_PASTE_SIZE_DATA 1006
#define EDIT_PASTE_POSITION_DATA 1007
#define EDIT_DELETE 1008
#define EDIT_SHOW_HIDE 1009
#define MENU_RENDERER_OPENGL 1010
#define MENU_RENDERER_D3D 1011
#define SCREEN_SIZE_PORTRAIT_320_480 1012
#define SCREEN_SIZE_LANDSCAPE_480_320 1013
#define SCREEN_SIZE_PORTRAIT_640_960 1014
#define SCREEN_SIZE_LANDSCAPE_960_640 1015
#define SCREEN_SIZE_PORTRAIT_640_1136 1016
#define SCREEN_SIZE_LANDSCAPE_1136_640 1017
#define SCREEN_SIZE_PORTRAIT_720_1280 1018
#define SCREEN_SIZE_LANDSCAPE_1280_720 1019
#define SCREEN_SIZE_PORTRAIT_768_1024 1020
#define SCREEN_SIZE_LANDSCAPE_1024_768 1021
#define SCREEN_SIZE_PORTRAIT_1536_2048 1022
#define SCREEN_SIZE_LANDSCAPE_2048_1536 1023
#define EDITOR_MENU_INSERT_STATIC 1024
#define EDITOR_MENU_INSERT_BUTTON 1025
#define EDITOR_MENU_INSERT_SLIDER 1026
#define EDITOR_MENU_INSERT_SPRITE 1027
#define EDITOR_MENU_MOVE_BACK 1028
#define EDITOR_MENU_MOVE_FRONT 1029
#define EDITOR_MENU_ANIMATION_PLAY 1030
#define EDITOR_MENU_ANIMATION_STOP 1031
#define EDITOR_MENU_ANIMATION_ADD_KEYFRAME 1032
#define EDITOR_MENU_ANIMATION_DELETE_KEYFRAME 1033
#define LAYOUT_VERTICAL_CENTRE 1034
#define LAYOUT_HORIZONTAL_CENTRE 1035
#define COLOR_PICK_BOUNDING_BOX 1036
#define COLOR_PICK_BOUNDING_BOX_SELECT 1037
#define TEXT_BUTTON_NAME 1038
#define TEXT_BUTTON_WIDTH 1039
#define TEXT_BUTTON_HEIGHT 1040
#define TEXT_BUTTON_X 1041
#define TEXT_BUTTON_Y 1042
#define TEXT_BUTTON_TEXT 1043
#define COLOR_PICKER_BUTTON 1044
#define SLIDER_BUTTON_ALPHA 1045
#define FILE_PICK_BUTTON_TEXTURE 1046
#define BUTTON_BUTTON_CLEAR_TEXTURE 1047
#define BUTTON_BUTTON_CREATE_UV 1048
#define FILE_PICK_BUTTON_ONPRESS_TEXTURE 1049
#define BUTTON_BUTTON_ONPRESS_CLEAR_TEXTURE 1050
#define BUTTON_BUTTON_ONPRESS_CREATE_UV 1051
#define FILE_PICK_BUTTON_FONT 1052
#define COLOR_PICKER_BUTTON_FONT 1053
#define TEXT_BUTTON_FONT_SIZE 1054
#define CHOICE_BUTTON_FONT_HORIZONTAL 1055
#define CHOICE_BUTTON_FONT_VERTICAL 1056
#define TEXT_SLIDER_NAME 1057
#define TEXT_SLIDER_WIDTH 1058
#define TEXT_SLIDER_HEIGHT 1059
#define TEXT_SLIDER_X 1060
#define TEXT_SLIDER_Y 1061
#define TEXT_SLIDER_MIN 1062
#define TEXT_SLIDER_MAX 1063
#define TEXT_SLIDER_DEFAULT 1064
#define FILE_PICK_SLIDER_TEXTURE_BAR 1065
#define BUTTON_SLIDER_CLEAR_TEXTURE_BAR 1066
#define BUTTON_SLIDER_CREATE_UV_BAR 1067
#define FILE_PICK_SLIDER_TEXTURE_BUTTON 1068
#define BUTTON_SLIDER_CLEAR_TEXTURE_BUTTON 1069
#define BUTTON_SLIDER_CREATE_UV_BUTTON 1070
#define TEXT_SPRITE_NAME 1071
#define TEXT_SPRITE_WIDTH 1072
#define TEXT_SPRITE_HEIGHT 1073
#define TEXT_SPRITE_X 1074
#define TEXT_SPRITE_Y 1075
#define COLOR_PICKER_SPRITE 1076
#define SLIDER_SPRITE_ALPHA 1077
#define FILE_PICK_SPRITE_TEXTURE_FILE 1078
#define BUTTON_SPRITE_CLEAR_TEXTURE 1079
#define TEXT_SPRITE_FPS 1080
#define TEXT_STATIC_NAME 1081
#define TEXT_STATIC_WIDTH 1082
#define TEXT_STATIC_HEIGHT 1083
#define TEXT_STATIC_X 1084
#define TEXT_STATIC_Y 1085
#define COLOR_PICKER_STATIC 1086
#define SLIDER_STATIC_ALPHA 1087
#define TEXT_STATIC_TEXT 1088
#define FILE_PICK_STATIC_TEXTURE 1089
#define BUTTON_STATIC_CLEAR_TEXTURE 1090
#define BUTTON_STATIC_CREATE_UV 1091
#define FILE_PICK_STATIC_FONT 1092
#define COLOR_PICKER_STATIC_FONT 1093
#define SLIDER_STATIC_ALPHA_FONT 1094
#define TEXT_STATIC_FONT_SIZE 1095
#define CHOICE_STATIC_FONT_HORIZONTAL 1096
#define CHOICE_STATIC_FONT_VERTICAL 1097
#define BUTTON_ID_ANIMATION_BACK_KEYFRAME 1098
#define BUTTON_ID_ANIMATION_FORWARD_KEYFRAME 1099
#define BUTTON_ID_ANIMATION_ADD_KEYFRAME 1100
#define BUTTON_ID_ANIMATION_DELETE_KEYFRAME 1101
#define BUTTON_ID_ANIMATION_PLAY 1102
#define BUTTON_ID_ANIMATION_STOP 1103
#define BUTTON_ID_ZOOM_IN 1104
#define BUTTON_ID_ZOOM_OUT 1105
#define BUTTON_ADD_ANIMATION_SEQUENCE 1106
#define BUTTON_DELETE_ANIMATION_SEQUENCE 1107
#define TEXT_ANIMATION_SEQUENCE_NAME 1108
#define CHOICE_ANIMATION_SELECTION 1109

///////////////////////////////////////////////////////////////////////////////
/// Class GuiEditorGui
///////////////////////////////////////////////////////////////////////////////
class GuiEditorGui : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* mMenuBar;
		wxMenu* mMenuFile;
		wxMenu* mMenuEdit;
		wxMenu* mMenuRenderer;
		wxMenu* mMenuEditor;
		wxMenu* mMenuScreenSize;
		wxMenu* mMenuAnimation;
		wxPanel* m_panel7;
		wxRadioBox* mRadioBoxTransform;
		wxStaticLine* m_staticline1;
		wxButton* m_button10;
		wxButton* m_button111;
		wxStaticLine* m_staticline2;
		wxStaticText* m_staticText27;
		wxColourPickerCtrl* mColorPickerBoundingBox;
		wxStaticLine* m_staticline3;
		wxStaticText* m_staticText28;
		wxColourPickerCtrl* mColorPickerBoundingBoxSelect;
		wxStaticText* m_staticText281;
		wxColourPickerCtrl* mColorPickerBackground;
		wxNotebook* mNotebook;
		wxPanel* m_panel2;
		wxTreeCtrl* mSceneGraphTree;
		wxScrolledWindow* mButtonProperties;
		wxStaticText* m_staticText38;
		wxTextCtrl* mTextButtonName;
		wxStaticText* m_staticText391;
		wxTextCtrl* mTextButtonWidth;
		wxStaticText* m_staticText3911;
		wxTextCtrl* mTextButtonHeight;
		wxStaticText* m_staticText411;
		wxTextCtrl* mTextButtonX;
		wxStaticText* m_staticText4111;
		wxTextCtrl* mTextButtonY;
		wxStaticText* m_staticText341;
		wxTextCtrl* mTextButtonText;
		wxStaticText* m_staticText35;
		wxColourPickerCtrl* mColorPickerButton;
		wxStaticText* m_staticText57;
		wxSlider* mSliderButtonAlpha;
		wxStaticText* m_staticText36;
		wxFilePickerCtrl* mFilePickButtonTexture;
		wxButton* mButtonButtonClearTexture;
		wxButton* mButtonButtonCreateUV;
		wxStaticText* mOnPressTextureLabel;
		wxFilePickerCtrl* mFilePickButtonOnPressTexture;
		wxButton* mButtonButtonClearOnPressTexture;
		wxButton* mButtonButtonOnPressCreateUV;
		wxStaticText* m_staticText37;
		wxFilePickerCtrl* mFilePickButtonFont;
		wxStaticText* m_staticText342;
		wxColourPickerCtrl* mColorPickerButtonFont;
		wxStaticText* m_staticText3521;
		wxTextCtrl* mTextButtonFontSize;
		wxChoice* mChoiceButtonFontHorizontal;
		wxChoice* mChoiceButtonFontVertical;
		wxScrolledWindow* mClearPanel;
		wxScrolledWindow* mSliderProperties;
		wxStaticText* m_staticText381;
		wxTextCtrl* mTextSliderName;
		wxStaticText* m_staticText3913;
		wxTextCtrl* mTextSliderWidth;
		wxStaticText* m_staticText39112;
		wxTextCtrl* mTextSliderHeight;
		wxStaticText* m_staticText4113;
		wxTextCtrl* mTextSliderX;
		wxStaticText* m_staticText41112;
		wxTextCtrl* mTextSliderY;
		wxStaticText* m_staticText81;
		wxTextCtrl* mTextSliderMin;
		wxStaticText* m_staticText82;
		wxTextCtrl* mTextSliderMax;
		wxStaticText* m_staticText83;
		wxTextCtrl* mTextSliderDefault;
		wxStaticText* m_staticText362;
		wxFilePickerCtrl* mFilePickSliderTextureBar;
		wxButton* mButtonSliderClearTextureBar;
		wxButton* mButtonSliderCreateUVBar;
		wxStaticText* m_staticText3621;
		wxFilePickerCtrl* mFilePickSliderTextureButton;
		wxButton* mButtonSliderClearTextureButton;
		wxButton* mButtonSliderCreateUVButton;
		wxScrolledWindow* mSpriteProperties;
		wxStaticText* m_staticText3831;
		wxTextCtrl* mTextSpriteName;
		wxStaticText* m_staticText39121;
		wxTextCtrl* mTextSpriteWidth;
		wxStaticText* m_staticText391111;
		wxTextCtrl* mTextSpriteHeight;
		wxStaticText* m_staticText41121;
		wxTextCtrl* mTextSpriteX;
		wxStaticText* m_staticText411111;
		wxTextCtrl* mTextSpriteY;
		wxStaticText* m_staticText3511;
		wxColourPickerCtrl* mColorPickerSprite;
		wxStaticText* m_staticText5712;
		wxSlider* mSliderSpriteAlpha;
		wxStaticText* m_staticText3611;
		wxFilePickerCtrl* mFilePickSpriteTextureFile;
		wxButton* mButtonSpriteClearTexture;
		wxStaticText* m_staticText38311;
		wxTextCtrl* mTextSpriteFPS;
		wxScrolledWindow* mStaticProperties;
		wxStaticText* m_staticText383;
		wxTextCtrl* mTextStaticName;
		wxStaticText* m_staticText3912;
		wxTextCtrl* mTextStaticWidth;
		wxStaticText* m_staticText39111;
		wxTextCtrl* mTextStaticHeight;
		wxStaticText* m_staticText4112;
		wxTextCtrl* mTextStaticX;
		wxStaticText* m_staticText41111;
		wxTextCtrl* mTextStaticY;
		wxStaticText* m_staticText351;
		wxColourPickerCtrl* mColorPickerStatic;
		wxStaticText* m_staticText571;
		wxSlider* mSliderStaticAlpha;
		wxStaticText* m_staticText3411;
		wxTextCtrl* mTextStaticText;
		wxStaticText* m_staticText361;
		wxFilePickerCtrl* mFilePickStaticTexture;
		wxButton* mButtonStaticClearTexture;
		wxButton* mButtonStaticCreateUV;
		wxStaticText* m_staticText371;
		wxFilePickerCtrl* mFilePickStaticFont;
		wxStaticText* m_staticText34;
		wxColourPickerCtrl* mColorPickerStaticFont;
		wxStaticText* m_staticText5711;
		wxSlider* mSliderStaticAlphaFont;
		wxStaticText* m_staticText352;
		wxTextCtrl* mTextStaticFontSize;
		wxChoice* mChoiceStaticFontHorizontal;
		wxChoice* mChoiceStaticFontVertical;
		wxPanel* mAnimationControlsPanel;
		wxStaticText* m_staticText191;
		wxTextCtrl* mTextAnimationFrame;
		wxButton* m_button9;
		wxButton* m_button101;
		wxButton* m_button5;
		wxButton* m_button6;
		wxButton* m_button7;
		wxButton* m_button8;
		wxButton* mZoomIn;
		wxButton* mZoomOut;
		wxPanel* m_panel6;
		wxButton* mAddAnimationSequenceButton;
		wxButton* mDeleteAnimationSequenceButton;
		wxTextCtrl* mTextAnimationSequenceName;
		wxStaticText* mAnimationSelectionLabel;
		wxChoice* mAnimationsChoice;
		TimeLineWidget* mTimeLineControl;
		OGLCanvas* mGraphicsPanel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuSelectionNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionSave( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionRenderer( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionPortrait( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionLandscape( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionEditor( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSelectionAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioBoxTransform( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickLayout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnColourChangedBoundingBox( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnColourChangedBackground( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnKeyDownSceneGraph( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnLeftDownTree( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnRightDownTree( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnTreeItemMenu( wxTreeEvent& event ) { event.Skip(); }
		virtual void OnKillFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnTextEnter( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnColorChanged( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void OnScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnScrollChanged( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		virtual void OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCharAnimationFrame( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnTextEnterAnimationFrame( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickAnimation( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonClickZoom( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnKeyDownGraphicsPanel( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnKeyUpGraphicsPanel( wxKeyEvent& event ) { event.Skip(); }
		virtual void OnLeftDownGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLeftUpGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnMotionGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnRightDownGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnRightUpGraphicsPanel( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnSizeGraphicsPanel( wxSizeEvent& event ) { event.Skip(); }
		
	
	public:
		
		GuiEditorGui( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Gui Editor"), const wxPoint& pos = wxPoint( 0,0 ), const wxSize& size = wxSize( 1200,1000 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~GuiEditorGui();
	
};

#endif //__guieditorgui__
