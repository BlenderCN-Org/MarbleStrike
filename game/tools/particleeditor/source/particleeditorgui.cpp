///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "OGLCanvas.hpp"

#include "particleeditorgui.h"

///////////////////////////////////////////////////////////////////////////

ParticleEditorGui::ParticleEditorGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,900 ), wxDefaultSize );
	
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
	
	mMenuRenderer = new wxMenu();
	wxMenuItem* mMenuItemOpenGL;
	mMenuItemOpenGL = new wxMenuItem( mMenuRenderer, MENU_RENDERER_OPENGL, wxString( wxT("OpenGL") ) , wxEmptyString, wxITEM_RADIO );
	mMenuRenderer->Append( mMenuItemOpenGL );
	
	wxMenuItem* mMenuItemD3D;
	mMenuItemD3D = new wxMenuItem( mMenuRenderer, MENU_RENDERER_D3D, wxString( wxT("Direct3D") ) , wxEmptyString, wxITEM_RADIO );
	mMenuRenderer->Append( mMenuItemD3D );
	
	mMenuBar->Append( mMenuRenderer, wxT("Renderer") ); 
	
	mMenuAnimation = new wxMenu();
	wxMenuItem* mMenuAnimationPlay;
	mMenuAnimationPlay = new wxMenuItem( mMenuAnimation, EDITOR_MENU_ANIMATION_PLAY, wxString( wxT("Play") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAnimation->Append( mMenuAnimationPlay );
	
	wxMenuItem* mMenuAnimationStop;
	mMenuAnimationStop = new wxMenuItem( mMenuAnimation, EDITOR_MENU_ANIMATION_STOP, wxString( wxT("Stop") ) , wxEmptyString, wxITEM_NORMAL );
	mMenuAnimation->Append( mMenuAnimationStop );
	
	mMenuBar->Append( mMenuAnimation, wxT("Animation") ); 
	
	this->SetMenuBar( mMenuBar );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxGridSizer* gSizer31;
	gSizer31 = new wxGridSizer( 1, 2, 0, 0 );
	
	wxBoxSizer* bSizer31;
	bSizer31 = new wxBoxSizer( wxVERTICAL );
	
	bSizer31->SetMinSize( wxSize( 450,-1 ) ); 
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	mParticleProperties = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	mParticleProperties->SetScrollRate( 5, 5 );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer111;
	fgSizer111 = new wxFlexGridSizer( 13, 3, 0, 0 );
	fgSizer111->SetFlexibleDirection( wxBOTH );
	fgSizer111->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText38 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText38->Wrap( -1 );
	fgSizer111->Add( m_staticText38, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextName = new wxTextCtrl( mParticleProperties, TEXT_NAME, wxT("ParticleSystem"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextName, 0, wxALL, 5 );
	
	m_staticText41 = new wxStaticText( mParticleProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText41->Wrap( -1 );
	fgSizer111->Add( m_staticText41, 0, wxALL, 5 );
	
	m_staticText381 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Max Particles:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText381->Wrap( -1 );
	fgSizer111->Add( m_staticText381, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextMaxParticles = new wxTextCtrl( mParticleProperties, TEXT_MAX_PARTICLES, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextMaxParticles, 0, wxALL, 5 );
	
	mSliderMaxParticles = new wxSlider( mParticleProperties, SLIDER_MAX_PARTICLES, 0, 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	fgSizer111->Add( mSliderMaxParticles, 0, wxALL, 5 );
	
	m_staticText51 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Emit Time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	fgSizer111->Add( m_staticText51, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextEmitTime = new wxTextCtrl( mParticleProperties, TEXT_EMIT_TIME, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextEmitTime, 0, wxALL, 5 );
	
	m_staticText52 = new wxStaticText( mParticleProperties, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText52->Wrap( -1 );
	fgSizer111->Add( m_staticText52, 0, wxALL, 5 );
	
	m_staticText39 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Emission Rate/sec:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText39->Wrap( -1 );
	fgSizer111->Add( m_staticText39, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextEmissionMin = new wxTextCtrl( mParticleProperties, TEXT_EMISSION_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextEmissionMin, 0, wxALL, 5 );
	
	mTextEmissionMax = new wxTextCtrl( mParticleProperties, TEXT_EMISSION_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextEmissionMax, 0, wxALL, 5 );
	
	m_staticText43 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText43->Wrap( -1 );
	fgSizer111->Add( m_staticText43, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextSizeMin = new wxTextCtrl( mParticleProperties, TEXT_SIZE_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextSizeMin, 0, wxALL, 5 );
	
	mTextSizeMax = new wxTextCtrl( mParticleProperties, TEXT_SIZE_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextSizeMax, 0, wxALL, 5 );
	
	m_staticText44 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Shrink Rate/sec:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	fgSizer111->Add( m_staticText44, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextShrinkMin = new wxTextCtrl( mParticleProperties, TEXT_SHRINK_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextShrinkMin, 0, wxALL, 5 );
	
	mTextShrinkMax = new wxTextCtrl( mParticleProperties, TEXT_SHRINK_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextShrinkMax, 0, wxALL, 5 );
	
	m_staticText45 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Velocity X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	fgSizer111->Add( m_staticText45, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextVelocityXMin = new wxTextCtrl( mParticleProperties, TEXT_VELOCITY_X_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextVelocityXMin, 0, wxALL, 5 );
	
	mTextVelocityXMax = new wxTextCtrl( mParticleProperties, TEXT_VELOCITY_X_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextVelocityXMax, 0, wxALL, 5 );
	
	m_staticText46 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Velocity Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	fgSizer111->Add( m_staticText46, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	mTextVelocityYMin = new wxTextCtrl( mParticleProperties, TEXT_VELOCITY_Y_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextVelocityYMin, 0, wxALL, 5 );
	
	mTextVelocityYMax = new wxTextCtrl( mParticleProperties, TEXT_VELOCITY_Y_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextVelocityYMax, 0, wxALL, 5 );
	
	m_staticText47 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Velocity Z:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	fgSizer111->Add( m_staticText47, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextVelocityZMin = new wxTextCtrl( mParticleProperties, TEXT_VELOCITY_Z_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextVelocityZMin, 0, wxALL, 5 );
	
	mTextVelocityZMax = new wxTextCtrl( mParticleProperties, TEXT_VELOCITY_Z_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextVelocityZMax, 0, wxALL, 5 );
	
	m_staticText48 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Life:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	fgSizer111->Add( m_staticText48, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextLifeMin = new wxTextCtrl( mParticleProperties, TEXT_LIFE_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextLifeMin, 0, wxALL, 5 );
	
	mTextLifeMax = new wxTextCtrl( mParticleProperties, TEXT_LIFE_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextLifeMax, 0, wxALL, 5 );
	
	m_staticText49 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Decay/sec:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	fgSizer111->Add( m_staticText49, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextDecayMin = new wxTextCtrl( mParticleProperties, TEXT_DECAY_MIN, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextDecayMin, 0, wxALL, 5 );
	
	mTextDecayMax = new wxTextCtrl( mParticleProperties, TEXT_DECAY_MAX, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	fgSizer111->Add( mTextDecayMax, 0, wxALL, 5 );
	
	m_staticText35 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Colour:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	fgSizer111->Add( m_staticText35, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mColorPickerButtonMin = new wxColourPickerCtrl( mParticleProperties, COLOR_PICKER_MIN, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer111->Add( mColorPickerButtonMin, 0, wxALL|wxEXPAND, 5 );
	
	mColorPickerButtonMax = new wxColourPickerCtrl( mParticleProperties, COLOR_PICKER_MAX, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	fgSizer111->Add( mColorPickerButtonMax, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText211 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Alpha:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText211->Wrap( -1 );
	fgSizer111->Add( m_staticText211, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	mSliderAlphaMin = new wxSlider( mParticleProperties, SLIDER_ALPHA_MIN, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_LABELS );
	fgSizer111->Add( mSliderAlphaMin, 0, wxALL, 5 );
	
	mSliderAlphaMax = new wxSlider( mParticleProperties, SLIDER_ALPHA_MAX, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_LABELS );
	fgSizer111->Add( mSliderAlphaMax, 0, wxALL, 5 );
	
	bSizer9->Add( fgSizer111, 0, wxALL, 10 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( mParticleProperties, wxID_ANY, wxT("Forces") ), wxVERTICAL );
	
	mListBoxForces = new wxListBox( mParticleProperties, LIST_BOX_FORCES, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sbSizer2->Add( mListBoxForces, 1, wxALL|wxEXPAND, 5 );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 2, 2, 0, 0 );
	
	mButtonAddForce = new wxButton( mParticleProperties, BUTTON_ADD_FORCE, wxT("+"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( mButtonAddForce, 0, wxALL, 5 );
	
	mButtonDeleteForce = new wxButton( mParticleProperties, BUTTON_DELETE_FORCE, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer3->Add( mButtonDeleteForce, 0, wxALL, 5 );
	
	sbSizer2->Add( gSizer3, 0, 0, 5 );
	
	wxGridSizer* gSizer2;
	gSizer2 = new wxGridSizer( 2, 4, 0, 0 );
	
	m_staticText18 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText18->Wrap( -1 );
	gSizer2->Add( m_staticText18, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText19 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	gSizer2->Add( m_staticText19, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText20 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText20->Wrap( -1 );
	gSizer2->Add( m_staticText20, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	m_staticText21 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText21->Wrap( -1 );
	gSizer2->Add( m_staticText21, 0, wxALL|wxALIGN_BOTTOM, 5 );
	
	mTextForceName = new wxTextCtrl( mParticleProperties, TEXT_FORCE_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	gSizer2->Add( mTextForceName, 0, wxALL, 5 );
	
	mTextForceX = new wxTextCtrl( mParticleProperties, TEXT_FORCE_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	gSizer2->Add( mTextForceX, 0, wxALL, 5 );
	
	mTextForceY = new wxTextCtrl( mParticleProperties, TEXT_FORCE_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	gSizer2->Add( mTextForceY, 0, wxALL, 5 );
	
	mTextForceZ = new wxTextCtrl( mParticleProperties, TEXT_FORCE_Z, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_RIGHT );
	gSizer2->Add( mTextForceZ, 0, wxALL, 5 );
	
	sbSizer2->Add( gSizer2, 0, 0, 5 );
	
	bSizer9->Add( sbSizer2, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer61;
	sbSizer61 = new wxStaticBoxSizer( new wxStaticBox( mParticleProperties, wxID_ANY, wxEmptyString ), wxVERTICAL );
	
	m_staticText36 = new wxStaticText( mParticleProperties, wxID_ANY, wxT("Texture:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText36->Wrap( -1 );
	sbSizer61->Add( m_staticText36, 0, wxALL, 5 );
	
	mFilePickTexture = new wxFilePickerCtrl( mParticleProperties, FILE_PICK_TEXTURE, wxEmptyString, wxT("Select a file"), wxT("*.png"), wxDefaultPosition, wxSize( -1,-1 ), wxFLP_DEFAULT_STYLE );
	sbSizer61->Add( mFilePickTexture, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer1221;
	fgSizer1221 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer1221->SetFlexibleDirection( wxBOTH );
	fgSizer1221->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mClearTexture = new wxButton( mParticleProperties, BUTTON_CLEAR_TEXTURE, wxT("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1221->Add( mClearTexture, 0, wxALL, 5 );
	
	mCreateUV = new wxButton( mParticleProperties, BUTTON_CREATE_UV, wxT("Create UV"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1221->Add( mCreateUV, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	sbSizer61->Add( fgSizer1221, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	bSizer9->Add( sbSizer61, 0, wxEXPAND|wxALL, 10 );
	
	mParticleProperties->SetSizer( bSizer9 );
	mParticleProperties->Layout();
	bSizer9->Fit( mParticleProperties );
	bSizer7->Add( mParticleProperties, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 10 );
	
	mClearPanel = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
	mClearPanel->SetScrollRate( 5, 5 );
	mClearPanel->Hide();
	
	bSizer7->Add( mClearPanel, 1, wxEXPAND, 5 );
	
	bSizer31->Add( bSizer7, 1, wxEXPAND, 5 );
	
	gSizer31->Add( bSizer31, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	bSizer3->SetMinSize( wxSize( 480,-1 ) ); 
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_panel5 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 1, 8, 0, 0 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	mButtonPlay = new wxButton( m_panel5, BUTTON_ID_ANIMATION_PLAY, wxT("Play"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer15->Add( mButtonPlay, 0, wxALL, 5 );
	
	mButtonStop = new wxButton( m_panel5, BUTTON_ID_ANIMATION_STOP, wxT("Stop"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	fgSizer15->Add( mButtonStop, 0, wxALL, 5 );
	
	m_panel5->SetSizer( fgSizer15 );
	m_panel5->Layout();
	fgSizer15->Fit( m_panel5 );
	bSizer11->Add( m_panel5, 0, wxEXPAND, 5 );
	
	bSizer10->Add( bSizer11, 0, wxEXPAND, 5 );
	
	mGraphicsPanel = new OGLCanvas( this, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	mGraphicsPanel->SetMinSize( wxSize( 480,-1 ) );
	
	bSizer10->Add( mGraphicsPanel, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	bSizer3->Add( bSizer10, 1, wxEXPAND, 5 );
	
	gSizer31->Add( bSizer3, 1, wxEXPAND, 5 );
	
	bSizer18->Add( gSizer31, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	// Connect Events
	this->Connect( mNewMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionNew ) );
	this->Connect( mOpenMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionOpen ) );
	this->Connect( mSaveAsMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionSaveAs ) );
	this->Connect( mSaveMenuItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionSave ) );
	this->Connect( mMenuItemOpenGL->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionRenderer ) );
	this->Connect( mMenuItemD3D->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionRenderer ) );
	this->Connect( mMenuAnimationPlay->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionAnimation ) );
	this->Connect( mMenuAnimationStop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionAnimation ) );
	mTextName->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextName->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextMaxParticles->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextMaxParticles->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mTextEmitTime->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextEmitTime->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextEmissionMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextEmissionMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextEmissionMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextEmissionMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextSizeMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextSizeMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextSizeMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextSizeMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextShrinkMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextShrinkMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextShrinkMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextShrinkMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityXMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityXMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityXMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityXMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityYMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityYMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityYMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityYMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityZMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityZMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityZMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityZMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextLifeMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextLifeMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextLifeMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextLifeMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextDecayMin->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextDecayMin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextDecayMax->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextDecayMax->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mColorPickerButtonMin->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ParticleEditorGui::OnColorChanged ), NULL, this );
	mColorPickerButtonMax->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ParticleEditorGui::OnColorChanged ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mListBoxForces->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnListBox ), NULL, this );
	mButtonAddForce->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mButtonDeleteForce->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mTextForceName->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceName->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextForceX->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceX->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextForceY->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceY->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextForceZ->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceZ->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mFilePickTexture->Connect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ParticleEditorGui::OnFileChanged ), NULL, this );
	mClearTexture->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mCreateUV->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mButtonPlay->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClickAnimation ), NULL, this );
	mButtonStop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClickAnimation ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ParticleEditorGui::OnKeyDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_KEY_UP, wxKeyEventHandler( ParticleEditorGui::OnKeyUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ParticleEditorGui::OnLeftDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( ParticleEditorGui::OnLeftUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_MOTION, wxMouseEventHandler( ParticleEditorGui::OnMotionGraphicsPanel ), NULL, this );
	mGraphicsPanel->Connect( wxEVT_SIZE, wxSizeEventHandler( ParticleEditorGui::OnSizeGraphicsPanel ), NULL, this );
}

ParticleEditorGui::~ParticleEditorGui()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionNew ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionSave ) );
	this->Disconnect( MENU_RENDERER_OPENGL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionRenderer ) );
	this->Disconnect( MENU_RENDERER_D3D, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionRenderer ) );
	this->Disconnect( EDITOR_MENU_ANIMATION_PLAY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionAnimation ) );
	this->Disconnect( EDITOR_MENU_ANIMATION_STOP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnMenuSelectionAnimation ) );
	mTextName->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextName->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextMaxParticles->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextMaxParticles->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderMaxParticles->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mTextEmitTime->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextEmitTime->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextEmissionMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextEmissionMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextEmissionMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextEmissionMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextSizeMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextSizeMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextSizeMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextSizeMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextShrinkMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextShrinkMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextShrinkMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextShrinkMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityXMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityXMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityXMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityXMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityYMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityYMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityYMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityYMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityZMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityZMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextVelocityZMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextVelocityZMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextLifeMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextLifeMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextLifeMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextLifeMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextDecayMin->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextDecayMin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextDecayMax->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextDecayMax->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mColorPickerButtonMin->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ParticleEditorGui::OnColorChanged ), NULL, this );
	mColorPickerButtonMax->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ParticleEditorGui::OnColorChanged ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMin->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mSliderAlphaMax->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( ParticleEditorGui::OnSliderScroll ), NULL, this );
	mListBoxForces->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ParticleEditorGui::OnListBox ), NULL, this );
	mButtonAddForce->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mButtonDeleteForce->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mTextForceName->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceName->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextForceX->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceX->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextForceY->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceY->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mTextForceZ->Disconnect( wxEVT_KILL_FOCUS, wxFocusEventHandler( ParticleEditorGui::OnKillFocus ), NULL, this );
	mTextForceZ->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ParticleEditorGui::OnTextEnter ), NULL, this );
	mFilePickTexture->Disconnect( wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler( ParticleEditorGui::OnFileChanged ), NULL, this );
	mClearTexture->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mCreateUV->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClick ), NULL, this );
	mButtonPlay->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClickAnimation ), NULL, this );
	mButtonStop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ParticleEditorGui::OnButtonClickAnimation ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ParticleEditorGui::OnKeyDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( ParticleEditorGui::OnKeyUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ParticleEditorGui::OnLeftDownGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( ParticleEditorGui::OnLeftUpGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_MOTION, wxMouseEventHandler( ParticleEditorGui::OnMotionGraphicsPanel ), NULL, this );
	mGraphicsPanel->Disconnect( wxEVT_SIZE, wxSizeEventHandler( ParticleEditorGui::OnSizeGraphicsPanel ), NULL, this );
	
}
