///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "OGLCanvas.hpp"
#include "timelinewidget.hpp"

#include "guieditorgui.h"

///////////////////////////////////////////////////////////////////////////

GuiEditorGui::GuiEditorGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1200,800 ), wxSize( -1,-1 ) );
	
	mMenuBar = new wxMenuBar( 0 );
	mMenuFile = new wxMenu();
	wxMenuItem* mNewMenuItem;
	mNewMenuItem = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("New") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mNewMenuItem );
	
	wxMenuItem* mOpenMenuItem;
	mOpenMenuItem = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mOpenMenuItem );
	
	wxMenuItem* m_separator1;
	m_separator1 = mMenuFile->AppendSeparator();
	
	wxMenuItem* mSaveAsMenuItem;
	mSaveAsMenuItem = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("Save As") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mSaveAsMenuItem );
	
	wxMenuItem* mSaveMenuItem;
	mSaveMenuItem = new wxMenuItem( mMenuFile, wxID_ANY, wxString( wxT("Save") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuFile->Append( mSaveMenuItem );
	
	mMenuBar->Append( mMenuFile, wxT("File") ); 
	
	mMenuEdit = new wxMenu();
	wxMenuItem* m_menuItem23;
	m_menuItem23 = new wxMenuItem( mMenuEdit, EDIT_UNDO, wxString( wxT("Undo") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( m_menuItem23 );
	
	wxMenuItem* m_separator2;
	m_separator2 = mMenuEdit->AppendSeparator();
	
	wxMenuItem* mMenuItemEditCopy;
	mMenuItemEditCopy = new wxMenuItem( mMenuEdit, EDIT_COPY, wxString( wxT("Copy") ) + wxT('\t') + wxT("CTRL+c"), wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemEditCopy );
	
	wxMenuItem* mMenuItemEditCut;
	mMenuItemEditCut = new wxMenuItem( mMenuEdit, EDIT_CUT, wxString( wxT("Cut") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemEditCut );
	
	wxMenuItem* mMenuItemEditPaste;
	mMenuItemEditPaste = new wxMenuItem( mMenuEdit, EDIT_PASTE, wxString( wxT("Paste") ) + wxT('\t') + wxT("CTRL+v"), wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemEditPaste );
	
	wxMenuItem* m_separator7;
	m_separator7 = mMenuEdit->AppendSeparator();
	
	wxMenuItem* mMenuitemEditCopySize;
	mMenuitemEditCopySize = new wxMenuItem( mMenuEdit, EDIT_COPY_SIZE_DATA, wxString( wxT("Copy Size") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuitemEditCopySize );
	
	wxMenuItem* mMenuitemEditCopyPosition;
	mMenuitemEditCopyPosition = new wxMenuItem( mMenuEdit, EDIT_COPY_POSITION_DATA, wxString( wxT("Copy Position") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuitemEditCopyPosition );
	
	wxMenuItem* mMenuitemEditPasteSize;
	mMenuitemEditPasteSize = new wxMenuItem( mMenuEdit, EDIT_PASTE_SIZE_DATA, wxString( wxT("Paste Size") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuitemEditPasteSize );
	
	wxMenuItem* mMenuitemEditPastePosition;
	mMenuitemEditPastePosition = new wxMenuItem( mMenuEdit, EDIT_PASTE_POSITION_DATA, wxString( wxT("Paste Position") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuitemEditPastePosition );
	
	wxMenuItem* m_separator3;
	m_separator3 = mMenuEdit->AppendSeparator();
	
	wxMenuItem* mMenuItemEditDelete;
	mMenuItemEditDelete = new wxMenuItem( mMenuEdit, EDIT_DELETE, wxString( wxT("Delete") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemEditDelete );
	
	wxMenuItem* m_separator4;
	m_separator4 = mMenuEdit->AppendSeparator();
	
	wxMenuItem* mMenuItemEditShowHide;
	mMenuItemEditShowHide = new wxMenuItem( mMenuEdit, EDIT_SHOW_HIDE, wxString( wxT("Show/Hide") ) + wxT('\t') + wxT("CTRL+h"), wxEmptyString, wxITEM_NORMAL );
	mMenuEdit->Append( mMenuItemEditShowHide );
	
	mMenuBar->Append( mMenuEdit, wxT("Edit") ); 
	
	mMenuRenderer = new wxMenu();
	wxMenuItem* mMenuItemOpenGL;
	mMenuItemOpenGL = new wxMenuItem( mMenuRenderer, MENU_RENDERER_OPENGL, wxString( wxT("OpenGL") ) , wxEmptyString, wxITEM_RADIO );
	mMenuRenderer->Append( mMenuItemOpenGL );
	
	wxMenuItem* mMenuItemD3D;
	mMenuItemD3D = new wxMenuItem( mMenuRenderer, MENU_RENDERER_D3D, wxString( wxT("Direct3D") ) , wxEmptyString, wxITEM_RADIO );
	mMenuRenderer->Append( mMenuItemD3D );
	
	mMenuBar->Append( mMenuRenderer, wxT("Renderer") ); 
	
	mMenuEditor = new wxMenu();
	mMenuScreenSize = new wxMenu();
	wxMenuItem* mMenuItemPortrait320x480;
	mMenuItemPortrait320x480 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_PORTRAIT_320_480, wxString( wxT("Portrait (320x480)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemPortrait320x480 );
	mMenuItemPortrait320x480->Check( true );
	
	wxMenuItem* mMenuItemLandscape480x320;
	mMenuItemLandscape480x320 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_LANDSCAPE_480_320, wxString( wxT("Landscape (480x320)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemLandscape480x320 );
	
	wxMenuItem* mMenuItemPortrait640x960;
	mMenuItemPortrait640x960 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_PORTRAIT_640_960, wxString( wxT("Portrait (640x960)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemPortrait640x960 );
	mMenuItemPortrait640x960->Check( true );
	
	wxMenuItem* mMenuItemLandscape960x640;
	mMenuItemLandscape960x640 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_LANDSCAPE_960_640, wxString( wxT("Landscape (9640x640)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemLandscape960x640 );
	
	wxMenuItem* mMenuItemPortrait640x1136;
	mMenuItemPortrait640x1136 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_PORTRAIT_640_1136, wxString( wxT("Portrait (640x1136)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemPortrait640x1136 );
	mMenuItemPortrait640x1136->Check( true );
	
	wxMenuItem* mMenuItemLandscape1136x640;
	mMenuItemLandscape1136x640 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_LANDSCAPE_1136_640, wxString( wxT("Landscape (1136x640)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemLandscape1136x640 );
	
	wxMenuItem* mMenuItemPortrait720x1280;
	mMenuItemPortrait720x1280 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_PORTRAIT_720_1280, wxString( wxT("Portrait (720x1280)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemPortrait720x1280 );
	mMenuItemPortrait720x1280->Check( true );
	
	wxMenuItem* mMenuItemLandscape1280x720;
	mMenuItemLandscape1280x720 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_LANDSCAPE_1280_720, wxString( wxT("Landscape (1280x720)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemLandscape1280x720 );
	
	wxMenuItem* mMenuItemPortrait768x1024;
	mMenuItemPortrait768x1024 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_PORTRAIT_768_1024, wxString( wxT("Portrait (768x1024)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemPortrait768x1024 );
	mMenuItemPortrait768x1024->Check( true );
	
	wxMenuItem* mMenuItemLandscape1024x768;
	mMenuItemLandscape1024x768 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_LANDSCAPE_1024_768, wxString( wxT("Landscape (1024x768)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemLandscape1024x768 );
	
	wxMenuItem* mMenuItemPortrait1536x2048;
	mMenuItemPortrait1536x2048 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_PORTRAIT_1536_2048, wxString( wxT("Portrait (1536x2048)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemPortrait1536x2048 );
	mMenuItemPortrait1536x2048->Check( true );
	
	wxMenuItem* mMenuItemLandscape2048x1536;
	mMenuItemLandscape2048x1536 = new wxMenuItem( mMenuScreenSize, SCREEN_SIZE_LANDSCAPE_2048_1536, wxString( wxT("Landscape (2048x1536)") ) , wxEmptyString, wxITEM_RADIO );
	mMenuScreenSize->Append( mMenuItemLandscape2048x1536 );
	
	mMenuEditor->Append( -1, wxT("Screen Size"), mMenuScreenSize );
	
	wxMenuItem* m_separator5;
	m_separator5 = mMenuEditor->AppendSeparator();
	
	wxMenuItem* mMenuInsertStatic;
	mMenuInsertStatic = new wxMenuItem( mMenuEditor, EDITOR_MENU_INSERT_STATIC, wxString( wxT("Insert Static") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEditor->Append( mMenuInsertStatic );
	
	wxMenuItem* mMenuInsertButton;
	mMenuInsertButton = new wxMenuItem( mMenuEditor, EDITOR_MENU_INSERT_BUTTON, wxString( wxT("Insert Button") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEditor->Append( mMenuInsertButton );
	
	wxMenuItem* mMenuInsertSlider;
	mMenuInsertSlider = new wxMenuItem( mMenuEditor, EDITOR_MENU_INSERT_SLIDER, wxString( wxT("Insert Slider") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEditor->Append( mMenuInsertSlider );
	
	wxMenuItem* mMenuInsertSprite;
	mMenuInsertSprite = new wxMenuItem( mMenuEditor, EDITOR_MENU_INSERT_SPRITE, wxString( wxT("Insert Sprite") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuEditor->Append( mMenuInsertSprite );
	
	wxMenuItem* m_separator6;
	m_separator6 = mMenuEditor->AppendSeparator();
	
	wxMenuItem* mMenuMoveBack;
	mMenuMoveBack = new wxMenuItem( mMenuEditor, EDITOR_MENU_MOVE_BACK, wxString( wxT("Move Back") ) + wxT('\t') + wxT("CTRL+b"), wxEmptyString, wxITEM_NORMAL );
	mMenuEditor->Append( mMenuMoveBack );
	
	wxMenuItem* mMenuMoveFront;
	mMenuMoveFront = new wxMenuItem( mMenuEditor, EDITOR_MENU_MOVE_FRONT, wxString( wxT("Move Front") ) + wxT('\t') + wxT("CTRL+f"), wxEmptyString, wxITEM_NORMAL );
	mMenuEditor->Append( mMenuMoveFront );
	
	mMenuBar->Append( mMenuEditor, wxT("Editor") ); 
	
	mMenuAnimation = new wxMenu();
	wxMenuItem* mMenuAnimationPlay;
	mMenuAnimationPlay = new wxMenuItem( mMenuAnimation, EDITOR_MENU_ANIMATION_PLAY, wxString( wxT("Play") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAnimation->Append( mMenuAnimationPlay );
	
	wxMenuItem* mMenuAnimationStop;
	mMenuAnimationStop = new wxMenuItem( mMenuAnimation, EDITOR_MENU_ANIMATION_STOP, wxString( wxT("Stop") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAnimation->Append( mMenuAnimationStop );
	
	wxMenuItem* mMenuAnimationAddKeyFrame;
	mMenuAnimationAddKeyFrame = new wxMenuItem( mMenuAnimation, EDITOR_MENU_ANIMATION_ADD_KEYFRAME, wxString( wxT("Add Key Frame") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAnimation->Append( mMenuAnimationAddKeyFrame );
	
	wxMenuItem* mMenuAnimationDeleteKeyFrame;
	mMenuAnimationDeleteKeyFrame = new wxMenuItem( mMenuAnimation, EDITOR_MENU_ANIMATION_DELETE_KEYFRAME, wxString( wxT("Delete Key Frame") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAnimation->Append( mMenuAnimationDeleteKeyFrame );
	
	mMenuBar->Append( mMenuAnimation, wxT("Animation") ); 
	
	this->SetMenuBar( mMenuBar );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	m_panel7 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer38;
	fgSizer38 = new wxFlexGridSizer( 1, 12, 0, 0 );
	fgSizer38->SetFlexibleDirection( wxBOTH );
	fgSizer38->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxString mRadioBoxTransformChoices[] = { wxT("X"), wxT("Y"), wxT("X-Y") };
	int mRadioBoxTransformNChoices = sizeof( mRadioBoxTransformChoices ) / sizeof( wxString );
	mRadioBoxTransform = new wxRadioBox( m_panel7, wxID_ANY, wxT("Transform"), wxDefaultPosition, wxDefaultSize, mRadioBoxTransformNChoices, mRadioBoxTransformChoices, 1, wxRA_SPECIFY_ROWS );
	mRadioBoxTransform->SetSelection( 2 );
	fgSizer38->Add( mRadioBoxTransform, 0, wxALL, 5 );
	
	m_staticline1 = new wxStaticLine( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer38->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_button10 = new wxButton( m_panel7, LAYOUT_VERTICAL_CENTRE, wxT("Centre Vertical"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	fgSizer38->Add( m_button10, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_button111 = new wxButton( m_panel7, LAYOUT_HORIZONTAL_CENTRE, wxT("Centre Horizontal"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	fgSizer38->Add( m_button111, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline2 = new wxStaticLine( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer38->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText27 = new wxStaticText( m_panel7, wxID_ANY, wxT("Bounding Box Color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText27->Wrap( -1 );
	fgSizer38->Add( m_staticText27, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mColorPickerBoundingBox = new wxColourPickerCtrl( m_panel7, COLOR_PICK_BOUNDING_BOX, *wxBLACK, wxDefaultPosition, wxSize( -1,-1 ), wxCLRP_DEFAULT_STYLE );
	fgSizer38->Add( mColorPickerBoundingBox, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticline3 = new wxStaticLine( m_panel7, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer38->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText28 = new wxStaticText( m_panel7, wxID_ANY, wxT("Bounding Box Select Color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	fgSizer38->Add( m_staticText28, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mColorPickerBoundingBoxSelect = new wxColourPickerCtrl( m_panel7, COLOR_PICK_BOUNDING_BOX_SELECT, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer38->Add( mColorPickerBoundingBoxSelect, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText281 = new wxStaticText( m_panel7, wxID_ANY, wxT("Background Color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText281->Wrap( -1 );
	fgSizer38->Add( m_staticText281, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	mColorPickerBackground = new wxColourPickerCtrl( m_panel7, COLOR_PICK_BOUNDING_BOX_SELECT, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer38->Add( mColorPickerBackground, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_panel7->SetSizer( fgSizer38 );
	m_panel7->Layout();
	fgSizer38->Fit( m_panel7 );
	bSizer18->Add( m_panel7, 0, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 1, 3, 0, 0 );
	fgSizer11->AddGrowableCol( 0 );
	fgSizer11->AddGrowableRow( 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	mNotebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_panel2 = new wxPanel( mNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	mSceneGraphTree = new wxTreeCtrl( m_panel2, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT|wxTR_MULTIPLE );
	mSceneGraphTree->SetMinSize( wxSize( 250,800 ) );
	
	bSizer1->Add( mSceneGraphTree, 1, wxALL|wxEXPAND, 5 );
	
	m_panel2->SetSizer( bSizer1 );
	m_panel2->Layout();
	bSizer1->Fit( m_panel2 );
	mNotebook->AddPage( m_panel2, wxT("Scene Graph"), false );
	
	bSizer31->Add( mNotebook, 0, 0, 10 );
	
	fgSizer11->Add( bSizer31, 0, 0, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	mButtonProperties = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	mButtonProperties->SetScrollRate( 5, 5 );
	mButtonProperties->Hide();
	mButtonProperties->SetMinSize( wxSize( 250,800 ) );
	
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer7;
	sbSizer7 = new wxStaticBoxSizer( new wxStaticBox( mButtonProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 8, 2, 0, 0 );
	
	m_staticText38 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	gSizer1->Add( m_staticText38, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextButtonName = new wxTextCtrl( mButtonProperties, TEXT_BUTTON_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer1->Add( mTextButtonName, 0, wxALL, 5 );
	
	m_staticText391 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391->Wrap( -1 );
	gSizer1->Add( m_staticText391, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextButtonWidth = new wxTextCtrl( mButtonProperties, TEXT_BUTTON_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer1->Add( mTextButtonWidth, 0, wxALL, 5 );
	
	m_staticText3911 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3911->Wrap( -1 );
	gSizer1->Add( m_staticText3911, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextButtonHeight = new wxTextCtrl( mButtonProperties, TEXT_BUTTON_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer1->Add( mTextButtonHeight, 0, wxALL, 5 );
	
	m_staticText411 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText411->Wrap( -1 );
	gSizer1->Add( m_staticText411, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextButtonX = new wxTextCtrl( mButtonProperties, TEXT_BUTTON_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer1->Add( mTextButtonX, 0, wxALL, 5 );
	
	m_staticText4111 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4111->Wrap( -1 );
	gSizer1->Add( m_staticText4111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextButtonY = new wxTextCtrl( mButtonProperties, TEXT_BUTTON_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer1->Add( mTextButtonY, 0, wxALL, 5 );
	
	m_staticText341 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText341->Wrap( -1 );
	gSizer1->Add( m_staticText341, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextButtonText = new wxTextCtrl( mButtonProperties, TEXT_BUTTON_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer1->Add( mTextButtonText, 0, wxALL, 5 );
	
	m_staticText35 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	gSizer1->Add( m_staticText35, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mColorPickerButton = new wxColourPickerCtrl( mButtonProperties, COLOR_PICKER_BUTTON, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer1->Add( mColorPickerButton, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText57 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Transparency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText57->Wrap( -1 );
	gSizer1->Add( m_staticText57, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mSliderButtonAlpha = new wxSlider( mButtonProperties, SLIDER_BUTTON_ALPHA, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gSizer1->Add( mSliderButtonAlpha, 0, wxALL, 5 );
	
	sbSizer7->Add( gSizer1, 0, 0, 5 );
	
	fgSizer19->Add( sbSizer7, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( mButtonProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText36 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Texture:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	sbSizer61->Add( m_staticText36, 0, wxALL, 5 );
	
	mFilePickButtonTexture = new wxFilePickerCtrl( mButtonProperties, FILE_PICK_BUTTON_TEXTURE, wxEmptyString, wxT("Select a file"), wxT("*.png"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE );
	sbSizer61->Add( mFilePickButtonTexture, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer1221;
	fgSizer1221 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1221->SetFlexibleDirection( wxBOTH );
	fgSizer1221->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mButtonButtonClearTexture = new wxButton( mButtonProperties, BUTTON_BUTTON_CLEAR_TEXTURE, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1221->Add( mButtonButtonClearTexture, 0, wxALL, 5 );
	
	mButtonButtonCreateUV = new wxButton( mButtonProperties, BUTTON_BUTTON_CREATE_UV, wxT("Create UV"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1221->Add( mButtonButtonCreateUV, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer61->Add( fgSizer1221, 0, 0, 5 );
	
	mOnPressTextureLabel = new wxStaticText( mButtonProperties, wxID_ANY, wxT("OnPress Texture:"), wxDefaultPosition, wxDefaultSize, 0 );
	mOnPressTextureLabel->Wrap( -1 );
	sbSizer61->Add( mOnPressTextureLabel, 0, wxALL, 5 );
	
	mFilePickButtonOnPressTexture = new wxFilePickerCtrl( mButtonProperties, FILE_PICK_BUTTON_ONPRESS_TEXTURE, wxEmptyString, wxT("Select a file"), wxT("*.png"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sbSizer61->Add( mFilePickButtonOnPressTexture, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer12213;
	fgSizer12213 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer12213->SetFlexibleDirection( wxBOTH );
	fgSizer12213->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mButtonButtonClearOnPressTexture = new wxButton( mButtonProperties, BUTTON_BUTTON_ONPRESS_CLEAR_TEXTURE, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12213->Add( mButtonButtonClearOnPressTexture, 0, wxALL, 5 );
	
	mButtonButtonOnPressCreateUV = new wxButton( mButtonProperties, BUTTON_BUTTON_ONPRESS_CREATE_UV, wxT("Create UV"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12213->Add( mButtonButtonOnPressCreateUV, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer61->Add( fgSizer12213, 1, wxEXPAND, 5 );
	
	m_staticText37 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Font:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText37->Wrap( -1 );
	sbSizer61->Add( m_staticText37, 0, wxALL, 5 );
	
	mFilePickButtonFont = new wxFilePickerCtrl( mButtonProperties, FILE_PICK_BUTTON_FONT, wxEmptyString, wxT("Select a file"), wxT("*.xml"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sbSizer61->Add( mFilePickButtonFont, 0, wxALL, 5 );
	
	wxGridSizer* gSizer51;
	gSizer51 = new wxGridSizer( 2, 2, 0, 0 );
	
	m_staticText342 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Font Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText342->Wrap( -1 );
	gSizer51->Add( m_staticText342, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mColorPickerButtonFont = new wxColourPickerCtrl( mButtonProperties, COLOR_PICKER_BUTTON_FONT, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer51->Add( mColorPickerButtonFont, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText3521 = new wxStaticText( mButtonProperties, wxID_ANY, wxT("Font Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3521->Wrap( -1 );
	gSizer51->Add( m_staticText3521, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextButtonFontSize = new wxTextCtrl( mButtonProperties, TEXT_BUTTON_FONT_SIZE, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER );
	gSizer51->Add( mTextButtonFontSize, 0, wxALL, 5 );
	
	sbSizer61->Add( gSizer51, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer81;
	gSizer81 = new wxGridSizer( 2, 2, 0, 0 );
	
	wxString mChoiceButtonFontHorizontalChoices[] = { wxT("Left"), wxT("Right"), wxT("Centre") };
	int mChoiceButtonFontHorizontalNChoices = sizeof( mChoiceButtonFontHorizontalChoices ) / sizeof( wxString );
	mChoiceButtonFontHorizontal = new wxChoice( mButtonProperties, CHOICE_BUTTON_FONT_HORIZONTAL, wxDefaultPosition, wxDefaultSize, mChoiceButtonFontHorizontalNChoices, mChoiceButtonFontHorizontalChoices, 0 );
	mChoiceButtonFontHorizontal->SetSelection( 0 );
	gSizer81->Add( mChoiceButtonFontHorizontal, 0, wxALL, 5 );
	
	wxString mChoiceButtonFontVerticalChoices[] = { wxT("Top"), wxT("Bottom"), wxT("Centre") };
	int mChoiceButtonFontVerticalNChoices = sizeof( mChoiceButtonFontVerticalChoices ) / sizeof( wxString );
	mChoiceButtonFontVertical = new wxChoice( mButtonProperties, CHOICE_BUTTON_FONT_VERTICAL, wxDefaultPosition, wxDefaultSize, mChoiceButtonFontVerticalNChoices, mChoiceButtonFontVerticalChoices, 0 );
	mChoiceButtonFontVertical->SetSelection( 0 );
	gSizer81->Add( mChoiceButtonFontVertical, 0, wxALL, 5 );
	
	sbSizer61->Add( gSizer81, 0, 0, 5 );
	
	fgSizer19->Add( sbSizer61, 0, wxEXPAND|wxALL, 5 );
	
	mButtonProperties->SetSizer( fgSizer19 );
	mButtonProperties->Layout();
	fgSizer19->Fit( mButtonProperties );
	bSizer7->Add( mButtonProperties, 1, wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	mClearPanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	mClearPanel->SetScrollRate( 5, 5 );
	mClearPanel->Hide();
	mClearPanel->SetMinSize( wxSize( 250,800 ) );
	
	bSizer7->Add( mClearPanel, 1, wxEXPAND, 5 );
	
	mSliderProperties = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	mSliderProperties->SetScrollRate( 5, 5 );
	mSliderProperties->Hide();
	mSliderProperties->SetMinSize( wxSize( 250,800 ) );
	
	wxFlexGridSizer* fgSizer192;
	fgSizer192 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer192->SetFlexibleDirection( wxBOTH );
	fgSizer192->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer72;
	sbSizer72 = new wxStaticBoxSizer( new wxStaticBox( mSliderProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	wxGridSizer* gSizer12;
	gSizer12 = new wxGridSizer( 8, 2, 0, 0 );
	
	m_staticText381 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText381->Wrap( -1 );
	gSizer12->Add( m_staticText381, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextSliderName = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderName, 0, wxALL, 5 );
	
	m_staticText3913 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3913->Wrap( -1 );
	gSizer12->Add( m_staticText3913, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextSliderWidth = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderWidth, 0, wxALL, 5 );
	
	m_staticText39112 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39112->Wrap( -1 );
	gSizer12->Add( m_staticText39112, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSliderHeight = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderHeight, 0, wxALL, 5 );
	
	m_staticText4113 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4113->Wrap( -1 );
	gSizer12->Add( m_staticText4113, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextSliderX = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderX, 0, wxALL, 5 );
	
	m_staticText41112 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41112->Wrap( -1 );
	gSizer12->Add( m_staticText41112, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSliderY = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderY, 0, wxALL, 5 );
	
	m_staticText81 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Min:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText81->Wrap( -1 );
	gSizer12->Add( m_staticText81, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSliderMin = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_MIN, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderMin, 0, wxALL, 5 );
	
	m_staticText82 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Max:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText82->Wrap( -1 );
	gSizer12->Add( m_staticText82, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSliderMax = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_MAX, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderMax, 0, wxALL, 5 );
	
	m_staticText83 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Default:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText83->Wrap( -1 );
	gSizer12->Add( m_staticText83, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSliderDefault = new wxTextCtrl( mSliderProperties, TEXT_SLIDER_DEFAULT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer12->Add( mTextSliderDefault, 0, wxALL, 5 );
	
	sbSizer72->Add( gSizer12, 0, 0, 5 );
	
	fgSizer192->Add( sbSizer72, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer612;
	sbSizer612 = new wxStaticBoxSizer( new wxStaticBox( mSliderProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText362 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Texture Bar:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText362->Wrap( -1 );
	sbSizer612->Add( m_staticText362, 0, wxALL, 5 );
	
	mFilePickSliderTextureBar = new wxFilePickerCtrl( mSliderProperties, FILE_PICK_SLIDER_TEXTURE_BAR, wxEmptyString, wxT("Select a file"), wxT("*.png"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE );
	sbSizer612->Add( mFilePickSliderTextureBar, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer12212;
	fgSizer12212 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer12212->SetFlexibleDirection( wxBOTH );
	fgSizer12212->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mButtonSliderClearTextureBar = new wxButton( mSliderProperties, BUTTON_SLIDER_CLEAR_TEXTURE_BAR, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12212->Add( mButtonSliderClearTextureBar, 0, wxALL, 5 );
	
	mButtonSliderCreateUVBar = new wxButton( mSliderProperties, BUTTON_SLIDER_CREATE_UV_BAR, wxT("Create UV"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12212->Add( mButtonSliderCreateUVBar, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer612->Add( fgSizer12212, 0, 0, 5 );
	
	m_staticText3621 = new wxStaticText( mSliderProperties, wxID_ANY, wxT("Texture Button:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3621->Wrap( -1 );
	sbSizer612->Add( m_staticText3621, 0, wxALL, 5 );
	
	mFilePickSliderTextureButton = new wxFilePickerCtrl( mSliderProperties, FILE_PICK_SLIDER_TEXTURE_BUTTON, wxEmptyString, wxT("Select a file"), wxT("*.png"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE );
	sbSizer612->Add( mFilePickSliderTextureButton, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer122121;
	fgSizer122121 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer122121->SetFlexibleDirection( wxBOTH );
	fgSizer122121->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mButtonSliderClearTextureButton = new wxButton( mSliderProperties, BUTTON_SLIDER_CLEAR_TEXTURE_BUTTON, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer122121->Add( mButtonSliderClearTextureButton, 0, wxALL, 5 );
	
	mButtonSliderCreateUVButton = new wxButton( mSliderProperties, BUTTON_SLIDER_CREATE_UV_BUTTON, wxT("Create UV"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer122121->Add( mButtonSliderCreateUVButton, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer612->Add( fgSizer122121, 1, wxEXPAND, 5 );
	
	bSizer181->Add( sbSizer612, 0, wxEXPAND|wxALL, 5 );
	
	fgSizer192->Add( bSizer181, 0, 0, 5 );
	
	mSliderProperties->SetSizer( fgSizer192 );
	mSliderProperties->Layout();
	fgSizer192->Fit( mSliderProperties );
	bSizer7->Add( mSliderProperties, 1, wxEXPAND, 5 );
	
	mSpriteProperties = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	mSpriteProperties->SetScrollRate( 5, 5 );
	mSpriteProperties->Hide();
	mSpriteProperties->SetMinSize( wxSize( 250,800 ) );
	
	wxFlexGridSizer* fgSizer1911;
	fgSizer1911 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer1911->SetFlexibleDirection( wxBOTH );
	fgSizer1911->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer711;
	sbSizer711 = new wxStaticBoxSizer( new wxStaticBox( mSpriteProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	wxGridSizer* gSizer111;
	gSizer111 = new wxGridSizer( 8, 2, 0, 0 );
	
	m_staticText3831 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3831->Wrap( -1 );
	gSizer111->Add( m_staticText3831, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextSpriteName = new wxTextCtrl( mSpriteProperties, TEXT_SPRITE_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer111->Add( mTextSpriteName, 0, wxALL, 5 );
	
	m_staticText39121 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39121->Wrap( -1 );
	gSizer111->Add( m_staticText39121, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextSpriteWidth = new wxTextCtrl( mSpriteProperties, TEXT_SPRITE_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer111->Add( mTextSpriteWidth, 0, wxALL, 5 );
	
	m_staticText391111 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText391111->Wrap( -1 );
	gSizer111->Add( m_staticText391111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSpriteHeight = new wxTextCtrl( mSpriteProperties, TEXT_SPRITE_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer111->Add( mTextSpriteHeight, 0, wxALL, 5 );
	
	m_staticText41121 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41121->Wrap( -1 );
	gSizer111->Add( m_staticText41121, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextSpriteX = new wxTextCtrl( mSpriteProperties, TEXT_SPRITE_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer111->Add( mTextSpriteX, 0, wxALL, 5 );
	
	m_staticText411111 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText411111->Wrap( -1 );
	gSizer111->Add( m_staticText411111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSpriteY = new wxTextCtrl( mSpriteProperties, TEXT_SPRITE_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer111->Add( mTextSpriteY, 0, wxALL, 5 );
	
	m_staticText3511 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3511->Wrap( -1 );
	gSizer111->Add( m_staticText3511, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mColorPickerSprite = new wxColourPickerCtrl( mSpriteProperties, COLOR_PICKER_SPRITE, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer111->Add( mColorPickerSprite, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5712 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("Transparency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5712->Wrap( -1 );
	gSizer111->Add( m_staticText5712, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mSliderSpriteAlpha = new wxSlider( mSpriteProperties, SLIDER_SPRITE_ALPHA, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gSizer111->Add( mSliderSpriteAlpha, 0, wxALL, 5 );
	
	sbSizer711->Add( gSizer111, 0, 0, 5 );
	
	fgSizer1911->Add( sbSizer711, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer6111;
	sbSizer6111 = new wxStaticBoxSizer( new wxStaticBox( mSpriteProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText3611 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("Sprite Texture:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3611->Wrap( -1 );
	sbSizer6111->Add( m_staticText3611, 0, wxALL, 5 );
	
	mFilePickSpriteTextureFile = new wxFilePickerCtrl( mSpriteProperties, FILE_PICK_SPRITE_TEXTURE_FILE, wxEmptyString, wxT("Select a file"), wxT("*.png"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE );
	sbSizer6111->Add( mFilePickSpriteTextureFile, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer122111;
	fgSizer122111 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer122111->SetFlexibleDirection( wxBOTH );
	fgSizer122111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mButtonSpriteClearTexture = new wxButton( mSpriteProperties, BUTTON_SPRITE_CLEAR_TEXTURE, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer122111->Add( mButtonSpriteClearTexture, 0, wxALL, 5 );
	
	sbSizer6111->Add( fgSizer122111, 0, 0, 5 );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText38311 = new wxStaticText( mSpriteProperties, wxID_ANY, wxT("Frames Per Second:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38311->Wrap( -1 );
	fgSizer17->Add( m_staticText38311, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextSpriteFPS = new wxTextCtrl( mSpriteProperties, TEXT_SPRITE_FPS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer17->Add( mTextSpriteFPS, 0, wxALL, 5 );
	
	sbSizer6111->Add( fgSizer17, 1, wxEXPAND, 5 );
	
	fgSizer1911->Add( sbSizer6111, 0, wxALL, 5 );
	
	mSpriteProperties->SetSizer( fgSizer1911 );
	mSpriteProperties->Layout();
	fgSizer1911->Fit( mSpriteProperties );
	bSizer7->Add( mSpriteProperties, 1, wxEXPAND, 5 );
	
	mStaticProperties = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	mStaticProperties->SetScrollRate( 5, 5 );
	mStaticProperties->SetMinSize( wxSize( 250,800 ) );
	
	wxFlexGridSizer* fgSizer191;
	fgSizer191 = new wxFlexGridSizer( 3, 1, 0, 0 );
	fgSizer191->SetFlexibleDirection( wxBOTH );
	fgSizer191->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticBoxSizer* sbSizer71;
	sbSizer71 = new wxStaticBoxSizer( new wxStaticBox( mStaticProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	wxGridSizer* gSizer11;
	gSizer11 = new wxGridSizer( 7, 2, 0, 0 );
	
	m_staticText383 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText383->Wrap( -1 );
	gSizer11->Add( m_staticText383, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextStaticName = new wxTextCtrl( mStaticProperties, TEXT_STATIC_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer11->Add( mTextStaticName, 0, wxALL, 5 );
	
	m_staticText3912 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Width:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3912->Wrap( -1 );
	gSizer11->Add( m_staticText3912, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextStaticWidth = new wxTextCtrl( mStaticProperties, TEXT_STATIC_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer11->Add( mTextStaticWidth, 0, wxALL, 5 );
	
	m_staticText39111 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39111->Wrap( -1 );
	gSizer11->Add( m_staticText39111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextStaticHeight = new wxTextCtrl( mStaticProperties, TEXT_STATIC_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer11->Add( mTextStaticHeight, 0, wxALL, 5 );
	
	m_staticText4112 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4112->Wrap( -1 );
	gSizer11->Add( m_staticText4112, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextStaticX = new wxTextCtrl( mStaticProperties, TEXT_STATIC_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer11->Add( mTextStaticX, 0, wxALL, 5 );
	
	m_staticText41111 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41111->Wrap( -1 );
	gSizer11->Add( m_staticText41111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextStaticY = new wxTextCtrl( mStaticProperties, TEXT_STATIC_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	gSizer11->Add( mTextStaticY, 0, wxALL, 5 );
	
	m_staticText351 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText351->Wrap( -1 );
	gSizer11->Add( m_staticText351, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mColorPickerStatic = new wxColourPickerCtrl( mStaticProperties, COLOR_PICKER_STATIC, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer11->Add( mColorPickerStatic, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText571 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Transparency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText571->Wrap( -1 );
	gSizer11->Add( m_staticText571, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mSliderStaticAlpha = new wxSlider( mStaticProperties, SLIDER_STATIC_ALPHA, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gSizer11->Add( mSliderStaticAlpha, 0, wxALL, 5 );
	
	sbSizer71->Add( gSizer11, 0, 0, 5 );
	
	fgSizer191->Add( sbSizer71, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( mStaticProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText3411 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3411->Wrap( -1 );
	sbSizer9->Add( m_staticText3411, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	mTextStaticText = new wxTextCtrl( mStaticProperties, TEXT_STATIC_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_PROCESS_ENTER );
	mTextStaticText->SetMinSize( wxSize( -1,70 ) );
	
	sbSizer9->Add( mTextStaticText, 1, wxALL|wxEXPAND, 5 );
	
	fgSizer191->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer611;
	sbSizer611 = new wxStaticBoxSizer( new wxStaticBox( mStaticProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText361 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Texture:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText361->Wrap( -1 );
	sbSizer611->Add( m_staticText361, 0, wxALL, 5 );
	
	mFilePickStaticTexture = new wxFilePickerCtrl( mStaticProperties, FILE_PICK_STATIC_TEXTURE, wxEmptyString, wxT("Select a file"), wxT("*.png"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE );
	sbSizer611->Add( mFilePickStaticTexture, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer12211;
	fgSizer12211 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer12211->SetFlexibleDirection( wxBOTH );
	fgSizer12211->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mButtonStaticClearTexture = new wxButton( mStaticProperties, BUTTON_STATIC_CLEAR_TEXTURE, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12211->Add( mButtonStaticClearTexture, 0, wxALL, 5 );
	
	mButtonStaticCreateUV = new wxButton( mStaticProperties, BUTTON_STATIC_CREATE_UV, wxT("Create UV"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12211->Add( mButtonStaticCreateUV, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer611->Add( fgSizer12211, 0, 0, 5 );
	
	m_staticText371 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Font:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText371->Wrap( -1 );
	sbSizer611->Add( m_staticText371, 0, wxALL, 5 );
	
	mFilePickStaticFont = new wxFilePickerCtrl( mStaticProperties, FILE_PICK_STATIC_FONT, wxEmptyString, wxT("Select a file"), wxT("*.xml"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE );
	sbSizer611->Add( mFilePickStaticFont, 0, wxALL, 5 );
	
	wxGridSizer* gSizer5;
	gSizer5 = new wxGridSizer( 3, 2, 0, 0 );
	
	m_staticText34 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Font Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText34->Wrap( -1 );
	gSizer5->Add( m_staticText34, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mColorPickerStaticFont = new wxColourPickerCtrl( mStaticProperties, COLOR_PICKER_STATIC_FONT, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	gSizer5->Add( mColorPickerStaticFont, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText5711 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Transparency:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5711->Wrap( -1 );
	gSizer5->Add( m_staticText5711, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	mSliderStaticAlphaFont = new wxSlider( mStaticProperties, SLIDER_STATIC_ALPHA_FONT, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	gSizer5->Add( mSliderStaticAlphaFont, 0, wxALL, 5 );
	
	m_staticText352 = new wxStaticText( mStaticProperties, wxID_ANY, wxT("Font Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText352->Wrap( -1 );
	gSizer5->Add( m_staticText352, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextStaticFontSize = new wxTextCtrl( mStaticProperties, TEXT_STATIC_FONT_SIZE, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER );
	gSizer5->Add( mTextStaticFontSize, 0, wxALL, 5 );
	
	sbSizer611->Add( gSizer5, 1, wxEXPAND, 5 );
	
	wxGridSizer* gSizer8;
	gSizer8 = new wxGridSizer( 2, 2, 0, 0 );
	
	wxString mChoiceStaticFontHorizontalChoices[] = { wxT("Left"), wxT("Right"), wxT("Centre") };
	int mChoiceStaticFontHorizontalNChoices = sizeof( mChoiceStaticFontHorizontalChoices ) / sizeof( wxString );
	mChoiceStaticFontHorizontal = new wxChoice( mStaticProperties, CHOICE_STATIC_FONT_HORIZONTAL, wxDefaultPosition, wxDefaultSize, mChoiceStaticFontHorizontalNChoices, mChoiceStaticFontHorizontalChoices, 0 );
	mChoiceStaticFontHorizontal->SetSelection( 0 );
	gSizer8->Add( mChoiceStaticFontHorizontal, 0, wxALL, 5 );
	
	wxString mChoiceStaticFontVerticalChoices[] = { wxT("Bottom"), wxT("Top"), wxT("Centre") };
	int mChoiceStaticFontVerticalNChoices = sizeof( mChoiceStaticFontVerticalChoices ) / sizeof( wxString );
	mChoiceStaticFontVertical = new wxChoice( mStaticProperties, CHOICE_STATIC_FONT_VERTICAL, wxDefaultPosition, wxDefaultSize, mChoiceStaticFontVerticalNChoices, mChoiceStaticFontVerticalChoices, 0 );
	mChoiceStaticFontVertical->SetSelection( 1 );
	gSizer8->Add( mChoiceStaticFontVertical, 0, wxALL, 5 );
	
	sbSizer611->Add( gSizer8, 0, 0, 5 );
	
	fgSizer191->Add( sbSizer611, 0, wxALL, 5 );
	
	mStaticProperties->SetSizer( fgSizer191 );
	mStaticProperties->Layout();
	fgSizer191->Fit( mStaticProperties );
	bSizer7->Add( mStaticProperties, 1, wxEXPAND, 5 );
	
	fgSizer11->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* mRightSideBoxSizer;
	mRightSideBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* mAnimationBoxSizer;
	mAnimationBoxSizer = new wxBoxSizer( wxVERTICAL );
	
	mAnimationControlsPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 1, 10, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText191 = new wxStaticText( mAnimationControlsPanel, wxID_ANY, wxT("Frame:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText191->Wrap( -1 );
	fgSizer15->Add( m_staticText191, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextAnimationFrame = new wxTextCtrl( mAnimationControlsPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxSize( 50,-1 ), wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer15->Add( mTextAnimationFrame, 0, wxALL, 5 );
	
	m_button9 = new wxButton( mAnimationControlsPanel, BUTTON_ID_ANIMATION_BACK_KEYFRAME, wxT("<"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	fgSizer15->Add( m_button9, 0, wxALL, 5 );
	
	m_button101 = new wxButton( mAnimationControlsPanel, BUTTON_ID_ANIMATION_FORWARD_KEYFRAME, wxT(">"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	fgSizer15->Add( m_button101, 0, wxALL, 5 );
	
	m_button5 = new wxButton( mAnimationControlsPanel, BUTTON_ID_ANIMATION_ADD_KEYFRAME, wxT("+"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer15->Add( m_button5, 0, wxALL, 5 );
	
	m_button6 = new wxButton( mAnimationControlsPanel, BUTTON_ID_ANIMATION_DELETE_KEYFRAME, wxT("-"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer15->Add( m_button6, 0, wxALL, 5 );
	
	m_button7 = new wxButton( mAnimationControlsPanel, BUTTON_ID_ANIMATION_PLAY, wxT("Play"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer15->Add( m_button7, 0, wxALL, 5 );
	
	m_button8 = new wxButton( mAnimationControlsPanel, BUTTON_ID_ANIMATION_STOP, wxT("Stop"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer15->Add( m_button8, 0, wxALL, 5 );
	
	mZoomIn = new wxButton( mAnimationControlsPanel, BUTTON_ID_ZOOM_IN, wxT("Zoom (+)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( mZoomIn, 0, wxALL, 5 );
	
	mZoomOut = new wxButton( mAnimationControlsPanel, BUTTON_ID_ZOOM_OUT, wxT("Zoom (-)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( mZoomOut, 0, wxALL, 5 );
	
	mAnimationControlsPanel->SetSizer( fgSizer15 );
	mAnimationControlsPanel->Layout();
	fgSizer15->Fit( mAnimationControlsPanel );
	mAnimationBoxSizer->Add( mAnimationControlsPanel, 0, wxEXPAND, 5 );
	
	m_panel6 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 1, 5, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mAddAnimationSequenceButton = new wxButton( m_panel6, BUTTON_ADD_ANIMATION_SEQUENCE, wxT("+"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	fgSizer12->Add( mAddAnimationSequenceButton, 0, wxALL, 5 );
	
	mDeleteAnimationSequenceButton = new wxButton( m_panel6, BUTTON_DELETE_ANIMATION_SEQUENCE, wxT("-"), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	fgSizer12->Add( mDeleteAnimationSequenceButton, 0, wxALL, 5 );
	
	mTextAnimationSequenceName = new wxTextCtrl( m_panel6, TEXT_ANIMATION_SEQUENCE_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer12->Add( mTextAnimationSequenceName, 0, wxALL, 5 );
	
	mAnimationSelectionLabel = new wxStaticText( m_panel6, wxID_ANY, wxT("Animations:"), wxDefaultPosition, wxDefaultSize, 0 );
	mAnimationSelectionLabel->Wrap( -1 );
	fgSizer12->Add( mAnimationSelectionLabel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString mAnimationsChoiceChoices;
	mAnimationsChoice = new wxChoice( m_panel6, CHOICE_ANIMATION_SELECTION, wxDefaultPosition, wxDefaultSize, mAnimationsChoiceChoices, 0 );
	mAnimationsChoice->SetSelection( 0 );
	fgSizer12->Add( mAnimationsChoice, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_panel6->SetSizer( fgSizer12 );
	m_panel6->Layout();
	fgSizer12->Fit( m_panel6 );
	mAnimationBoxSizer->Add( m_panel6, 1, wxEXPAND, 5 );
	
	mTimeLineControl = new TimeLineWidget( this, wxID_ANY, wxDefaultPosition, wxSize( -1,30 ), wxTAB_TRAVERSAL );
	mAnimationBoxSizer->Add( mTimeLineControl, 0, wxALL|wxEXPAND, 1 );
	
	bSizer10->Add( mAnimationBoxSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	mGraphicsPanel = new OGLCanvas( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	mGraphicsPanel->SetMinSize( wxSize( 10,-1 ) );
	
	bSizer9->Add( mGraphicsPanel, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer10->Add( bSizer9, 0, 0, 5 );
	
	mRightSideBoxSizer->Add( bSizer10, 0, 0, 5 );
	
	fgSizer11->Add( mRightSideBoxSizer, 0, 0, 5 );
	
	bSizer18->Add( fgSizer11, 0, 0, 5 );
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	// Connect Events
	this->Connect( mNewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionNew ) );
	this->Connect( mOpenMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionOpen ) );
	this->Connect( mSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionSaveAs ) );
	this->Connect( mSaveMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionSave ) );
	this->Connect( m_menuItem23->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuItemEditCopy->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuItemEditCut->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuItemEditPaste->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuitemEditCopySize->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuitemEditCopyPosition->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuitemEditPasteSize->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuitemEditPastePosition->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuItemEditDelete->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuItemEditShowHide->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Connect( mMenuItemOpenGL->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionRenderer ) );
	this->Connect( mMenuItemD3D->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionRenderer ) );
	this->Connect( mMenuItemPortrait320x480->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Connect( mMenuItemLandscape480x320->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Connect( mMenuItemPortrait640x960->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Connect( mMenuItemLandscape960x640->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Connect( mMenuItemPortrait640x1136->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Connect( mMenuItemLandscape1136x640->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Connect( mMenuItemPortrait720x1280->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Connect( mMenuItemLandscape1280x720->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Connect( mMenuItemPortrait768x1024->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Connect( mMenuItemLandscape1024x768->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Connect( mMenuItemPortrait1536x2048->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Connect( mMenuItemLandscape2048x1536->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Connect( mMenuInsertStatic->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Connect( mMenuInsertButton->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Connect( mMenuInsertSlider->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Connect( mMenuInsertSprite->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Connect( mMenuMoveBack->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Connect( mMenuMoveFront->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Connect( mMenuAnimationPlay->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	this->Connect( mMenuAnimationStop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	this->Connect( mMenuAnimationAddKeyFrame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	this->Connect( mMenuAnimationDeleteKeyFrame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	mRadioBoxTransform->Connect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( GuiEditorGui::OnRadioBoxTransform ), NULL, this );
	m_button10->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickLayout ), NULL, this );
	m_button111->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickLayout ), NULL, this );
	mColorPickerBoundingBox->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColourChangedBoundingBox ), NULL, this );
	mColorPickerBoundingBoxSelect->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColourChangedBoundingBox ), NULL, this );
	mColorPickerBackground->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColourChangedBackground ), NULL, this );
	mSceneGraphTree->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( GuiEditorGui::OnKeyDownSceneGraph ), NULL, this );
	mSceneGraphTree->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GuiEditorGui::OnLeftDownTree ), NULL, this );
	mSceneGraphTree->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GuiEditorGui::OnRightDownTree ), NULL, this );
	mSceneGraphTree->Connect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( GuiEditorGui::OnTreeItemMenu ), NULL, this );
	mTextButtonName->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonName->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonHeight->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonX->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonX->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonY->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonY->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonText->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mColorPickerButton->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mFilePickButtonTexture->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonButtonClearTexture->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonButtonCreateUV->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickButtonOnPressTexture->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonButtonClearOnPressTexture->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonButtonOnPressCreateUV->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickButtonFont->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mColorPickerButtonFont->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mTextButtonFontSize->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mChoiceButtonFontHorizontal->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mChoiceButtonFontVertical->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mTextSliderName->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderName->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderHeight->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderHeight->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderX->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderX->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderY->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderY->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderDefault->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderDefault->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mFilePickSliderTextureBar->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonSliderClearTextureBar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonSliderCreateUVBar->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickSliderTextureButton->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonSliderClearTextureButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonSliderCreateUVButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mTextSpriteName->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteName->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteHeight->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteHeight->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteX->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteX->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteY->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteY->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mColorPickerSprite->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mFilePickSpriteTextureFile->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonSpriteClearTexture->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mTextSpriteFPS->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteFPS->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticName->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticName->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticWidth->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticHeight->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticHeight->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticX->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticX->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticY->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticY->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mColorPickerStatic->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mTextStaticText->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mFilePickStaticTexture->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonStaticClearTexture->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonStaticCreateUV->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickStaticFont->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mColorPickerStaticFont->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mTextStaticFontSize->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mChoiceStaticFontHorizontal->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mChoiceStaticFontVertical->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mTextAnimationFrame->Connect( wxEVT_CHAR, wxKeyEventHandler( GuiEditorGui::OnCharAnimationFrame ), NULL, this );
	mTextAnimationFrame->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnterAnimationFrame ), NULL, this );
	m_button9->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button101->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button6->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	mZoomIn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickZoom ), NULL, this );
	mZoomOut->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickZoom ), NULL, this );
	mAddAnimationSequenceButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mDeleteAnimationSequenceButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mTextAnimationSequenceName->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextAnimationSequenceName->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mAnimationsChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( GuiEditorGui::OnKeyDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GuiEditorGui::OnKeyUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GuiEditorGui::OnLeftDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( GuiEditorGui::OnLeftUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_MOTION, wxMouseEventHandler( GuiEditorGui::OnMotionGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GuiEditorGui::OnRightDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_RIGHT_UP, wxMouseEventHandler( GuiEditorGui::OnRightUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_SIZE, wxSizeEventHandler( GuiEditorGui::OnSizeGraphicsPanel ), NULL, this );
}

GuiEditorGui::~GuiEditorGui()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionNew ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionSave ) );
	this->Disconnect( EDIT_UNDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_COPY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_CUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_PASTE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_COPY_SIZE_DATA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_COPY_POSITION_DATA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_PASTE_SIZE_DATA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_PASTE_POSITION_DATA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( EDIT_SHOW_HIDE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEdit ) );
	this->Disconnect( MENU_RENDERER_OPENGL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionRenderer ) );
	this->Disconnect( MENU_RENDERER_D3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionRenderer ) );
	this->Disconnect( SCREEN_SIZE_PORTRAIT_320_480, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Disconnect( SCREEN_SIZE_LANDSCAPE_480_320, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Disconnect( SCREEN_SIZE_PORTRAIT_640_960, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Disconnect( SCREEN_SIZE_LANDSCAPE_960_640, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Disconnect( SCREEN_SIZE_PORTRAIT_640_1136, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Disconnect( SCREEN_SIZE_LANDSCAPE_1136_640, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Disconnect( SCREEN_SIZE_PORTRAIT_720_1280, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Disconnect( SCREEN_SIZE_LANDSCAPE_1280_720, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Disconnect( SCREEN_SIZE_PORTRAIT_768_1024, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Disconnect( SCREEN_SIZE_LANDSCAPE_1024_768, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Disconnect( SCREEN_SIZE_PORTRAIT_1536_2048, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionPortrait ) );
	this->Disconnect( SCREEN_SIZE_LANDSCAPE_2048_1536, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionLandscape ) );
	this->Disconnect( EDITOR_MENU_INSERT_STATIC, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Disconnect( EDITOR_MENU_INSERT_BUTTON, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Disconnect( EDITOR_MENU_INSERT_SLIDER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Disconnect( EDITOR_MENU_INSERT_SPRITE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Disconnect( EDITOR_MENU_MOVE_BACK, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Disconnect( EDITOR_MENU_MOVE_FRONT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionEditor ) );
	this->Disconnect( EDITOR_MENU_ANIMATION_PLAY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	this->Disconnect( EDITOR_MENU_ANIMATION_STOP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	this->Disconnect( EDITOR_MENU_ANIMATION_ADD_KEYFRAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	this->Disconnect( EDITOR_MENU_ANIMATION_DELETE_KEYFRAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GuiEditorGui::OnMenuSelectionAnimation ) );
	mRadioBoxTransform->Disconnect( wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler( GuiEditorGui::OnRadioBoxTransform ), NULL, this );
	m_button10->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickLayout ), NULL, this );
	m_button111->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickLayout ), NULL, this );
	mColorPickerBoundingBox->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColourChangedBoundingBox ), NULL, this );
	mColorPickerBoundingBoxSelect->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColourChangedBoundingBox ), NULL, this );
	mColorPickerBackground->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColourChangedBackground ), NULL, this );
	mSceneGraphTree->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( GuiEditorGui::OnKeyDownSceneGraph ), NULL, this );
	mSceneGraphTree->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GuiEditorGui::OnLeftDownTree ), NULL, this );
	mSceneGraphTree->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GuiEditorGui::OnRightDownTree ), NULL, this );
	mSceneGraphTree->Disconnect( wxEVT_COMMAND_TREE_ITEM_MENU, wxTreeEventHandler( GuiEditorGui::OnTreeItemMenu ), NULL, this );
	mTextButtonName->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonName->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonHeight->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonX->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonX->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonY->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonY->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextButtonText->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextButtonText->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mColorPickerButton->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderButtonAlpha->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mFilePickButtonTexture->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonButtonClearTexture->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonButtonCreateUV->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickButtonOnPressTexture->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonButtonClearOnPressTexture->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonButtonOnPressCreateUV->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickButtonFont->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mColorPickerButtonFont->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mTextButtonFontSize->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mChoiceButtonFontHorizontal->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mChoiceButtonFontVertical->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mTextSliderName->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderName->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderHeight->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderHeight->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderX->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderX->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderY->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderY->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSliderDefault->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSliderDefault->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mFilePickSliderTextureBar->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonSliderClearTextureBar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonSliderCreateUVBar->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickSliderTextureButton->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonSliderClearTextureButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonSliderCreateUVButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mTextSpriteName->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteName->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteHeight->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteHeight->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteX->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteX->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextSpriteY->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteY->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mColorPickerSprite->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderSpriteAlpha->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mFilePickSpriteTextureFile->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonSpriteClearTexture->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mTextSpriteFPS->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextSpriteFPS->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticName->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticName->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticWidth->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticHeight->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticHeight->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticX->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticX->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mTextStaticY->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticY->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mColorPickerStatic->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlpha->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mTextStaticText->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextStaticText->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mFilePickStaticTexture->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mButtonStaticClearTexture->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mButtonStaticCreateUV->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mFilePickStaticFont->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( GuiEditorGui::OnFileChanged ), NULL, this );
	mColorPickerStaticFont->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( GuiEditorGui::OnColorChanged ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScroll ), NULL, this );
	mSliderStaticAlphaFont->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( GuiEditorGui::OnScrollChanged ), NULL, this );
	mTextStaticFontSize->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mChoiceStaticFontHorizontal->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mChoiceStaticFontVertical->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mTextAnimationFrame->Disconnect( wxEVT_CHAR, wxKeyEventHandler( GuiEditorGui::OnCharAnimationFrame ), NULL, this );
	mTextAnimationFrame->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnterAnimationFrame ), NULL, this );
	m_button9->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button101->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button6->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickAnimation ), NULL, this );
	mZoomIn->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickZoom ), NULL, this );
	mZoomOut->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClickZoom ), NULL, this );
	mAddAnimationSequenceButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mDeleteAnimationSequenceButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GuiEditorGui::OnButtonClick ), NULL, this );
	mTextAnimationSequenceName->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( GuiEditorGui::OnKillFocus ), NULL, this );
	mTextAnimationSequenceName->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( GuiEditorGui::OnTextEnter ), NULL, this );
	mAnimationsChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( GuiEditorGui::OnChoice ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( GuiEditorGui::OnKeyDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( GuiEditorGui::OnKeyUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( GuiEditorGui::OnLeftDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( GuiEditorGui::OnLeftUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_MOTION, wxMouseEventHandler( GuiEditorGui::OnMotionGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( GuiEditorGui::OnRightDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_RIGHT_UP, wxMouseEventHandler( GuiEditorGui::OnRightUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_SIZE, wxSizeEventHandler( GuiEditorGui::OnSizeGraphicsPanel ), NULL, this );
	
}
