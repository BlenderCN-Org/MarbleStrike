//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "command.hpp"
#include "commandmove.hpp"
#include "commandsavestate.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/game/resolutionconfigs.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guicontrol.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/gui/guislider.hpp"
#include "common/engine/gui/guisprite.hpp"
#include "common/engine/gui/guistatic.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include "guieditor.hpp"
#include "oglcanvas.hpp"
#include "textureuvdialog.hpp"
#include "timelinewidget.hpp"
#include "copydatamanager.hpp"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/list.h>
#include <wx/notebook.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static GameApp::GameType gGameType = GameApp::GAME_TYPE_GUIEDITOR;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

GuiEditor::GuiEditor( const wxString& title )
	:	GuiEditorGui( NULL )
{
	UNUSED_ALWAYS( title );    

	//
	// Initialize Image Handlers for wxWidgets
	//
	wxInitAllImageHandlers();	

	//
	// Setup base path for data files
	//
	wchar_t filename[ _MAX_PATH ];
	wchar_t exedir[ _MAX_PATH ];
	wchar_t drive[ _MAX_DRIVE ];
	wchar_t path[ _MAX_PATH ];

	::GetModuleFileName( GetModuleHandle( NULL ), filename, _MAX_PATH );
	::_wsplitpath_s( filename, drive, _MAX_DRIVE, path, _MAX_PATH, NULL, NULL, NULL, NULL );
	::_wmakepath_s( exedir, _MAX_PATH, drive, path, NULL, NULL );
	::SetCurrentDirectory( exedir );

    ResolutionConfig resolutionConfig = giPhoneiPodConfig;

	//
	// Database initialization
	//
	Database::Create();
	Database::Get()->SetResourcePath( "..\\..\\assets\\" );
	Database::Get()->SetShaderResourcePath( "" );
	Database::Get()->SetBackBufferWidth( resolutionConfig.mFrameBufferWidth );
	Database::Get()->SetBackBufferHeight( resolutionConfig.mFrameBufferHeight );

	//
	// Create application
	//	
	RendererType currentRenderer = RENDERER_OPENGL;
	GameApp::Create();
    GameApp::Get()->SetScreenWidth( resolutionConfig.mGameScreenWidth );
    GameApp::Get()->SetScreenHeight( resolutionConfig.mGameScreenHeight );
	GameApp::Get()->SetRendererType( currentRenderer );
	GameApp::Get()->SetHWND( (HWND)mGraphicsPanel->GetHandle() );
	GameApp::Get()->CreateRenderer();
	GameApp::Get()->Initialize( gGameType );	

	mGraphicsPanel->SetSize( 
		Database::Get()->GetAppScreenWidth(),
		Database::Get()->GetAppScreenHeight()
		);

	mGuiEditorApp = reinterpret_cast<GuiEditorApp*>( GameApp::Get()->GetGame() );

	mSceneGraphTree->AddRoot( "Root" );

	mCurrentSavePath = "";

	mZoomScale = 1.0f;
	mScreenWidth = 0.0f;
	mScreenHeight = 0.0f;

	mTextStaticX->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextStaticY->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextStaticWidth->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextStaticHeight->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextStaticFontSize->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );

	mTextButtonX->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextButtonY->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextButtonWidth->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextButtonHeight->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextButtonFontSize->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );

	mTextSliderX->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSliderY->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSliderWidth->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSliderHeight->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSliderMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSliderMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSliderDefault->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );

	mTextAnimationFrame->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );		

	Math::Vector4 boundingBoxColor = mGuiEditorApp->GetBoundingBoxColor();	
	mColorPickerBoundingBox->SetColour( 
		wxColour( 
		boundingBoxColor[0] * 255.0f, 
		boundingBoxColor[1] * 255.0f, 
		boundingBoxColor[2] * 255.0f, 
		boundingBoxColor[3] * 255.0f 
		) 
		);
	Math::Vector4 boundingBoxColorSelect = mGuiEditorApp->GetBoundingBoxColorSelect();	
	mColorPickerBoundingBoxSelect->SetColour( 
		wxColour( 
		boundingBoxColorSelect[0] * 255.0f, 
		boundingBoxColorSelect[1] * 255.0f, 
		boundingBoxColorSelect[2] * 255.0f, 
		boundingBoxColorSelect[3] * 255.0f 
		) 
		);

	mTimeLineControl->SetAnimationLength( 200 );
	mTimeLineControl->SetGuiEditor( this );

	mPropertyPanels[PANEL_PROPERTIES_STATIC] = mStaticProperties;
	mPropertyPanels[PANEL_PROPERTIES_BUTTON] = mButtonProperties;
	mPropertyPanels[PANEL_PROPERTIES_SLIDER] = mSliderProperties;
	mPropertyPanels[PANEL_PROPERTIES_SPRITE] = mSpriteProperties;
	mPropertyPanels[PANEL_PROPERTIES_NONE] = mClearPanel;
	TogglePanel( PANEL_PROPERTIES_NONE );

	mRadioBoxTransform->SetSelection( TRANSFORM_TRANSLATE_XY );
	mTransformType = MOVEMENT_XY;	
	mIsFiledOpened = false;

	mCurrentCopyControlID = -1;
	mIsScrollingSlider = false;

	mMenuScreenSize->Check( SCREEN_SIZE_PORTRAIT_320_480, true );

    CopyDataManager::CreateCopyDataManager();

	Fit();
}

//===========================================================================

GuiEditor::~GuiEditor()
{
	while ( !mCommandList.empty() )
	{
		Command* command = mCommandList.top();
		DELETE_PTR( command );
		mCommandList.pop();
	}

	Database::Terminate();
	GameApp::Get()->Shutdown();
	GameApp::Destroy();		

    CopyDataManager::DestroyCopyDataManager();
}

//===========================================================================

void GuiEditor::OnQuit( wxCommandEvent &WXUNUSED(event) )
{
	Close( true );
}

//===========================================================================

void GuiEditor::OnMenuSelectionRenderer( wxCommandEvent& event )
{
	RendererType rendererType = RENDERER_OPENGL;

	switch ( event.GetId() )
	{
	case MENU_RENDERER_OPENGL:
		rendererType = RENDERER_OPENGL;	
		break;
	case MENU_RENDERER_D3D:
		rendererType = RENDERER_D3D9;
		break;
	default:		
		break;
	}

	if ( Database::Get() != NULL )
	{
		Database::Terminate();
	}

	if ( GameApp::Get() != NULL )
	{
		GameApp::Get()->Shutdown();
		GameApp::Destroy();
	}

	Database::Create();
	Database::Get()->SetResourcePath( "game\\assets\\" );
	Database::Get()->SetShaderResourcePath( "game\\data\\pc\\" );

	GameApp::Create();
	GameApp::Get()->SetRendererType( rendererType );
	GameApp::Get()->SetHWND( (HWND)mGraphicsPanel->GetHandle() );
	GameApp::Get()->CreateRenderer();
	GameApp::Get()->Initialize( gGameType );

	mGuiEditorApp = reinterpret_cast<GuiEditorApp*>( GameApp::Get()->GetGame() );
}

//===========================================================================

void GuiEditor::OnMenuSelectionEditor( wxCommandEvent& event )
{	
	switch ( event.GetId() )
	{
	case EDITOR_MENU_INSERT_STATIC:
		{			
			GuiStatic* guiStatic = mGuiEditorApp->AddGuiStatic();

			wxTreeItemId root = mSceneGraphTree->GetRootItem();
			wxString newName = guiStatic->GetName();
			newName += " (Static)";
			wxTreeItemId node = mSceneGraphTree->AppendItem( root, newName );
			mSceneGraphTree->SetItemData( node, new SceneGraphNode( guiStatic->GetType(), guiStatic->GetID() ) );
			mSceneGraphTree->UnselectAll();
			mSceneGraphTree->SelectItem( node );
			SelectItem( node );
		}
		break;
	case EDITOR_MENU_INSERT_BUTTON:
		{
			GuiButton* guiButton = mGuiEditorApp->AddGuiButton();

			wxTreeItemId root = mSceneGraphTree->GetRootItem();
			wxString newName = guiButton->GetName();
			newName += " (Button)";
			wxTreeItemId node = mSceneGraphTree->AppendItem( root, newName );
			mSceneGraphTree->SetItemData( node, new SceneGraphNode( guiButton->GetType(), guiButton->GetID() ) );
			mSceneGraphTree->UnselectAll();
			mSceneGraphTree->SelectItem( node );
			SelectItem( node );
		}
		break;
	case EDITOR_MENU_INSERT_SLIDER:
		{
			GuiSlider* guiSlider = mGuiEditorApp->AddGuiSlider();

			wxTreeItemId root = mSceneGraphTree->GetRootItem();
			wxString newName = guiSlider->GetName();
			newName += " (Slider)";
			wxTreeItemId node = mSceneGraphTree->AppendItem( root, newName );
			mSceneGraphTree->SetItemData( node, new SceneGraphNode( guiSlider->GetType(), guiSlider->GetID() ) );
			mSceneGraphTree->UnselectAll();
			mSceneGraphTree->SelectItem( node );
			SelectItem( node );
		}
		break;
	case EDITOR_MENU_INSERT_SPRITE:
		{
			GuiSprite* guiSprite = mGuiEditorApp->AddGuiSprite();

			wxTreeItemId root = mSceneGraphTree->GetRootItem();
			wxString newName = guiSprite->GetName();
			newName += " (Sprite)";
			wxTreeItemId node = mSceneGraphTree->AppendItem( root, newName );
			mSceneGraphTree->SetItemData( node, new SceneGraphNode( guiSprite->GetType(), guiSprite->GetID() ) );
			mSceneGraphTree->UnselectAll();
			mSceneGraphTree->SelectItem( node );
			SelectItem( node );
		}
		break;
	case EDITOR_MENU_MOVE_BACK:
		{
			wxTreeItemId selection = GetTreeCurrentSelection();	
			if ( selection.IsOk() )
			{	
				wxString currentTextSelect = mSceneGraphTree->GetItemText( selection );

				SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
				mGuiEditorApp->MoveControlBack( sceneGraphNode->mID );
				UpdateScenegraphTree();

				wxArrayTreeItemIds selections;
				size_t numSelecdtions = mSceneGraphTree->GetSelections( selections );
				for ( size_t i = 0; i < numSelecdtions; ++i )
				{
					mSceneGraphTree->SelectItem( selections[i], false );
				}

				wxTreeItemIdValue cookie;
				wxTreeItemId currentItem = mSceneGraphTree->GetFirstChild( mSceneGraphTree->GetRootItem(), cookie );
				while ( currentItem.IsOk() )
				{                    
					wxString currentString = mSceneGraphTree->GetItemText( currentItem );
					if ( currentString == currentTextSelect )
					{
						mSceneGraphTree->SelectItem( currentItem );
						break;
					}

					currentItem = mSceneGraphTree->GetNextSibling( currentItem );
				}
			}		
		}
		break;
	case EDITOR_MENU_MOVE_FRONT:
		{
			wxTreeItemId selection = GetTreeCurrentSelection();	
			if ( selection.IsOk() )
			{	
				wxString currentTextSelect = mSceneGraphTree->GetItemText( selection );

				SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
				mGuiEditorApp->MoveControlFront( sceneGraphNode->mID );			
				UpdateScenegraphTree();

				wxArrayTreeItemIds selections;
				size_t numSelecdtions = mSceneGraphTree->GetSelections( selections );
				for ( size_t i = 0; i < numSelecdtions; ++i )
				{
					mSceneGraphTree->SelectItem( selections[i], false );
				}

				wxTreeItemIdValue cookie;
				wxTreeItemId currentItem = mSceneGraphTree->GetFirstChild( mSceneGraphTree->GetRootItem(), cookie );
				while ( currentItem.IsOk() )
				{                    
					wxString currentString = mSceneGraphTree->GetItemText( currentItem );
					if ( currentString == currentTextSelect )
					{
						mSceneGraphTree->SelectItem( currentItem );
						break;
					}

					currentItem = mSceneGraphTree->GetNextSibling( currentItem );
				}
			}		
		}
		break;
	default:
		break;
	}
}

