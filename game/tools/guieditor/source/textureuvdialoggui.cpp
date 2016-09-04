///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "textureuvdialoggui.h"

///////////////////////////////////////////////////////////////////////////

TextureUVDialogGui::TextureUVDialogGui( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 2, 6, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button4 = new wxButton( this, wxID_ANY, wxT("Zoom (+)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_button4, 0, wxALL, 5 );
	
	m_button5 = new wxButton( this, wxID_ANY, wxT("Zoom (-)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_button5, 0, wxALL, 5 );
	
	m_button51 = new wxButton( this, wxID_ANY, wxT("Copy Size"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_button51, 0, wxALL, 5 );
	
	m_button511 = new wxButton( this, wxID_ANY, wxT("Copy Position"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_button511, 0, wxALL, 5 );
	
	m_button512 = new wxButton( this, wxID_ANY, wxT("Paste Size"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_button512, 0, wxALL, 5 );
	
	m_button513 = new wxButton( this, wxID_ANY, wxT("Paste Position"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( m_button513, 0, wxALL, 5 );
	
	bSizer5->Add( fgSizer3, 0, 0, 5 );
	
	mImagePanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer5->Add( mImagePanel, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 3, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Width/Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer6->Add( m_staticText9, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextUVWidth = new wxTextCtrl( this, UV_PROPERTIES_WIDTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( mTextUVWidth, 0, wxALL, 5 );
	
	mTextUVHeight = new wxTextCtrl( this, UV_PROPERTIES_HEIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( mTextUVHeight, 0, wxALL, 5 );
	
	m_staticText11 = new wxStaticText( this, wxID_ANY, wxT("Position:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer6->Add( m_staticText11, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	mTextUVX = new wxTextCtrl( this, UV_PROPERTIES_POSITION_X, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( mTextUVX, 0, wxALL, 5 );
	
	mTextUVY = new wxTextCtrl( this, UV_PROPERTIES_POSITION_Y, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	fgSizer6->Add( mTextUVY, 0, wxALL, 5 );
	
	bSizer5->Add( fgSizer6, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_button1, 0, wxALL, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_button2, 0, wxALL, 5 );
	
	bSizer5->Add( fgSizer2, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( bSizer5 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( TextureUVDialogGui::OnInitDialog ) );
	m_button4->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickMagnified ), NULL, this );
	m_button5->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickMinified ), NULL, this );
	m_button51->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickCopySize ), NULL, this );
	m_button511->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickCopyPosition ), NULL, this );
	m_button512->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickPasteSize ), NULL, this );
	m_button513->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickPastePosition ), NULL, this );
	mImagePanel->Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( TextureUVDialogGui::OnEraseBackground ), NULL, this );
	mImagePanel->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( TextureUVDialogGui::OnKeyDownImagePanel ), NULL, this );
	mImagePanel->Connect( wxEVT_KEY_UP, wxKeyEventHandler( TextureUVDialogGui::OnKeyUpImagePanel ), NULL, this );
	mImagePanel->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextureUVDialogGui::OnLeftDownImagePanel ), NULL, this );
	mImagePanel->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( TextureUVDialogGui::OnLeftUpImagePanel ), NULL, this );
	mImagePanel->Connect( wxEVT_MOTION, wxMouseEventHandler( TextureUVDialogGui::OnMotionImagePanel ), NULL, this );
	mImagePanel->Connect( wxEVT_PAINT, wxPaintEventHandler( TextureUVDialogGui::OnPaint ), NULL, this );
	mTextUVWidth->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	mTextUVHeight->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	mTextUVX->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	mTextUVY->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickOK ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickCancel ), NULL, this );
}

TextureUVDialogGui::~TextureUVDialogGui()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( TextureUVDialogGui::OnInitDialog ) );
	m_button4->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickMagnified ), NULL, this );
	m_button5->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickMinified ), NULL, this );
	m_button51->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickCopySize ), NULL, this );
	m_button511->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickCopyPosition ), NULL, this );
	m_button512->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickPasteSize ), NULL, this );
	m_button513->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickPastePosition ), NULL, this );
	mImagePanel->Disconnect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( TextureUVDialogGui::OnEraseBackground ), NULL, this );
	mImagePanel->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( TextureUVDialogGui::OnKeyDownImagePanel ), NULL, this );
	mImagePanel->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( TextureUVDialogGui::OnKeyUpImagePanel ), NULL, this );
	mImagePanel->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( TextureUVDialogGui::OnLeftDownImagePanel ), NULL, this );
	mImagePanel->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( TextureUVDialogGui::OnLeftUpImagePanel ), NULL, this );
	mImagePanel->Disconnect( wxEVT_MOTION, wxMouseEventHandler( TextureUVDialogGui::OnMotionImagePanel ), NULL, this );
	mImagePanel->Disconnect( wxEVT_PAINT, wxPaintEventHandler( TextureUVDialogGui::OnPaint ), NULL, this );
	mTextUVWidth->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	mTextUVHeight->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	mTextUVX->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	mTextUVY->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( TextureUVDialogGui::OnTextEnter ), NULL, this );
	m_button1->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickOK ), NULL, this );
	m_button2->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( TextureUVDialogGui::OnButtonClickCancel ), NULL, this );
	
}