//===========================================================================

void GuiEditor::OnMenuSelectionNew( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	int answer = wxMessageBox( "Start a new file and lose any unsaved data?", "Confirm", wxYES_NO | wxCANCEL, this );
	if ( answer == wxYES )
	{
		mCurrentSavePath = "";

		wxTreeItemId root = mSceneGraphTree->GetRootItem();
		if ( mSceneGraphTree->HasChildren( root ) )
		{
			mSceneGraphTree->DeleteChildren( root );
		}

		mGuiEditorApp->Clear();

		TogglePanel( PANEL_PROPERTIES_NONE );
	}
}

//===========================================================================

void GuiEditor::OnMenuSelectionOpen( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	wxFileDialog openDialog(
		this, 
		"Choose a file to open", 
		wxEmptyString, 
		wxEmptyString, 
		"*.xml",
		wxFD_OPEN, 
		wxDefaultPosition
		);

	if ( openDialog.ShowModal() == wxID_OK )
	{
		int answer = wxYES;
		if ( mIsFiledOpened == true )
		{
			answer = wxMessageBox( "Open new file and lose any unsaved data?", "Confirm", wxYES_NO | wxCANCEL, this );
		}

		if ( answer == wxYES )
		{
			mIsFiledOpened = true;

			wxString currentDocPath = openDialog.GetPath();
			wxString searchString("\\assets\\");
			int searchStringIndex = currentDocPath.Find( searchString );
			if ( searchStringIndex > 0 )
			{				
				int searchStringLength = searchString.Length();
				currentDocPath.Remove( 0, searchStringIndex + searchStringLength );
			}

			mGuiEditorApp->Open( currentDocPath.c_str() );

			int screenWidth = mGuiEditorApp->GetGuiWidth();
			int screenHeight = mGuiEditorApp->GetGuiHeight();
			Database::Get()->SetAppScreenWidth( screenWidth );
			Database::Get()->SetAppScreenHeight( screenHeight );
			if ( screenWidth == 480 && screenHeight == 320 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_LANDSCAPE_480_320, true );
			}
			else if ( screenWidth == 320 && screenHeight == 480 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_PORTRAIT_320_480, true );
			}
			else if ( screenWidth == 960 && screenHeight == 640 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_LANDSCAPE_960_640, true );
			}
			else if ( screenWidth == 640 && screenHeight == 960 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_PORTRAIT_640_960, true );
			}
            else if ( screenWidth == 1136 && screenHeight == 640 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_LANDSCAPE_1136_640, true );
			}
			else if ( screenWidth == 640 && screenHeight == 1136 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_PORTRAIT_640_1136, true );
			}
            else if ( screenWidth == 1280 && screenHeight == 720 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_LANDSCAPE_1280_720, true );
			}
			else if ( screenWidth == 720 && screenHeight == 1280 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_PORTRAIT_720_1280, true );
			}
			else if ( screenWidth == 1024 && screenHeight == 768 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_LANDSCAPE_1024_768, true );
			}
			else if ( screenWidth == 768 && screenHeight == 1024 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_PORTRAIT_768_1024, true );
			}
			else if ( screenWidth == 2048 && screenHeight == 1536 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_LANDSCAPE_2048_1536, true );
			}
			else if ( screenWidth == 1536 && screenHeight == 2048 )
			{
				mMenuScreenSize->Check( SCREEN_SIZE_PORTRAIT_1536_2048, true );
			}

			mScreenWidth = screenWidth;
			mScreenHeight = screenHeight;

			mZoomScale  = 0.3f;
			float floatWidth = static_cast<float>( screenWidth );
			float floatHeight = static_cast<float>( screenHeight );
			floatWidth *= mZoomScale;
			floatHeight *= mZoomScale;
			int intWidth = static_cast<int>( floatWidth );
			int intHeight = static_cast<int>( floatHeight );

			Database::Get()->SetBackBufferWidth( intWidth );
			Database::Get()->SetBackBufferHeight( intHeight );
						
			mGuiEditorApp->SetZoomScale( mZoomScale );

			mGraphicsPanel->SetMinSize( wxSize( 
				floatWidth, 
				floatHeight ) 
				);
			mGraphicsPanel->GetParent()->Layout();

			mCurrentSavePath = openDialog.GetPath();

			UpdateScenegraphTree();

			wxTreeItemId currentSelection = GetTreeCurrentSelection();
			SelectItem( currentSelection );
		}
	}
}

//===========================================================================

void GuiEditor::OnMenuSelectionSaveAs( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	SaveData( true );
}

//===========================================================================

void GuiEditor::OnMenuSelectionSave( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	if ( mCurrentSavePath == "" )
	{
		SaveData( true );
	}
	else
	{
		SaveData( false );
	}
}

//===========================================================================

void GuiEditor::OnMenuSelectionPortrait( wxCommandEvent& event )
{
	int width = 0;
	int height = 0;
	switch ( event.GetId() )
    {
    case SCREEN_SIZE_PORTRAIT_320_480:
        width = 320;
        height = 480;
        break;
    case SCREEN_SIZE_PORTRAIT_640_960:
        width = 640;
        height = 960;
        break;
    case SCREEN_SIZE_PORTRAIT_640_1136:
        width = 640;
        height = 1136;
        break;
    case SCREEN_SIZE_PORTRAIT_720_1280:
        width = 720;
        height = 1280;
        break;
	case SCREEN_SIZE_PORTRAIT_768_1024:
        width = 768;
        height = 1024;
        break;
	case SCREEN_SIZE_PORTRAIT_1536_2048:
        width = 1536;
        height = 2048;
        break;
	}

	Database::Get()->SetAppScreenWidth( width );
	Database::Get()->SetAppScreenHeight( height );
	Database::Get()->SetBackBufferWidth( width );
	Database::Get()->SetBackBufferHeight( height );

	mGraphicsPanel->SetMinSize( wxSize( width, height ) );
	mGraphicsPanel->GetParent()->Layout();
}

//===========================================================================

void GuiEditor::OnMenuSelectionLandscape( wxCommandEvent& event )
{
	int width = 0;
	int height = 0;
	switch ( event.GetId() )
	{
	case SCREEN_SIZE_LANDSCAPE_480_320:
		width = 480;
		height = 320;
		break;
	case SCREEN_SIZE_LANDSCAPE_960_640:
		width = 960;
		height = 640;
		break;
    case SCREEN_SIZE_LANDSCAPE_1136_640:
        width = 1136;
        height = 640;        
        break;
    case SCREEN_SIZE_LANDSCAPE_1280_720:
        width = 1280;
        height = 720;
        break;
    case SCREEN_SIZE_LANDSCAPE_1024_768:
        width = 1024;
        height = 768;
        break;
    case SCREEN_SIZE_LANDSCAPE_2048_1536:
        width = 2048;
        height = 1536;
        break;
	}

	Database::Get()->SetAppScreenWidth( width );
	Database::Get()->SetAppScreenHeight( height );
	Database::Get()->SetBackBufferWidth( width );
	Database::Get()->SetBackBufferHeight( height );

	mGraphicsPanel->SetMinSize( wxSize( width, height ) );
	mGraphicsPanel->GetParent()->Layout();
}

//===========================================================================

void GuiEditor::OnMenuSelectionEdit( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
	case EDIT_SHOW_HIDE:
		{
			wxArrayTreeItemIds selectionList;
			unsigned int numSelections = mSceneGraphTree->GetSelections( selectionList );
			for ( unsigned int i = 0; i < numSelections; ++i )
			{        
				wxTreeItemId selection = selectionList[i];	               
				if ( selection.IsOk() )
				{	
					SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );                
					GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );
					const wxColour* colour = wxBLACK;
					if ( guiControl->GetVisible()  )
					{
						colour = wxRED;
					}
					mSceneGraphTree->SetItemTextColour( selection, *colour );
					guiControl->SetVisible( !guiControl->GetVisible() );
				}	
			}
		}
		break;
	case EDIT_PASTE:
		{
			if ( mCurrentCopyControlID >= 0 )
			{
				GuiControl* guiControl = mGuiEditorApp->GetGuiControl( mCurrentCopyControlID );
				mGuiEditorApp->CopyGuiControl( guiControl );

				UpdateScenegraphTree();

				mCurrentCopyControlID  = -1;
			}			
		}
		break;
    case EDIT_COPY_SIZE_DATA:
		{
			wxTreeItemId selection = GetTreeCurrentSelection();	
			if ( selection.IsOk() )
			{	
				SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
				GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

                Math::Vector2 data;
                data[0] = guiControl->GetWidth();
                data[1] = guiControl->GetHeight();
                CopyDataManager::GetInstance()->SetCopySizeData( data );
			}			
		}
		break;
    case EDIT_COPY_POSITION_DATA:
		{
            wxTreeItemId selection = GetTreeCurrentSelection();	
            if ( selection.IsOk() )
            {	
                SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
                GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

                Math::Vector2 data;
                data[0] = guiControl->GetX();
                data[1] = guiControl->GetY();
                CopyDataManager::GetInstance()->SetCopyPositionData( data );
            }			
        }
		break;
    case EDIT_PASTE_SIZE_DATA:
		{
			wxTreeItemId selection = GetTreeCurrentSelection();	
			if ( selection.IsOk() )
			{	
				SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
				GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

                Math::Vector2 data = CopyDataManager::GetInstance()->GetCopySizeData();
                guiControl->SetSize( data[0], data[1] );

				UpdateProperties( guiControl );
			}			
		}
		break;
	case EDIT_PASTE_POSITION_DATA:
		{
			wxTreeItemId selection = GetTreeCurrentSelection();	
			if ( selection.IsOk() )
			{	
				SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
				GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

                Math::Vector2 data = CopyDataManager::GetInstance()->GetCopyPositionData();
				guiControl->SetPosition( data[0], data[1] );

				UpdateProperties( guiControl );
			}			
		}
		break;
	case EDIT_COPY:
		{			
			wxTreeItemId selection = GetTreeCurrentSelection();	
			if ( selection.IsOk() )
			{	
				SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
				GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );
				mCurrentCopyControlID = guiControl->GetID();
			}		
		}
		break;
	case EDIT_DELETE:
		{
			wxArrayTreeItemIds selectionList;
			unsigned int numSelections = mSceneGraphTree->GetSelections( selectionList );
			for ( unsigned int i = 0; i < numSelections; ++i )
			{      
				wxTreeItemId selection = selectionList[i];	               
				if ( selection.IsOk() )
				{	
					SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
					mGuiEditorApp->DeleteGuiControl( sceneGraphNode->mID );					
				}
			}         

			UpdateScenegraphTree();
		}
		break;
	case EDIT_CUT:
		break;
	case EDIT_UNDO:
		{
			if ( !mCommandList.empty() )
			{
				Command* command = mCommandList.top();
				if ( command )
				{	
					switch ( command->GetCommandType() )
					{
					case COMMAND_SAVE_STATE:
						{
							CommandSaveState* commandSaveState = reinterpret_cast<CommandSaveState*>( command );
							GuiControl* guiControl = commandSaveState->GetControl();							
							UpdateProperties( guiControl );							
							UpdateAnimationPanel( guiControl );
							command->Undo();
						}
						break;
					}
					DELETE_PTR( command );
					mCommandList.pop();
				}
			}
			mCurrentCopyControlID = -1;
		}
		break;
	default:
		break;
	}
}

//===========================================================================

void GuiEditor::OnMenuSelectionAnimation( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
	case EDITOR_MENU_ANIMATION_PLAY:
		{		
			PlayAnimation();
		}
		break;
	case EDITOR_MENU_ANIMATION_STOP:
		{	
			StopAnimation();
		}
		break;
	case EDITOR_MENU_ANIMATION_ADD_KEYFRAME:
		{
			AddKeyFrame();
		}
		break;
	case EDITOR_MENU_ANIMATION_DELETE_KEYFRAME:
		{
			DeleteKeyFrame();
		}
		break;
	default:
		break;
	}
}

//===========================================================================

void GuiEditor::OnTextEnter( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
	case TEXT_STATIC_NAME:
	case TEXT_STATIC_WIDTH:
	case TEXT_STATIC_HEIGHT:
	case TEXT_STATIC_X:
	case TEXT_STATIC_Y:
	case TEXT_STATIC_TEXT:
	case TEXT_STATIC_FONT_SIZE:
		OnTextEnterStatic( event );
		break;
	case TEXT_BUTTON_NAME:
	case TEXT_BUTTON_WIDTH:
	case TEXT_BUTTON_HEIGHT:
	case TEXT_BUTTON_X:
	case TEXT_BUTTON_Y:
	case TEXT_BUTTON_TEXT:
	case TEXT_BUTTON_FONT_SIZE:
		OnTextEnterButton( event );
		break;
	case TEXT_SLIDER_NAME:
	case TEXT_SLIDER_WIDTH:
	case TEXT_SLIDER_HEIGHT:
	case TEXT_SLIDER_X:
	case TEXT_SLIDER_Y:
	case TEXT_SLIDER_MIN:
	case TEXT_SLIDER_MAX:
	case TEXT_SLIDER_DEFAULT:
		OnTextEnterSlider( event );
		break;
	case TEXT_SPRITE_NAME:
	case TEXT_SPRITE_WIDTH:
	case TEXT_SPRITE_HEIGHT:
	case TEXT_SPRITE_X:
	case TEXT_SPRITE_Y:
	case TEXT_SPRITE_FPS:
		OnTextEnterSprite( event );
		break;
	case TEXT_ANIMATION_SEQUENCE_NAME:
		OnTextEnterAnimationSequence( event );
		break;
	}
}

//===========================================================================

void GuiEditor::OnKillFocus( wxFocusEvent& event )
{
	switch ( event.GetId() )
	{
	case TEXT_STATIC_NAME:
	case TEXT_STATIC_WIDTH:
	case TEXT_STATIC_HEIGHT:
	case TEXT_STATIC_X:
	case TEXT_STATIC_Y:
	case TEXT_STATIC_TEXT:
		OnKillFocusStatic( event );
		break;
	case TEXT_BUTTON_NAME:
	case TEXT_BUTTON_WIDTH:
	case TEXT_BUTTON_HEIGHT:
	case TEXT_BUTTON_X:
	case TEXT_BUTTON_Y:
	case TEXT_BUTTON_TEXT:
		OnKillFocusButton();
		break;
	case TEXT_SLIDER_NAME:
	case TEXT_SLIDER_WIDTH:
	case TEXT_SLIDER_HEIGHT:
	case TEXT_SLIDER_X:
	case TEXT_SLIDER_Y:
	case TEXT_SLIDER_MIN:
	case TEXT_SLIDER_MAX:
	case TEXT_SLIDER_DEFAULT:
		OnKillFocusSlider();
		break;
	case TEXT_SPRITE_NAME:
	case TEXT_SPRITE_WIDTH:
	case TEXT_SPRITE_HEIGHT:
	case TEXT_SPRITE_X:
	case TEXT_SPRITE_Y:
		OnKillFocusSprite();
		break;
	}

	event.Skip();
}

//===========================================================================

void GuiEditor::OnScroll( wxScrollEvent& event )
{
	switch ( event.GetId() )
	{
	case SLIDER_STATIC_ALPHA:
		OnScrollStatic( event );
		break;
	case SLIDER_BUTTON_ALPHA:
		OnScrollButton( event );
		break;
	case SLIDER_STATIC_ALPHA_FONT:
		OnScrollStatic( event );
		break;
	case SLIDER_SPRITE_ALPHA:
		OnScrollSprite( event );
		break;
	}
}

//===========================================================================

void GuiEditor::OnScrollChanged(  wxScrollEvent& event )
{
	switch ( event.GetId() )
	{
	case SLIDER_STATIC_ALPHA:
		OnScrollChangedStatic( event );
		break;
	case SLIDER_BUTTON_ALPHA:
		OnScrollChangedButton( event );
		break;
	case SLIDER_STATIC_ALPHA_FONT:
		OnScrollChangedStatic( event );
		break;
	case SLIDER_SPRITE_ALPHA:
		OnScrollChangedSprite( event );
		break;
	}
}

//===========================================================================

void GuiEditor::OnChoice( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
	case CHOICE_STATIC_FONT_HORIZONTAL:
	case CHOICE_STATIC_FONT_VERTICAL:
		OnChoiceStatic( event );
		break;
	case CHOICE_BUTTON_FONT_HORIZONTAL:
	case CHOICE_BUTTON_FONT_VERTICAL:
		OnChoiceButton( event );
		break;
	case CHOICE_ANIMATION_SELECTION:
		OnChoiceAnimationSequence( event );
		break;
	}
}

//===========================================================================

void GuiEditor::OnColorChanged( wxColourPickerEvent& event )
{
	switch ( event.GetId() )
	{
	case COLOR_PICKER_STATIC:
	case COLOR_PICKER_STATIC_FONT:
		OnColorChangedStatic( event );
		break;
	case COLOR_PICKER_BUTTON:
	case COLOR_PICKER_BUTTON_FONT:
		OnColorChangedButton( event );
		break;
	case COLOR_PICKER_SPRITE:
		OnColorChangedSprite( event );
		break;
	}
}

//===========================================================================

void GuiEditor::OnButtonClick( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
	case BUTTON_STATIC_CREATE_UV:
	case BUTTON_STATIC_CLEAR_TEXTURE:
		OnButtonClickStatic( event );
		break;
	case BUTTON_BUTTON_CREATE_UV:
	case BUTTON_BUTTON_CLEAR_TEXTURE:
	case BUTTON_BUTTON_ONPRESS_CREATE_UV:
	case BUTTON_BUTTON_ONPRESS_CLEAR_TEXTURE:
		OnButtonClickButton( event );
		break;
	case BUTTON_SLIDER_CREATE_UV_BAR:
	case BUTTON_SLIDER_CREATE_UV_BUTTON:
	case BUTTON_SLIDER_CLEAR_TEXTURE_BAR:
	case BUTTON_SLIDER_CLEAR_TEXTURE_BUTTON:
		OnButtonClickSlider( event );
		break;
	case BUTTON_SPRITE_CLEAR_TEXTURE:
		OnButtonClickSprite( event );
		break;
	case BUTTON_ADD_ANIMATION_SEQUENCE:
	case BUTTON_DELETE_ANIMATION_SEQUENCE:
		OnButtonAnimationSequence( event );
		break;
	}
}

//===========================================================================

void GuiEditor::OnFileChanged( wxFileDirPickerEvent& event )
{
	switch ( event.GetId() )
	{
	case FILE_PICK_STATIC_FONT:
	case FILE_PICK_STATIC_TEXTURE:
		OnFileChangedStatic( event );
		break;
	case FILE_PICK_BUTTON_FONT:
	case FILE_PICK_BUTTON_TEXTURE:
	case FILE_PICK_BUTTON_ONPRESS_TEXTURE:
		OnFileChangedButton( event );
		break;	
	case FILE_PICK_SLIDER_TEXTURE_BAR:
	case FILE_PICK_SLIDER_TEXTURE_BUTTON:
		OnFileChangedSlider( event );
		break;	
	case FILE_PICK_SPRITE_TEXTURE_FILE:
		OnFileChangedSprite( event );
		break;
	}
}

//===========================================================================

void GuiEditor::OnKeyDownSceneGraph( wxKeyEvent& event )
{
	UNUSED_ALWAYS( event );
}

//===========================================================================

void GuiEditor::OnLeftDownTree( wxMouseEvent& event )
{
	wxPoint position = event.GetPosition();

	int flags = 0;
	wxTreeItemId foundItem = mSceneGraphTree->HitTest( position, flags );	

	SelectItem( foundItem );

	event.Skip();
}

//===========================================================================

void GuiEditor::OnTreeItemMenu( wxTreeEvent& event )
{
	UNUSED_ALWAYS( event );

	wxMenu popupMenu;
	popupMenu.Append( EDIT_SHOW_HIDE, "Show/Hide" );    
	popupMenu.Append( EDIT_DELETE, "Delete" );
	popupMenu.Connect( 
		wxEVT_COMMAND_MENU_SELECTED, 
		(wxObjectEventFunction)&GuiEditor::OnMenuSelectionEdit, 
		NULL, 
		this
		);
	PopupMenu( &popupMenu );	
}

//===========================================================================

void GuiEditor::OnRightDownTree( wxMouseEvent& event )
{
	wxPoint position = event.GetPosition();

	int flags = 0;
	wxTreeItemId foundItem = mSceneGraphTree->HitTest( position, flags );	

	if ( foundItem.IsOk() == false )
	{	
		wxMenu popupMenu;
		popupMenu.Append( EDITOR_MENU_INSERT_STATIC, "Insert Static" );	
		popupMenu.Append( EDITOR_MENU_INSERT_BUTTON, "Insert Button" );	
		popupMenu.Append( EDITOR_MENU_INSERT_SLIDER, "Insert Slider" );	
		popupMenu.Append( EDITOR_MENU_INSERT_SPRITE, "Insert Sprite" );	
		popupMenu.Connect( 
			wxEVT_COMMAND_MENU_SELECTED, 
			(wxObjectEventFunction)&GuiEditor::OnMenuSelectionEditor, 
			NULL, 
			this
			);
		PopupMenu( &popupMenu );
	}
}

//===========================================================================

void GuiEditor::OnSizeGraphicsPanel( wxSizeEvent& event )
{
	UNUSED_ALWAYS( event );

	//mGraphicsPanel->SetSize( 
		//Database::Get()->GetAppScreenWidth(),
		//Database::Get()->GetAppScreenHeight()
		//);
}

//===========================================================================

void GuiEditor::OnLeftDownGraphicsPanel( wxMouseEvent& event )
{
	mGuiEditorApp->OnLeftDown( event.GetX(), event.GetY() );

	GuiControl* control = mGuiEditorApp->GetSelected();
	if ( control )
	{
		CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
		command->SaveState( control );
		mCommandList.push( command );
	}
}

//===========================================================================

void GuiEditor::OnLeftUpGraphicsPanel( wxMouseEvent& event )
{
	mGuiEditorApp->OnLeftUp( event.GetX(), event.GetY() );	
}

//===========================================================================

void GuiEditor::OnRightDownGraphicsPanel( wxMouseEvent& event )
{
	mCurrentScrollX = event.GetX();
    mCurrentScrollY = event.GetY();
}

//===========================================================================

void GuiEditor::OnRightUpGraphicsPanel( wxMouseEvent& event )
{
	event;
}

//===========================================================================

void GuiEditor::OnMotionGraphicsPanel( wxMouseEvent& event )
{
    if ( event.LeftIsDown() )
    {
	    mGuiEditorApp->OnMotion( event.GetX(), event.GetY(), mTransformType );

	    GuiControl* guiControl = GetCurrentControl();
	    if ( guiControl )
	    {
		    UpdateProperties( guiControl );
	    }

	    mGraphicsPanel->SetFocus();
    }
    else if ( event.RightIsDown() )
    {
        int x = event.GetX();
        int y = event.GetY();
        int deltaX = x - mCurrentScrollX;
        int deltaY = y - mCurrentScrollY;

        wxPoint currentPosition = mGraphicsPanel->GetPosition();

        currentPosition.x += deltaX;
        currentPosition.y += deltaY;
        mGraphicsPanel->Move( currentPosition );
    }
}

//===========================================================================

void GuiEditor::OnKeyDownGraphicsPanel( wxKeyEvent& event )
{
	event.Skip();
}

//===========================================================================

void GuiEditor::OnKeyUpGraphicsPanel( wxKeyEvent& event )
{
	event.Skip();
}

//===========================================================================

void GuiEditor::OnRadioBoxTransform( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	switch ( mRadioBoxTransform->GetSelection() )
	{
	case TRANSFORM_TRANSLATE_XY:
		mTransformType = MOVEMENT_XY;
		break;
	case TRANSFORM_TRANSLATE_X:
		mTransformType = MOVEMENT_X;
		break;
	case TRANSFORM_TRANSLATE_Y:
		mTransformType = MOVEMENT_Y;
		break;
	}	
}

//===========================================================================

void GuiEditor::OnButtonClickLayout( wxCommandEvent& event )
{
	GuiControl* guiControl = GetCurrentControl();
	if ( guiControl )
	{
		CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
		command->SaveState( guiControl );
		mCommandList.push( command );

		switch ( event.GetId() )
		{
		case LAYOUT_VERTICAL_CENTRE:
			{
				int screenHeight = Database::Get()->GetAppScreenHeight();
				int halfScreenHeight = screenHeight / 2;				
				int controlHeight = guiControl->GetHeight();
				int halfControlHeight = controlHeight / 2;

				int y = halfScreenHeight - halfControlHeight;
				guiControl->SetY( y );
			}
			break;
		case LAYOUT_HORIZONTAL_CENTRE:
			{
				int screenWidth = Database::Get()->GetAppScreenWidth();
				int halfScreenWidth = screenWidth / 2;
				int controlWidth = guiControl->GetWidth();
				int halfControlWidth = controlWidth / 2;

				int x = halfScreenWidth - halfControlWidth;
				guiControl->SetX( x );
			}
			break;
		}	
	}
}

//===========================================================================

void GuiEditor::OnTextEnterAnimationFrame( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	wxString valueString = mTextAnimationFrame->GetValue();
	long numberValue = 0;
	valueString.ToLong( &numberValue );

	mTimeLineControl->SetCurrentFrame( numberValue );
	UpdateAnimationFrame( numberValue );
}

//===========================================================================

void GuiEditor::OnCharAnimationFrame( wxKeyEvent& event )
{	
	int keycode = event.GetKeyCode();
	if (  keycode != '.' && keycode != '-' )
	{
		mTextAnimationFrame->OnChar( event );
	}
}

//===========================================================================

void GuiEditor::UpdateAnimationFrame( int frame )
{
	wxString label = wxString::Format( wxT( "%i"), frame );
	mTextAnimationFrame->SetValue( label );

	GuiControl* guiControl = GetCurrentControl();
	if ( guiControl )
	{
		guiControl->GoToKeyFrame( frame );
		UpdateProperties( guiControl );
	}
}

//===========================================================================

void GuiEditor::OnButtonClickAnimation( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
	case BUTTON_ID_ANIMATION_ADD_KEYFRAME:
		{
			AddKeyFrame();
		}
		break;
	case BUTTON_ID_ANIMATION_DELETE_KEYFRAME:
		{
			DeleteKeyFrame();
		}
		break;
	case BUTTON_ID_ANIMATION_PLAY:
		{
			PlayAnimation();
		}
		break;
	case BUTTON_ID_ANIMATION_STOP:
		{
			StopAnimation();
		}
		break;
	case BUTTON_ID_ANIMATION_BACK_KEYFRAME:
		{
			GuiControl* guiControl = GetCurrentControl();
			if ( guiControl )
			{
				int currentFrame = mTimeLineControl->GetCurrentFrame();
				int previousFrame = guiControl->GetPreviousFrame( currentFrame );				
				guiControl->GoToKeyFrame( previousFrame );
				UpdateProperties( guiControl );
				UpdateAnimationPanel( guiControl );
				UpdateAnimationFrame( previousFrame );
				mTimeLineControl->SetCurrentFrame( previousFrame );
			}
		}
		break;
	case BUTTON_ID_ANIMATION_FORWARD_KEYFRAME:
		{
			GuiControl* guiControl = GetCurrentControl();
			if ( guiControl )
			{
				int currentFrame = mTimeLineControl->GetCurrentFrame();
				int nextFrame = guiControl->GetNextFrame( currentFrame );				
				guiControl->GoToKeyFrame( nextFrame );
				UpdateProperties( guiControl );
				UpdateAnimationPanel( guiControl );
				UpdateAnimationFrame( nextFrame );
				mTimeLineControl->SetCurrentFrame( nextFrame );
			}
		}
		break;
	default:
		break;
	}	
}

//===========================================================================

void GuiEditor::OnColourChangedBoundingBox( wxColourPickerEvent& event )
{
	if ( event.GetId() == COLOR_PICK_BOUNDING_BOX )
	{
		wxColour regularColor = mColorPickerBoundingBox->GetColour();
		Math::Vector4 color( 
			static_cast<float>( regularColor.Red() ) / 255.0f, 
			static_cast<float>( regularColor.Green() ) / 255.0f, 
			static_cast<float>( regularColor.Blue() ) / 255.0f, 
			1.0f
			);
		mGuiEditorApp->SetBoundingBoxColor( color );
	}
	else if ( event.GetId() == COLOR_PICK_BOUNDING_BOX_SELECT )
	{
		wxColour selectColor = mColorPickerBoundingBoxSelect->GetColour();
		Math::Vector4 color( 
			static_cast<float>( selectColor.Red() ) / 255.0f, 
			static_cast<float>( selectColor.Green() ) / 255.0f, 
			static_cast<float>( selectColor.Blue() ) / 255.0f, 
			1.0f
			);
		mGuiEditorApp->SetBoundingBoxColorSelect( color );
	}
}

//===========================================================================

void GuiEditor::OnColourChangedBackground( wxColourPickerEvent& event )
{
	UNUSED_ALWAYS( event );

	wxColour selectColor = mColorPickerBackground->GetColour();
	Math::Vector4 color( 
		static_cast<float>( selectColor.Red() ) / 255.0f, 
		static_cast<float>( selectColor.Green() ) / 255.0f, 
		static_cast<float>( selectColor.Blue() ) / 255.0f, 
		1.0f
		);
	mGuiEditorApp->SetBackgroundColor( color );	
}

//===========================================================================

void GuiEditor::OnButtonClickZoom( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
	case BUTTON_ID_ZOOM_IN:
		mZoomScale -= 0.1f;
		break;
	case BUTTON_ID_ZOOM_OUT:
		mZoomScale += 0.1f;
		break;
	default:
		break;
	}

	float floatWidth = static_cast<float>( mScreenWidth );
	float floatHeight = static_cast<float>( mScreenHeight );
	floatWidth *= mZoomScale;
	floatHeight *= mZoomScale;
	int intWidth = static_cast<int>( floatWidth );
	int intHeight = static_cast<int>( floatHeight );

	Database::Get()->SetBackBufferWidth( intWidth );
	Database::Get()->SetBackBufferHeight( intHeight );

	mGuiEditorApp->SetZoomScale( mZoomScale );

	mGraphicsPanel->SetMinSize( wxSize( 
		floatWidth, 
		floatHeight ) 
		);
	mGraphicsPanel->GetParent()->Layout();
}

//===========================================================================

GuiControl* GuiEditor::GetCurrentControl()
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

		return guiControl;
	}

	return NULL;
}

//===========================================================================

void GuiEditor::UpdateAnimationPanel( GuiControl* guiControl )
{	
	if ( guiControl )
	{
		mTimeLineControl->ClearKeyFrames();

		int numKeyFrames = guiControl->GetNumKeyFrames();
		for ( int i = 0; i < numKeyFrames; ++i )
		{
			AnimationController<ControlKeyFrameData>::KeyFrameData* keyFrameData = guiControl->GetKeyFrameDataByIndex( i );
			mTimeLineControl->AddKeyFrame( keyFrameData->mKeyFrame );			
		}		

		mTimeLineControl->SetCurrentFrame( 0 );
		guiControl->GoToKeyFrame( 0 );

		int currentSelection = mAnimationsChoice->GetSelection();

		mAnimationsChoice->Clear();
		int numAnimationSequences = guiControl->GetNumAnimationSequences();
		for ( int i = 0; i < numAnimationSequences; ++i )
		{
			AnimationSequence* currentAnimationSequence = guiControl->GetAnimationSquence( i );            
			mAnimationsChoice->Append( currentAnimationSequence->mAnimationSequenceName );
		}

		if ( currentSelection == -1 )
		{
			currentSelection = 0;
		}

		mAnimationsChoice->Select( currentSelection );		
		if ( currentSelection >= 0 )
		{
			wxString currentStringName = mAnimationsChoice->GetString( currentSelection );
			mTextAnimationSequenceName->SetValue( currentStringName );
		}

		guiControl->SetCurrentAnimationSequence( mAnimationsChoice->GetSelection() );
	}
}

//===========================================================================

void GuiEditor::AddKeyFrame()
{
	GuiControl* guiControl = GetCurrentControl();
	if ( guiControl )
	{	
		CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
		command->SaveState( guiControl );
		mCommandList.push( command );

		int currentFrame = mTimeLineControl->GetCurrentFrame();

		ControlKeyFrameData newData;
		newData.mPosition[0] = guiControl->GetX();
		newData.mPosition[1] = guiControl->GetY();
		newData.mSize[0] = guiControl->GetWidth();
		newData.mSize[1] = guiControl->GetHeight();
		newData.mAlpha = guiControl->GetAlpha();
		guiControl->InsertKeyFrame( newData, currentFrame );

		UpdateAnimationPanel( guiControl );

		guiControl->GoToKeyFrame( currentFrame );
		mTimeLineControl->SetCurrentFrame( currentFrame );
	}			
}

//===========================================================================

void GuiEditor::DeleteKeyFrame()
{
	GuiControl* guiControl = GetCurrentControl();
	if ( guiControl )
	{	
		CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
		command->SaveState( guiControl );
		mCommandList.push( command );

		int currentFrame = mTimeLineControl->GetCurrentFrame();
		guiControl->DeleteKeyFrame( currentFrame );
		UpdateAnimationPanel( guiControl );
	}			
}

//===========================================================================

void GuiEditor::PlayAnimation()
{
	GuiControl* guiControl = GetCurrentControl();
	if ( guiControl )
	{
		guiControl->PlayAnimation();
	}		
}

//===========================================================================

void GuiEditor::StopAnimation()
{
	GuiControl* guiControl = GetCurrentControl();
	if ( guiControl )
	{
		//guiControl->StopAnimation();
	}		
}

//===========================================================================

void GuiEditor::UpdateScenegraphTree()
{
	wxTreeItemId root = mSceneGraphTree->GetRootItem();
	if ( mSceneGraphTree->HasChildren( root ) )
	{
		mSceneGraphTree->DeleteChildren( root );
	}

	int numControls = mGuiEditorApp->GetNumControls();
	for ( int i = 0; i < numControls; ++i )
	{
		GuiControl* guiControl = mGuiEditorApp->GetGuiControlByIndex( i );
		wxTreeItemId root = mSceneGraphTree->GetRootItem();

		wxString newName = guiControl->GetName();
		switch ( guiControl->GetType() )
		{
		case CONTROL_TYPE_STATIC:
			newName += " (Static)";
			break;
		case CONTROL_TYPE_BUTTON:
			newName += " (Button)";
			break;
		case CONTROL_TYPE_CHECKBOX:
			newName += " (Checkbox)";
			break;
		case CONTROL_TYPE_SLIDER:
			newName += " (Slider)";
			break;
		case CONTROL_TYPE_RADIOBUTTON:
			newName += " (RadioButton)";
			break;
		case CONTROL_TYPE_SPRITE:
			newName += " (Sprite)";
			break;
		default:
			assert( false );
			break;
		}        

		wxTreeItemId node = mSceneGraphTree->AppendItem( root, newName );
		mSceneGraphTree->SetItemData( node, new SceneGraphNode( guiControl->GetType(), guiControl->GetID() ) );				
		if ( guiControl->GetVisible() == false )
		{
			const wxColour* colour = wxRED;
			mSceneGraphTree->SetItemTextColour( node, *colour );
		}
	}

	wxTreeItemId lastItem = mSceneGraphTree->GetLastChild( mSceneGraphTree->GetRootItem() );
	mSceneGraphTree->SelectItem( lastItem );

	GuiControl* guiControl = NULL;
	if ( lastItem.IsOk() )
	{
		SceneGraphNode* lastNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( lastItem ) );
		guiControl = mGuiEditorApp->GetGuiControl( lastNode->mID );	
		UpdateProperties( guiControl );
	}

	TogglePanels( guiControl );
}

//===========================================================================

void GuiEditor::UpdateProperties( GuiControl* guiControl )
{
	if ( guiControl )
	{
		switch ( guiControl->GetType() )
		{
		case CONTROL_TYPE_STATIC:
			{
				UpdatePropertiesStatic( reinterpret_cast<GuiStatic*>( guiControl ) );
			}
			break;
		case CONTROL_TYPE_BUTTON:
			{
				UpdatePropertiesButton( reinterpret_cast<GuiButton*>( guiControl ) );
			}
			break;
		case CONTROL_TYPE_SLIDER:
			{
				UpdatePropertiesSlider( reinterpret_cast<GuiSlider*>( guiControl ) );
			}
			break;
		case CONTROL_TYPE_SPRITE:
			{
				UpdatePropertiesSprite( reinterpret_cast<GuiSprite*>( guiControl ) );
			}
			break;
		}
	}
}

//===========================================================================

void GuiEditor::OnChoiceAnimationSequence( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

		if ( guiControl )
		{
			guiControl->SetCurrentAnimationSequence( mAnimationsChoice->GetSelection() );

			int currentSelection = mAnimationsChoice->GetSelection();
			if ( currentSelection >= 0 )
			{
				wxString currentStringName = mAnimationsChoice->GetString( currentSelection );
				mTextAnimationSequenceName->SetValue( currentStringName );
			}

			UpdateProperties( guiControl );
		}        
	}    
}

//===========================================================================

void GuiEditor::OnTextEnterAnimationSequence( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );
}

//===========================================================================

void GuiEditor::OnButtonAnimationSequence( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

		if ( guiControl )
		{
			switch ( event.GetId() )
			{
			case BUTTON_ADD_ANIMATION_SEQUENCE:
				{
					wxString value = mTextAnimationSequenceName->GetValue();
					if ( !value.IsEmpty() )
					{
						int foundIndex = guiControl->FindAnimationSequenceIndexByName( value.c_str() );
						if ( foundIndex == -1 )
						{
							guiControl->AddAnimationSequence( value.c_str() );
						}
					}
				}
				break;
			case BUTTON_DELETE_ANIMATION_SEQUENCE:
				{
					int currentSelection = mAnimationsChoice->GetSelection();
					if ( currentSelection >= 0 )
					{
						wxString currentName = mAnimationsChoice->GetString( currentSelection );
						int foundIndex = guiControl->FindAnimationSequenceIndexByName( currentName.c_str() );
						if ( foundIndex >= 0 )
						{
							guiControl->DeleteAnimationSequence( foundIndex );
						}
					}
				}
				break;
			}

			UpdateAnimationPanel( guiControl );            
		}
	}
}

//===========================================================================

void GuiEditor::TogglePanels( GuiControl* guiControl )
{
	if ( guiControl )
	{
		switch ( guiControl->GetType() )
		{
		case CONTROL_TYPE_STATIC:
			{
				TogglePanel( PANEL_PROPERTIES_STATIC );
			}
			break;
		case CONTROL_TYPE_BUTTON:
			{
				TogglePanel( PANEL_PROPERTIES_BUTTON );
			}
			break;
		case CONTROL_TYPE_SLIDER:
			{
				TogglePanel( PANEL_PROPERTIES_SLIDER );
			}
			break;
		case CONTROL_TYPE_SPRITE:
			{
				TogglePanel( PANEL_PROPERTIES_SPRITE );
			}
			break;
		}
	}
	else
	{
		TogglePanel( PANEL_PROPERTIES_NONE );
	}
}

//===========================================================================

void GuiEditor::OnTextEnterStatic( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );

		if ( guiStatic )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiStatic );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case TEXT_STATIC_NAME:
				{
					wxString name = event.GetString();
					guiStatic->SetName( name.c_str() );
					wxString graphName = name;
					graphName += " (Static)";
					mSceneGraphTree->SetItemText( selection, graphName );
				}
				break;
			case TEXT_STATIC_WIDTH:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiStatic->SetWidth( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_STATIC_HEIGHT:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiStatic->SetHeight( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_STATIC_X:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiStatic->SetX( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_STATIC_Y:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiStatic->SetY( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_STATIC_TEXT:
				{
					wxString name = event.GetString();
					guiStatic->SetText( name.c_str() );					
				}
				break;
			case TEXT_STATIC_FONT_SIZE:
				{
					wxString valueString = event.GetString();
					double valueDouble = 0;
					valueString.ToDouble( &valueDouble );					
					guiStatic->SetFontSize( static_cast<float>( valueDouble ) );
				}
				break;
			default:
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnKillFocusStatic( wxFocusEvent& event  )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );

		switch ( event.GetId() )
		{
		case TEXT_STATIC_TEXT:
			{
				CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
				command->SaveState( guiStatic );
				mCommandList.push( command );

				wxString name = mTextStaticText->GetValue();
				guiStatic->SetText( name.c_str() );					
			}
			break;
		default:
			break;
		}

		if ( guiStatic )
		{
			UpdatePropertiesStatic( guiStatic );			
		}
	}
}

//===========================================================================

void GuiEditor::OnScrollStatic( wxScrollEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );

		if ( guiStatic )
		{
			if ( !mIsScrollingSlider )
			{
				CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
				command->SaveState( guiStatic );
				mCommandList.push( command );

				mIsScrollingSlider = true;
			}

			switch ( event.GetId() )
			{
			case SLIDER_STATIC_ALPHA:
				{
					int value = event.GetPosition();

					Math::Vector4 color = guiStatic->GetColor();
					color[3] = static_cast<float>( value ) / 255.0f;
					guiStatic->SetColor( color );
					guiStatic->SetAlpha( color[3] );
				}
				break;
			case SLIDER_STATIC_ALPHA_FONT:
				{
					int value = event.GetPosition();

					Math::Vector4 color = guiStatic->GetFontColor();
					color[3] = static_cast<float>( value ) / 255.0f;
					guiStatic->SetFontColor( color );					
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnScrollChangedStatic( wxScrollEvent& event )
{
	UNUSED_ALWAYS( event );

	mIsScrollingSlider = false;
}

//===========================================================================

void GuiEditor::OnChoiceStatic( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );

		if ( guiStatic )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiStatic );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case CHOICE_STATIC_FONT_HORIZONTAL:
				{
					int value = event.GetInt();
					TextHorizontalAlignment alignment = static_cast<TextHorizontalAlignment>( value );
					guiStatic->SetTextHorizontalAlignment( alignment );
				}
				break;
			case CHOICE_STATIC_FONT_VERTICAL:
				{
					int value = event.GetInt();
					TextVerticalAlignment alignment = static_cast<TextVerticalAlignment>( value );
					guiStatic->SetTextVerticalAlignment( alignment );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnColorChangedStatic( wxColourPickerEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );

		if ( guiStatic )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiStatic );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case COLOR_PICKER_STATIC:
				{
					wxColour wxcolor = event.GetColour();
					Math::Vector4 color = guiStatic->GetColor();
					color[0] = static_cast<float>( wxcolor.Red() ) / 255.0f;
					color[1] = static_cast<float>( wxcolor.Green() ) / 255.0f;
					color[2] = static_cast<float>( wxcolor.Blue() ) / 255.0f;
					guiStatic->SetColor( color );
				}
				break;
			case COLOR_PICKER_STATIC_FONT:
				{
					wxColour wxcolor = event.GetColour();
					Math::Vector4 color = guiStatic->GetFontColor();
					color[0] = static_cast<float>( wxcolor.Red() ) / 255.0f;
					color[1] = static_cast<float>( wxcolor.Green() ) / 255.0f;
					color[2] = static_cast<float>( wxcolor.Blue() ) / 255.0f;
					guiStatic->SetFontColor( color );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnButtonClickStatic( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );

		if ( guiStatic )
		{
			switch ( event.GetId() )
			{
			case BUTTON_STATIC_CREATE_UV:
				{
					wxString path = mFilePickStaticTexture->GetPath();

					wxString texturePath = Database::Get()->GetResourcePath();
					texturePath += path;
					texturePath += ".png";

					TextureUVDialog dialog( this );
					dialog.SetCurrentUVSize( guiStatic->GetUVSize() );
					dialog.SetCurrentUVPosition( guiStatic->GetUVPosition() );
					dialog.SetTextureImage( texturePath );
					if ( dialog.ShowModal() == wxOK )
					{
						CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
						command->SaveState( guiStatic );
						mCommandList.push( command );

						guiStatic->SetUVSize( dialog.GetUVSize() );
						guiStatic->SetUVPosition( dialog.GetUVPosition() );		
					}
				}
				break;
			case BUTTON_STATIC_CLEAR_TEXTURE:
				{
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiStatic );
					mCommandList.push( command );

					mFilePickStaticTexture->SetPath( "" );
					guiStatic->SetTexture( NULL );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnFileChangedStatic( wxFileDirPickerEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );

		if ( guiStatic )
		{
			switch ( event.GetId() )
			{
			case FILE_PICK_STATIC_TEXTURE:
				{
					Texture* filePickTexture = GetFilePickTexture( event, guiStatic );
					if ( filePickTexture )
					{
						guiStatic->SetTexture( filePickTexture );
					}	

					UpdatePropertiesStatic( guiStatic );
				}
				break;
			case FILE_PICK_STATIC_FONT:
				{			
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiStatic );
					mCommandList.push( command );

					wxString path = mFilePickStaticFont->GetPath();

					const char* resourcePath = Database::Get()->GetResourcePath();

					size_t index = path.find( wxString( resourcePath ) );
					wxString assetPathFilename = path.SubString( index + strlen( resourcePath ), path.Length() );

					wxString filename = ::wxFileNameFromPath( path );

					char filenameNoExt[128];
					System::Path::GetFilenameNoExt( filename.c_str(), filenameNoExt, 128 );

					wxString fontFilename = ::wxPathOnly( assetPathFilename );
					fontFilename += "\\";
					fontFilename += filenameNoExt;

					int fontIndex = GameApp::Get()->GetFontManager()->AddFont( filenameNoExt );
					guiStatic->SetFontIndex( fontIndex );

					UpdatePropertiesStatic( guiStatic );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::UpdatePropertiesStatic( GuiStatic* guiStatic )
{
	if ( guiStatic != NULL )
	{
		mTextStaticName->SetValue( guiStatic->GetName() );

		wxString valueString;

		valueString = wxString::Format( wxT("%i"), guiStatic->GetWidth() );
		mTextStaticWidth->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiStatic->GetHeight() );
		mTextStaticHeight->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiStatic->GetX() );	
		mTextStaticX->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiStatic->GetY() );	
		mTextStaticY->SetValue( valueString );

		mTextStaticText->SetValue( guiStatic->GetText() );

		Math::Vector4 color = guiStatic->GetColor();
		wxColour wxcolor(
			static_cast<unsigned int>( color[0] * 255.0f ),
			static_cast<unsigned int>( color[1] * 255.0f ),
			static_cast<unsigned int>( color[2] * 255.0f )
			);
		mColorPickerStatic->SetColour( wxcolor );

		int alpha = static_cast<int>( color[3] * 255.0f );
		mSliderStaticAlpha->SetValue( alpha );

		Texture* texture = guiStatic->GetTexture();
		wxString textureName = "";
		if ( texture )
		{
			textureName = texture->GetName();			
		}
		mFilePickStaticTexture->SetPath( textureName );

		int fontIndex = guiStatic->GetFontIndex();
		Font* font = GameApp::Get()->GetFontManager()->GetFont( fontIndex );
		wxString fontPath = font->GetFontName();		
		mFilePickStaticFont->SetPath( fontPath );

		Math::Vector4 fontColor = guiStatic->GetFontColor();
		wxColour wxFontColor(
			static_cast<unsigned int>( fontColor[0] * 255.0f ),
			static_cast<unsigned int>( fontColor[1] * 255.0f ),
			static_cast<unsigned int>( fontColor[2] * 255.0f )
			);
		mColorPickerStaticFont->SetColour( wxFontColor );
		int alphaFont = static_cast<int>( fontColor[3] * 255.0f );
		mSliderStaticAlphaFont->SetValue( alphaFont );

		valueString = wxString::Format( wxT("%.3f"), guiStatic->GetFontSize() );	
		mTextStaticFontSize->SetValue( valueString );

		TextHorizontalAlignment horizontalAlignment = guiStatic->GetTextHorizontalAlignment();
		mChoiceStaticFontHorizontal->SetSelection( horizontalAlignment );

		TextVerticalAlignment verticalAlignment = guiStatic->GetTextVerticalAlignment();
		mChoiceStaticFontVertical->SetSelection( verticalAlignment );
	}
}

//===========================================================================

void GuiEditor::OnTextEnterButton( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );

		if ( guiButton )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiButton );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case TEXT_BUTTON_NAME:
				{
					wxString name = event.GetString();
					guiButton->SetName( name.c_str() );
					wxString graphName = name;
					graphName += " (Button)";
					mSceneGraphTree->SetItemText( selection, graphName );
				}
				break;
			case TEXT_BUTTON_WIDTH:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiButton->SetWidth( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_BUTTON_HEIGHT:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiButton->SetHeight( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_BUTTON_X:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiButton->SetX( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_BUTTON_Y:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiButton->SetY( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_BUTTON_TEXT:
				{
					wxString name = event.GetString();
					guiButton->SetText( name.c_str() );					
				}
				break;
			case TEXT_BUTTON_FONT_SIZE:
				{
					wxString valueString = event.GetString();
					double valueDouble = 0;
					valueString.ToDouble( &valueDouble );					
					guiButton->SetFontSize( static_cast<float>( valueDouble ) );
				}
				break;
			default:
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnKillFocusButton()
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );

		if ( guiButton )
		{
			UpdatePropertiesButton( guiButton );			
		}
	}
}

//===========================================================================

void GuiEditor::OnScrollButton( wxScrollEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );

		if ( guiButton )
		{
			if ( !mIsScrollingSlider )
			{
				CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
				command->SaveState( guiButton );
				mCommandList.push( command );

				mIsScrollingSlider = true;
			}

			switch ( event.GetId() )
			{
			case SLIDER_BUTTON_ALPHA:
				{
					int value = event.GetPosition();

					Math::Vector4 color = guiButton->GetColor();
					color[3] = static_cast<float>( value ) / 255.0f;
					guiButton->SetColor( color );
					guiButton->SetAlpha( color[3] );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnScrollChangedButton( wxScrollEvent& event )
{
	UNUSED_ALWAYS( event );

	mIsScrollingSlider = false;
}

//===========================================================================

void GuiEditor::OnChoiceButton( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );

		if ( guiButton )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiButton );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case CHOICE_BUTTON_FONT_HORIZONTAL:
				{
					int value = event.GetInt();
					TextHorizontalAlignment alignment = static_cast<TextHorizontalAlignment>( value );
					guiButton->SetTextHorizontalAlignment( alignment );
				}
				break;
			case CHOICE_BUTTON_FONT_VERTICAL:
				{
					int value = event.GetInt();
					TextVerticalAlignment alignment = static_cast<TextVerticalAlignment>( value );
					guiButton->SetTextVerticalAlignment( alignment );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnColorChangedButton( wxColourPickerEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );

		if ( guiButton )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiButton );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case COLOR_PICKER_BUTTON:
				{
					wxColour wxcolor = event.GetColour();
					Math::Vector4 color = guiButton->GetColor();
					color[0] = static_cast<float>( wxcolor.Red() ) / 255.0f;
					color[1] = static_cast<float>( wxcolor.Green() ) / 255.0f;
					color[2] = static_cast<float>( wxcolor.Blue() ) / 255.0f;
					guiButton->SetColor( color );
				}
				break;
			case COLOR_PICKER_BUTTON_FONT:
				{
					wxColour wxcolor = event.GetColour();
					Math::Vector4 color = guiButton->GetFontColor();
					color[0] = static_cast<float>( wxcolor.Red() ) / 255.0f;
					color[1] = static_cast<float>( wxcolor.Green() ) / 255.0f;
					color[2] = static_cast<float>( wxcolor.Blue() ) / 255.0f;
					guiButton->SetFontColor( color );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnButtonClickButton( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );

		if ( guiButton )
		{
			switch ( event.GetId() )
			{
			case BUTTON_BUTTON_CREATE_UV:
				{					
					wxString path = mFilePickButtonTexture->GetPath();

					wxString texturePath = Database::Get()->GetResourcePath();
					texturePath += path;
					texturePath += ".png";

					TextureUVDialog dialog( this );
					dialog.SetCurrentUVSize( guiButton->GetUVSize() );
					dialog.SetCurrentUVPosition( guiButton->GetUVPosition() );
					dialog.SetTextureImage( texturePath );
					if ( dialog.ShowModal() == wxOK )
					{
						CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
						command->SaveState( guiButton );
						mCommandList.push( command );

						guiButton->SetUVSize( dialog.GetUVSize() );
						guiButton->SetUVPosition( dialog.GetUVPosition() );		
					}
				}
				break;
			case BUTTON_BUTTON_ONPRESS_CREATE_UV:
				{					
					wxString path = mFilePickButtonOnPressTexture->GetPath();

					wxString texturePath = Database::Get()->GetResourcePath();
					texturePath += path;
					texturePath += ".png";

					TextureUVDialog dialog( this );
					dialog.SetCurrentUVSize( guiButton->GetOnPressUVSize() );
					dialog.SetCurrentUVPosition( guiButton->GetOnPressUVPosition() );
					dialog.SetTextureImage( texturePath );
					if ( dialog.ShowModal() == wxOK )
					{
						CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
						command->SaveState( guiButton );
						mCommandList.push( command );

						guiButton->SetOnPressUVSize( dialog.GetUVSize() );
						guiButton->SetOnPressUVPosition( dialog.GetUVPosition() );
					}
				}
				break;
			case BUTTON_BUTTON_CLEAR_TEXTURE:
				{
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiButton );
					mCommandList.push( command );

					mFilePickButtonTexture->SetPath( "" );
					guiButton->SetTexture( NULL );
				}
				break;
			case BUTTON_BUTTON_ONPRESS_CLEAR_TEXTURE:
				{
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiButton );
					mCommandList.push( command );

					mFilePickButtonOnPressTexture->SetPath( "" );
					guiButton->SetOnPressTexture( NULL );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnFileChangedButton( wxFileDirPickerEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );

		if ( guiButton )
		{
			switch ( event.GetId() )
			{
			case FILE_PICK_BUTTON_TEXTURE:
				{
					Texture* filePickTexture = GetFilePickTexture( event, guiButton );
					if ( filePickTexture )
					{
						guiButton->SetTexture( filePickTexture );
					}

					UpdatePropertiesButton( guiButton );
				}
				break;
			case FILE_PICK_BUTTON_ONPRESS_TEXTURE:
				{
					Texture* filePickTexture = GetFilePickTexture( event, guiButton );
					if ( filePickTexture )
					{
						guiButton->SetOnPressTexture( filePickTexture );
					}

					UpdatePropertiesButton( guiButton );
				}
				break;
			case FILE_PICK_BUTTON_FONT:
				{				
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiButton );
					mCommandList.push( command );

					wxString path = mFilePickButtonFont->GetPath();

					const char* resourcePath = Database::Get()->GetResourcePath();

					size_t index = path.find( wxString( resourcePath ) );
					wxString assetPathFilename = path.SubString( index + strlen( resourcePath ), path.Length() );

					wxString filename = ::wxFileNameFromPath( path );

					char filenameNoExt[128];
					System::Path::GetFilenameNoExt( filename.c_str(), filenameNoExt, 128 );

					wxString fontFilename = ::wxPathOnly( assetPathFilename );
					fontFilename += "\\";
					fontFilename += filenameNoExt;

					int fontIndex = GameApp::Get()->GetFontManager()->AddFont( filenameNoExt );
					guiButton->SetFontIndex( fontIndex );

					UpdatePropertiesButton( guiButton );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::UpdatePropertiesButton( GuiButton* guiButton )
{
	if ( guiButton != NULL )
	{
		mTextButtonName->SetValue( guiButton->GetName() );

		wxString valueString;

		valueString = wxString::Format( wxT("%i"), guiButton->GetWidth() );
		mTextButtonWidth->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiButton->GetHeight() );
		mTextButtonHeight->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiButton->GetX() );	
		mTextButtonX->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiButton->GetY() );	
		mTextButtonY->SetValue( valueString );

		mTextButtonText->SetValue( guiButton->GetText() );

		Math::Vector4 color = guiButton->GetColor();
		wxColour wxcolor(
			static_cast<unsigned int>( color[0] * 255.0f ),
			static_cast<unsigned int>( color[1] * 255.0f ),
			static_cast<unsigned int>( color[2] * 255.0f )
			);
		mColorPickerButton->SetColour( wxcolor );

		int alpha = static_cast<int>( color[3] * 255.0f );
		mSliderButtonAlpha->SetValue( alpha );

		Texture* texture = guiButton->GetTexture();
		wxString textureName = "";
		if ( texture )
		{
			textureName = texture->GetName();			
		}
		mFilePickButtonTexture->SetPath( textureName );

		Texture* onPressTexture = guiButton->GetOnPressTexture();
		wxString onPressTextureName = "";
		if ( onPressTexture )
		{
			onPressTextureName = onPressTexture->GetName();			
		}
		mFilePickButtonOnPressTexture->SetPath( onPressTextureName );

		int fontIndex = guiButton->GetFontIndex();
		Font* font = GameApp::Get()->GetFontManager()->GetFont( fontIndex );
		wxString fontPath = font->GetFontName();		
		mFilePickButtonFont->SetPath( fontPath );

		Math::Vector4 fontColor = guiButton->GetFontColor();
		wxColour wxFontColor(
			static_cast<unsigned int>( fontColor[0] * 255.0f ),
			static_cast<unsigned int>( fontColor[1] * 255.0f ),
			static_cast<unsigned int>( fontColor[2] * 255.0f )
			);
		mColorPickerButtonFont->SetColour( wxFontColor );

		valueString = wxString::Format( wxT("%.3f"), guiButton->GetFontSize() );	
		mTextButtonFontSize->SetValue( valueString );

		TextHorizontalAlignment horizontalAlignment = guiButton->GetTextHorizontalAlignment();
		mChoiceButtonFontHorizontal->SetSelection( horizontalAlignment );

		TextVerticalAlignment verticalAlignment = guiButton->GetTextVerticalAlignment();
		mChoiceButtonFontVertical->SetSelection( verticalAlignment );
	}
}

//===========================================================================

void GuiEditor::OnTextEnterSlider( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSlider* guiSlider = mGuiEditorApp->GetGuiSlider( sceneGraphNode->mID );

		if ( guiSlider )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiSlider );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case TEXT_SLIDER_NAME:
				{
					wxString name = event.GetString();
					guiSlider->SetName( name.c_str() );
					wxString graphName = name;
					graphName += " (Slider)";
					mSceneGraphTree->SetItemText( selection, graphName );
				}
				break;
			case TEXT_SLIDER_WIDTH:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSlider->SetWidth( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SLIDER_HEIGHT:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSlider->SetHeight( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SLIDER_X:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSlider->SetX( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SLIDER_Y:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSlider->SetY( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SLIDER_MIN:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSlider->SetMin( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SLIDER_MAX:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSlider->SetMax( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SLIDER_DEFAULT:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSlider->SetValue( static_cast<int>( valueLong ) );
				}
				break;
			default:
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnKillFocusSlider()
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSlider* guiSlider = mGuiEditorApp->GetGuiSlider( sceneGraphNode->mID );

		if ( guiSlider )
		{
			UpdatePropertiesSlider( guiSlider );			
		}
	}
}

//===========================================================================

void GuiEditor::OnButtonClickSlider( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSlider* guiSlider = mGuiEditorApp->GetGuiSlider( sceneGraphNode->mID );

		if ( guiSlider )
		{
			switch ( event.GetId() )
			{
			case BUTTON_SLIDER_CREATE_UV_BAR:
				{			
					wxString path = mFilePickSliderTextureBar->GetPath();

					wxString texturePath = Database::Get()->GetResourcePath();
					texturePath += path;
					texturePath += ".png";

					TextureUVDialog dialog( this );
					dialog.SetCurrentUVSize( guiSlider->GetUVSizeBar() );
					dialog.SetCurrentUVPosition( guiSlider->GetUVPositionBar() );
					dialog.SetTextureImage( texturePath );
					if ( dialog.ShowModal() == wxOK )
					{
						CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
						command->SaveState( guiSlider );
						mCommandList.push( command );

						guiSlider->SetUVSizeBar( dialog.GetUVSize() );
						guiSlider->SetUVPositionBar( dialog.GetUVPosition() );	
					}
				}
				break;
			case BUTTON_SLIDER_CREATE_UV_BUTTON:
				{
					wxString path = mFilePickSliderTextureButton->GetPath();

					wxString texturePath = Database::Get()->GetResourcePath();
					texturePath += path;
					texturePath += ".png";

					TextureUVDialog dialog( this );
					dialog.SetCurrentUVSize( guiSlider->GetUVSizeButton() );
					dialog.SetCurrentUVPosition( guiSlider->GetUVSizeButton() );
					dialog.SetTextureImage( texturePath );
					if ( dialog.ShowModal() == wxOK )
					{
						CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
						command->SaveState( guiSlider );
						mCommandList.push( command );

						guiSlider->SetUVSizeButton( dialog.GetUVSize() );
						guiSlider->SetUVPositionButton( dialog.GetUVPosition() );	
					}
				}
				break;
			case BUTTON_SLIDER_CLEAR_TEXTURE_BAR:
				{
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiSlider );
					mCommandList.push( command );

					mFilePickSliderTextureBar->SetPath( "" );
					guiSlider->SetTextureBar( NULL );
				}
				break;
			case BUTTON_SLIDER_CLEAR_TEXTURE_BUTTON:
				{
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiSlider );
					mCommandList.push( command );

					mFilePickSliderTextureButton->SetPath( "" );
					guiSlider->SetTextureButton( NULL );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnFileChangedSlider( wxFileDirPickerEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSlider* guiSlider = mGuiEditorApp->GetGuiSlider( sceneGraphNode->mID );

		if ( guiSlider )
		{
			switch ( event.GetId() )
			{
			case FILE_PICK_SLIDER_TEXTURE_BAR:
				{
					Texture* filePickTexture = GetFilePickTexture( event, guiSlider );
					if ( filePickTexture )
					{
						guiSlider->SetTextureBar( filePickTexture );
					}

					UpdatePropertiesSlider( guiSlider );
				}
				break;
			case FILE_PICK_SLIDER_TEXTURE_BUTTON:
				{		
					Texture* filePickTexture = GetFilePickTexture( event, guiSlider );
					if ( filePickTexture )
					{
						guiSlider->SetTextureBar( filePickTexture );
					}

					UpdatePropertiesSlider( guiSlider );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::UpdatePropertiesSlider( GuiSlider* guiSlider )
{
	if ( guiSlider != NULL )
	{
		mTextSliderName->SetValue( guiSlider->GetName() );

		wxString valueString;

		valueString = wxString::Format( wxT("%i"), guiSlider->GetWidth() );
		mTextSliderWidth->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSlider->GetHeight() );
		mTextSliderHeight->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSlider->GetX() );	
		mTextSliderX->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSlider->GetY() );	
		mTextSliderY->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSlider->GetMin() );	
		mTextSliderMin->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSlider->GetMax() );	
		mTextSliderMax->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSlider->GetValue() );	
		mTextSliderDefault->SetValue( valueString );

		Texture* textureBar = guiSlider->GetTextureBar();
		wxString textureNameBar = "";
		if ( textureBar )
		{
			textureNameBar = textureBar->GetName();			
		}
		mFilePickSliderTextureBar->SetPath( textureNameBar );

		Texture* textureButton = guiSlider->GetTextureButton();
		wxString textureNameButton = "";
		if ( textureButton )
		{
			textureNameButton = textureButton->GetName();			
		}
		mFilePickSliderTextureButton->SetPath( textureNameButton );
	}
}

//===========================================================================

void GuiEditor::OnTextEnterSprite( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSprite* guiSprite = mGuiEditorApp->GetGuiSprite( sceneGraphNode->mID );

		if ( guiSprite )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiSprite );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case TEXT_SPRITE_NAME:
				{
					wxString name = event.GetString();
					guiSprite->SetName( name.c_str() );
					wxString graphName = name;
					graphName += " (Sprite)";
					mSceneGraphTree->SetItemText( selection, graphName );
				}
				break;
			case TEXT_SPRITE_WIDTH:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSprite->SetWidth( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SPRITE_HEIGHT:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSprite->SetHeight( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SPRITE_X:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSprite->SetX( static_cast<int>( valueLong ) );
				}
				break;
			case TEXT_SPRITE_Y:
				{
					wxString valueString = event.GetString();
					long valueLong = 0;
					valueString.ToLong( &valueLong );					
					guiSprite->SetY( static_cast<int>( valueLong ) );
				}
				break;   
			case TEXT_SPRITE_FPS:
				{
					wxString valueString = event.GetString();
					double doubeValue = 0;
					valueString.ToDouble( &doubeValue );					
					guiSprite->SetFramesPerSecond( static_cast<float>( doubeValue ) );
				}
				break;   
			default:
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnKillFocusSprite()
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSprite* guiSprite = mGuiEditorApp->GetGuiSprite( sceneGraphNode->mID );

		if ( guiSprite )
		{
			UpdatePropertiesSprite( guiSprite );			
		}
	}
}

//===========================================================================

void GuiEditor::OnScrollSprite( wxScrollEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSprite* guiSprite = mGuiEditorApp->GetGuiSprite( sceneGraphNode->mID );

		if ( guiSprite )
		{
			if ( !mIsScrollingSlider )
			{
				CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
				command->SaveState( guiSprite );
				mCommandList.push( command );

				mIsScrollingSlider = true;
			}

			switch ( event.GetId() )
			{
			case SLIDER_SPRITE_ALPHA:
				{
					int value = event.GetPosition();

					Math::Vector4 color = guiSprite->GetColor();
					color[3] = static_cast<float>( value ) / 255.0f;
					guiSprite->SetColor( color );
					guiSprite->SetAlpha( color[3] );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnScrollChangedSprite( wxScrollEvent& event )
{
	UNUSED_ALWAYS( event );

	mIsScrollingSlider = false;
}

//===========================================================================

void GuiEditor::OnColorChangedSprite( wxColourPickerEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSprite* guiSprite = mGuiEditorApp->GetGuiSprite( sceneGraphNode->mID );

		if ( guiSprite )
		{
			CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
			command->SaveState( guiSprite );
			mCommandList.push( command );

			switch ( event.GetId() )
			{
			case COLOR_PICKER_SPRITE:
				{
					wxColour wxcolor = event.GetColour();
					Math::Vector4 color = guiSprite->GetColor();
					color[0] = static_cast<float>( wxcolor.Red() ) / 255.0f;
					color[1] = static_cast<float>( wxcolor.Green() ) / 255.0f;
					color[2] = static_cast<float>( wxcolor.Blue() ) / 255.0f;
					guiSprite->SetColor( color );
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnButtonClickSprite( wxCommandEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSprite* guiSprite = mGuiEditorApp->GetGuiSprite( sceneGraphNode->mID );

		if ( guiSprite )
		{
			switch ( event.GetId() )
			{            
			case BUTTON_SPRITE_CLEAR_TEXTURE:
				{
					CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
					command->SaveState( guiSprite );
					mCommandList.push( command );

					mFilePickSpriteTextureFile->SetPath( "" );
					guiSprite->Clear();
				}
				break;
			}
		}
	}
}

//===========================================================================

void GuiEditor::OnFileChangedSprite( wxFileDirPickerEvent& event )
{
	wxTreeItemId selection = GetTreeCurrentSelection();	
	if ( selection.IsOk() )
	{	
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( selection ) );
		GuiSprite* guiSprite = mGuiEditorApp->GetGuiSprite( sceneGraphNode->mID );

		if ( guiSprite )
		{
			switch ( event.GetId() )
			{
			case FILE_PICK_SPRITE_TEXTURE_FILE:
				{
					wxString filePath = GetFilePickPath( event );
					guiSprite->LoadSprite( filePath.c_str() );
					UpdatePropertiesSprite( guiSprite );
				}
				break;
			}
		}
	}
}

//===========================================================================


void GuiEditor::UpdatePropertiesSprite( GuiSprite* guiSprite )
{
	if ( guiSprite != NULL )
	{
		mTextSpriteName->SetValue( guiSprite->GetName() );

		wxString valueString;

		valueString = wxString::Format( wxT("%i"), guiSprite->GetWidth() );
		mTextSpriteWidth->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSprite->GetHeight() );
		mTextSpriteHeight->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSprite->GetX() );	
		mTextSpriteX->SetValue( valueString );

		valueString = wxString::Format( wxT("%i"), guiSprite->GetY() );	
		mTextSpriteY->SetValue( valueString );

		valueString = wxString::Format( wxT("%f"), guiSprite->GetFramesPerSecond() );	
		mTextSpriteFPS->SetValue( valueString );

		Math::Vector4 color = guiSprite->GetColor();
		wxColour wxcolor(
			static_cast<unsigned int>( color[0] * 255.0f ),
			static_cast<unsigned int>( color[1] * 255.0f ),
			static_cast<unsigned int>( color[2] * 255.0f )
			);
		mColorPickerSprite->SetColour( wxcolor );

		Texture* textureButton = guiSprite->GetTexture();
		wxString textureNameButton = "";
		if ( textureButton )
		{
			textureNameButton = textureButton->GetName();			
		}
		mFilePickSpriteTextureFile->SetPath( textureNameButton );
	}
}

//===========================================================================

void GuiEditor::TogglePanel( PanelProperties panelType )
{
	Assert( panelType < PANEL_PROPERTIES_MAX, "" );

	wxScrolledWindow* panel = mPropertyPanels[panelType];
	panel->Show();

	for ( int i = 0; i < PANEL_PROPERTIES_MAX; ++i )
	{
		if ( panel != mPropertyPanels[i] )
		{
			mPropertyPanels[i]->Hide();
		}
	}	

	wxPoint graphicsPanelPosition = mGraphicsPanel->GetPosition();

	panel->GetParent()->Layout();

	mGraphicsPanel->Move( graphicsPanelPosition );
}

//===========================================================================

void GuiEditor::SaveData( bool saveAs )
{
	if ( saveAs )
	{
		wxFileDialog saveDialog(
			this, 
			"Choose a file to save", 
			wxEmptyString, 
			wxEmptyString, 
			"*.xml",
			wxFD_SAVE, 
			wxDefaultPosition
			);

		if ( saveDialog.ShowModal() == wxID_OK )
		{
			wxString currentDocPath = saveDialog.GetPath();
			mGuiEditorApp->Save( currentDocPath.c_str() );
			mCurrentSavePath = currentDocPath;
		}
	}
	else
	{
		mGuiEditorApp->Save( mCurrentSavePath );
	}
}

//===========================================================================

void GuiEditor::SelectItem( wxTreeItemId &item )
{
	if ( item.IsOk() )
	{
		SceneGraphNode* sceneGraphNode = reinterpret_cast<SceneGraphNode*>( mSceneGraphTree->GetItemData( item ) );

		GuiControl* guiControl = mGuiEditorApp->GetGuiControl( sceneGraphNode->mID );

		mAnimationsChoice->Clear();
		int numAnimationSequences = guiControl->GetNumAnimationSequences();
		for ( int i = 0; i < numAnimationSequences; ++i )
		{
			AnimationSequence* currentAnimationSequence = guiControl->GetAnimationSquence( i );            
			mAnimationsChoice->Append( currentAnimationSequence->mAnimationSequenceName );
		}

		switch ( sceneGraphNode->mNodeType )
		{
		case CONTROL_TYPE_STATIC:
			{
				GuiStatic* guiStatic = mGuiEditorApp->GetGuiStatic( sceneGraphNode->mID );
				TogglePanels( guiStatic );
				UpdateProperties( guiStatic );
				mGuiEditorApp->SetSelected( guiStatic );
				UpdateAnimationPanel( guiStatic );
			}
			break;
		case CONTROL_TYPE_BUTTON:
			{
				GuiButton* guiButton = mGuiEditorApp->GetGuiButton( sceneGraphNode->mID );
				TogglePanels( guiButton );
				UpdateProperties( guiButton );
				mGuiEditorApp->SetSelected( guiButton );
				UpdateAnimationPanel( guiButton );
			}
			break;
		case CONTROL_TYPE_SLIDER:
			{
				GuiSlider* guiSlider = mGuiEditorApp->GetGuiSlider( sceneGraphNode->mID );
				TogglePanels( guiSlider );
				UpdateProperties( guiSlider );
				mGuiEditorApp->SetSelected( guiSlider );
				UpdateAnimationPanel( guiSlider );
			}
			break;
		case CONTROL_TYPE_SPRITE:
			{
				GuiSprite* guiSprite = mGuiEditorApp->GetGuiSprite( sceneGraphNode->mID );
				TogglePanels( guiSprite );
				UpdateProperties( guiSprite );
				mGuiEditorApp->SetSelected( guiSprite );
				UpdateAnimationPanel( guiSprite );
			}
			break;
		}				
	}
}

//===========================================================================

Texture* GuiEditor::GetFilePickTexture( wxFileDirPickerEvent& event, GuiControl* guiControl )
{
	wxString path = event.GetPath();
	wxString currentDir = ::wxGetCwd();					
	currentDir.Replace( "deploy\\pc", "assets" );

	wxString assetPathFilename = path;					
	int index = path.Find( currentDir );
	assetPathFilename.Remove( index, currentDir.Length() + 1 );

	wxString ddsFilename = ::wxGetCwd();
	ddsFilename += "\\";
	ddsFilename += assetPathFilename;
	ddsFilename.Replace( "png", "dds" );

	wxString textureFilename = assetPathFilename;
	textureFilename.Replace( ".png", "" );

	Texture* pickTexture = NULL;

	if ( ::wxFileExists( ddsFilename ) )
	{			
		CommandSaveState* command = NEW_PTR( "Command Move" ) CommandSaveState( mGuiEditorApp );
		command->SaveState( guiControl );
		mCommandList.push( command );

		pickTexture = GameApp::Get()->GetTextureManager()->GetTexture( textureFilename.c_str() );        
	}
	else
	{
		wxMessageDialog* warningDialog = NEW_PTR( "Message Dialog") wxMessageDialog(
			NULL,
			wxT("Warning: Cannot find corresponding DDS file. Please build PNG texture."), 
			wxT( "Warning"), 
			wxOK | wxICON_ERROR
			);
		warningDialog->ShowModal();

		DELETE_PTR( warningDialog );
	}			

	return pickTexture;
}

//===========================================================================

wxString GuiEditor::GetFilePickPath( wxFileDirPickerEvent& event )
{
	wxString path = event.GetPath();
	wxString currentDir = ::wxGetCwd();					
	currentDir.Replace( "deploy\\pc", "assets" );

	wxString assetPathFilename = path;					
	int index = path.Find( currentDir );
	assetPathFilename.Remove( index, currentDir.Length() + 1 );

	wxString ddsFilename = ::wxGetCwd();
	ddsFilename += "\\";
	ddsFilename += assetPathFilename;
	ddsFilename.Replace( "png", "dds" );

	wxString textureFilename = assetPathFilename;
	textureFilename.Replace( ".png", "" );

	if ( ::wxFileExists( ddsFilename ) == false )
	{
		wxMessageDialog* warningDialog = NEW_PTR( "Message Dialog") wxMessageDialog(
			NULL,
			wxT("Warning: Cannot find corresponding DDS file. Please build PNG texture."), 
			wxT( "Warning"), 
			wxOK | wxICON_ERROR
			);
		warningDialog->ShowModal();

		DELETE_PTR( warningDialog );
	}			

	return textureFilename;
}

//===========================================================================

wxTreeItemId GuiEditor::GetTreeCurrentSelection()
{
	wxArrayTreeItemIds selection;
	unsigned int numSelections = mSceneGraphTree->GetSelections( selection );
	if (  numSelections > 0 )
	{
		return selection[0];
	}

	return wxTreeItemId();
}