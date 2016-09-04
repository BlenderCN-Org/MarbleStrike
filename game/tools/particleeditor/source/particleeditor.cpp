//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "command.hpp"
#include "commandsavestate.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guicontrol.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/gui/guislider.hpp"
#include "common/engine/gui/guistatic.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/particlesystem.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/texturemanager.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include "oglcanvas.hpp"
#include "particleeditor.hpp"
#include "textureuvdialog.hpp"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/list.h>
#include <wx/notebook.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static GameApp::GameType gGameType = GameApp::GAME_TYPE_PARTICLEEDITOR;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ParticleEditor::ParticleEditor( const wxString& title )
	:	ParticleEditorGui( NULL )
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

    mResolutionConfig = gIPhone4Config;

	//
	// Database initialization
	//
	Database::Create();
	Database::Get()->SetResourcePath( "..\\..\\assets\\" );
	Database::Get()->SetShaderResourcePath( "" );
    Database::Get()->SetBackBufferWidth( mResolutionConfig.mFrameBufferWidth );
    Database::Get()->SetBackBufferHeight( mResolutionConfig.mFrameBufferHeight );

	//
	// Create application
	//	
	RendererType currentRenderer = RENDERER_OPENGL;
	GameApp::Create();
    GameApp::Get()->SetScreenWidth( mResolutionConfig.mGameScreenWidth );
    GameApp::Get()->SetScreenHeight( mResolutionConfig.mGameScreenHeight );
	GameApp::Get()->SetRendererType( currentRenderer );
	GameApp::Get()->SetHWND( (HWND)mGraphicsPanel->GetHandle() );
	GameApp::Get()->CreateRenderer();
	GameApp::Get()->Initialize( gGameType );	

    mGraphicsPanel->SetSize( 
        Database::Get()->GetAppScreenWidth(),
        Database::Get()->GetAppScreenHeight()
        );

	mParticleEditorApp = reinterpret_cast<ParticleEditorApp*>( GameApp::Get()->GetGame() );
	
	mCurrentSavePath = "";

	mTextMaxParticles->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextEmitTime->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextEmissionMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextEmissionMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSizeMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSizeMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextShrinkMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextShrinkMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextVelocityXMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextVelocityXMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextVelocityYMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextVelocityYMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextVelocityZMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextVelocityZMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextLifeMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextLifeMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextDecayMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextDecayMax->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextSizeMin->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextForceX->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextForceY->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextForceZ->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	
	mIsFiledOpened = false;	

	mButtonPlay->Enable( true );
	mButtonStop->Enable( false );

	UpdateProperties();
	
	Fit();
}

//===========================================================================

ParticleEditor::~ParticleEditor()
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
}

//===========================================================================

void ParticleEditor::OnQuit( wxCommandEvent &WXUNUSED(event) )
{
	Close( true );
}

//===========================================================================

void ParticleEditor::OnMenuSelectionRenderer( wxCommandEvent& event )
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
	Database::Get()->SetResourcePath( "..\\..\\assets\\" );
	Database::Get()->SetShaderResourcePath( "" );
    Database::Get()->SetBackBufferWidth( mResolutionConfig.mFrameBufferWidth );
    Database::Get()->SetBackBufferHeight(  mResolutionConfig.mFrameBufferHeight );

	GameApp::Create();
    GameApp::Get()->SetScreenWidth( mResolutionConfig.mGameScreenWidth );
    GameApp::Get()->SetScreenHeight( mResolutionConfig.mGameScreenHeight );
	GameApp::Get()->SetRendererType( rendererType );
	GameApp::Get()->SetHWND( (HWND)mGraphicsPanel->GetHandle() );
	GameApp::Get()->CreateRenderer();
	GameApp::Get()->Initialize( gGameType );

	mParticleEditorApp = reinterpret_cast<ParticleEditorApp*>( GameApp::Get()->GetGame() );
}

//===========================================================================

void ParticleEditor::OnMenuSelectionNew( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	int answer = wxMessageBox( "Start a new file and lose any unsaved data?", "Confirm", wxYES_NO | wxCANCEL, this );
	if ( answer == wxYES )
	{
		mCurrentSavePath = "";

		mParticleEditorApp->Clear();
	}
}

//===========================================================================

void ParticleEditor::OnMenuSelectionOpen( wxCommandEvent& event )
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
			mParticleEditorApp->Open( currentDocPath.c_str() );

			int screenWidth = Database::Get()->GetAppScreenWidth();
			int screenHeight = Database::Get()->GetAppScreenHeight();
			mGraphicsPanel->SetMinSize( wxSize( screenWidth, screenHeight ) );
			mGraphicsPanel->GetParent()->Layout();

			UpdateProperties();

			mParticleEditorApp->PlayAnimation();
			mButtonPlay->Enable( false );
			mButtonStop->Enable( true );

			mCurrentSavePath = openDialog.GetPath();
		}
	}
}

//===========================================================================

void ParticleEditor::OnMenuSelectionSaveAs( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	SaveData( true );
}

//===========================================================================

void ParticleEditor::OnMenuSelectionSave( wxCommandEvent& event )
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

void ParticleEditor::OnMenuSelectionPortrait( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	Database::Get()->SetAppScreenWidth( 320 );
	Database::Get()->SetAppScreenHeight( 480 );

	mGraphicsPanel->SetMinSize( wxSize( 320, 480 ) );
	mGraphicsPanel->GetParent()->Layout();
}

//===========================================================================

void ParticleEditor::OnMenuSelectionLandscape( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	Database::Get()->SetAppScreenWidth( 480 );
	Database::Get()->SetAppScreenHeight( 320 );

	mGraphicsPanel->SetMinSize( wxSize( 480, 320 ) );
	mGraphicsPanel->GetParent()->Layout();
}

//===========================================================================

void ParticleEditor::OnMenuSelectionAnimation( wxCommandEvent& event )
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
	default:
		break;
	}
}

//===========================================================================

void ParticleEditor::OnTextEnter( wxCommandEvent& event )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		switch ( event.GetId() )
		{
		case TEXT_NAME:	
			{ 
				wxString valueString = event.GetString();
				particleSystem->SetName( valueString.c_str() );
			}
			break;
		case TEXT_MAX_PARTICLES:
			{	
				wxString valueString = event.GetString();
				long value = 0;
				valueString.ToLong( &value );	
				UpdateMaxParticles( value );
			}
			break;			
		case TEXT_EMIT_TIME:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );	
				particleSystem->SetEmitTime( value );
			}
			break;
		case TEXT_EMISSION_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetEmissionRange();
				particleSystem->SetEmissionRange( value, range.GetMax() );
			}
			break;
		case TEXT_EMISSION_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetEmissionRange();
				particleSystem->SetEmissionRange( range.GetMin(), value );
			}
			break;
		case TEXT_SIZE_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetSizeRange();
				particleSystem->SetSizeRange( value, range.GetMax() );
			}
			break;
		case TEXT_SIZE_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetSizeRange();
				particleSystem->SetSizeRange( range.GetMin(), value );
			}
			break;
		case TEXT_SHRINK_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetShrinkRange();
				particleSystem->SetShrinkRange( value, range.GetMax() );
			}
			break;
		case TEXT_SHRINK_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetShrinkRange();
				particleSystem->SetShrinkRange( range.GetMin(), value );
			}
			break;
		case TEXT_VELOCITY_X_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetVelocityXRange();
				particleSystem->SetVelocityXRange( value, range.GetMax() );
			}
			break;
		case TEXT_VELOCITY_X_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetVelocityXRange();
				particleSystem->SetVelocityXRange( range.GetMin(), value );
			}
			break;
		case TEXT_VELOCITY_Y_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetVelocityYRange();
				particleSystem->SetVelocityYRange( value, range.GetMax() );
			}
			break;
		case TEXT_VELOCITY_Y_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetVelocityYRange();
				particleSystem->SetVelocityYRange( range.GetMin(), value );
			}
			break;
		case TEXT_VELOCITY_Z_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetVelocityZRange();
				particleSystem->SetVelocityZRange( value, range.GetMax() );
			}
			break;
		case TEXT_VELOCITY_Z_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetVelocityZRange();
				particleSystem->SetVelocityZRange( range.GetMin(), value );
			}
			break;
		case TEXT_LIFE_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetLifeRange();
				particleSystem->SetLifeRange( value, range.GetMax() );
			}
			break;
		case TEXT_LIFE_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetLifeRange();
				particleSystem->SetLifeRange( range.GetMin(), value );
			}
			break;
		case TEXT_DECAY_MIN:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetDecayRange();
				particleSystem->SetDecayRange( value, range.GetMax() );
			}
			break;
		case TEXT_DECAY_MAX:
			{
				wxString valueString = event.GetString();
				double value = 0;
				valueString.ToDouble( &value );
				ValueRange<float> range = particleSystem->GetDecayRange();
				particleSystem->SetDecayRange( range.GetMin(), value );
			}
			break;
		case TEXT_FORCE_NAME:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{	
						System::StringCopy( forceData->mForceName, 32, event.GetString().c_str() );					
						mListBoxForces->SetString( mCurrentForceDataIndex, event.GetString() );
					}		
				}
			}
			break;
		case TEXT_FORCE_X:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{
						wxString valueString = event.GetString();
						double value = 0;
						valueString.ToDouble( &value );
						forceData->mForce[0] = value;					
					}
				}
			}
			break;
		case TEXT_FORCE_Y:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{
						wxString valueString = event.GetString();
						double value = 0;
						valueString.ToDouble( &value );
						forceData->mForce[1] = value;					
					}
				}
			}
			break;
		case TEXT_FORCE_Z:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{
						wxString valueString = event.GetString();
						double value = 0;
						valueString.ToDouble( &value );
						forceData->mForce[2] = value;					
					}
				}
			}
			break;
		default:
			break;
		}
	}
}

//===========================================================================

void ParticleEditor::OnKillFocus( wxFocusEvent& event )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		switch ( event.GetId() )
		{
		case TEXT_NAME:	
			{ 
				mTextName->SetValue( particleSystem->GetName() );
			}
			break;
		case TEXT_MAX_PARTICLES:
			{
				wxString valueString = wxString::Format( wxT("%i"), particleSystem->GetMaxParticles() );	
				mTextMaxParticles->SetValue( valueString );								
			}
			break;			
		case TEXT_EMIT_TIME:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetEmitTime() );	
				mTextEmitTime->SetValue( valueString );
			}
			break;
		case TEXT_EMISSION_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetEmissionRange().GetMin() );	
				mTextEmissionMin->SetValue( valueString );
			}
			break;
		case TEXT_EMISSION_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetEmissionRange().GetMax() );	
				mTextEmissionMax->SetValue( valueString );
			}
			break;
		case TEXT_SIZE_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetSizeRange().GetMin() );	
				mTextSizeMin->SetValue( valueString );
			}
			break;
		case TEXT_SIZE_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetSizeRange().GetMax() );	
				mTextSizeMax->SetValue( valueString );
			}
			break;
		case TEXT_SHRINK_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetShrinkRange().GetMin() );	
				mTextShrinkMin->SetValue( valueString );
			}
			break;
		case TEXT_SHRINK_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetShrinkRange().GetMax() );	
				mTextShrinkMax->SetValue( valueString );
			}
			break;
		case TEXT_VELOCITY_X_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetVelocityXRange().GetMin() );	
				mTextVelocityXMin->SetValue( valueString );
			}
			break;
		case TEXT_VELOCITY_X_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetVelocityXRange().GetMax() );	
				mTextVelocityXMax->SetValue( valueString );
			}
			break;
		case TEXT_VELOCITY_Y_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetVelocityYRange().GetMin() );	
				mTextVelocityYMin->SetValue( valueString );
			}
			break;
		case TEXT_VELOCITY_Y_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetVelocityYRange().GetMax() );	
				mTextVelocityYMax->SetValue( valueString );
			}
			break;
		case TEXT_VELOCITY_Z_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetVelocityZRange().GetMin() );	
				mTextVelocityZMin->SetValue( valueString );
			}
			break;
		case TEXT_VELOCITY_Z_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetVelocityZRange().GetMax() );	
				mTextVelocityZMax->SetValue( valueString );
			}
			break;
		case TEXT_LIFE_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetLifeRange().GetMin() );	
				mTextLifeMin->SetValue( valueString );
			}
			break;
		case TEXT_LIFE_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetLifeRange().GetMax() );	
				mTextLifeMax->SetValue( valueString );
			}
			break;
		case TEXT_DECAY_MIN:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetDecayRange().GetMin() );	
				mTextDecayMin->SetValue( valueString );
			}
			break;
		case TEXT_DECAY_MAX:
			{
				wxString valueString = wxString::Format( wxT("%f"), particleSystem->GetDecayRange().GetMax() );	
				mTextDecayMax->SetValue( valueString );
			}
			break;
		case TEXT_FORCE_NAME:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{	
						mTextForceName->SetValue( currentName );
					}		
				}
			}
			break;
		case TEXT_FORCE_X:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{	
						wxString valueString = wxString::Format( wxT("%f"), forceData->mForce[0] );	
						mTextForceX->SetValue( valueString );
					}		
				}
			}
			break;
		case TEXT_FORCE_Y:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{	
						wxString valueString = wxString::Format( wxT("%f"), forceData->mForce[1] );	
						mTextForceX->SetValue( valueString );
					}
				}
			}
			break;
		case TEXT_FORCE_Z:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString currentName = mListBoxForces->GetString( mCurrentForceDataIndex );
					ForceData* forceData = particleSystem->GetForceData( currentName );
					if ( forceData )
					{	
						wxString valueString = wxString::Format( wxT("%f"), forceData->mForce[2] );	
						mTextForceX->SetValue( valueString );
					}	
				}
			}
			break;
		default:
			break;
		}
	}

	event.Skip();
}

//===========================================================================

void ParticleEditor::OnColorChanged( wxColourPickerEvent& event )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		switch ( event.GetId() )
		{
		case COLOR_PICKER_MIN:
			{
				wxColour color = event.GetColour();
				ValueRange<Math::Vector4> colorRange = particleSystem->GetColorRange();
				Math::Vector4 minColor( color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f, 1.0f );
				particleSystem->SetColorRange( minColor, colorRange.GetMax() );
			}
			break;
		case COLOR_PICKER_MAX:
			{
				wxColour color = event.GetColour();
				ValueRange<Math::Vector4> colorRange = particleSystem->GetColorRange();
				Math::Vector4 maxColor( color.Red() / 255.0f, color.Green() / 255.0f, color.Blue() / 255.0f, 1.0f );
				particleSystem->SetColorRange( colorRange.GetMin(), maxColor );
			}
			break;
		default:
			break;
		}
	}
}

//===========================================================================

void ParticleEditor::OnButtonClick( wxCommandEvent& event )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		switch ( event.GetId() )
		{
		case BUTTON_CREATE_UV:
			{					
				wxString path = mFilePickTexture->GetPath();

				wxString texturePath = Database::Get()->GetResourcePath();
				texturePath += path;
				texturePath += ".png";

				TextureUVDialog dialog( this );
				//dialog.SetCurrentUVSize( particleSystem->GetUVSize() );
				//dialog.SetCurrentUVPosition( particleSystem->GetUVPosition() );
				dialog.SetTextureImage( texturePath );
				if ( dialog.ShowModal() == wxOK )
				{
					//particleSystem->SetUVSize( dialog.GetUVSize() );
					//particleSystem->SetUVPosition( dialog.GetUVPosition() );		
				}
			}
			break;
		case BUTTON_ADD_FORCE:
			{
				wxString forceName = mTextForceName->GetValue();
				double x = 0;
				double y = 0;
				double z = 0;

				wxString valueString;
				valueString = mTextForceX->GetValue();
				valueString.ToDouble( &x );
				valueString = mTextForceY->GetValue();
				valueString.ToDouble( &y );
				valueString = mTextForceZ->GetValue();
				valueString.ToDouble( &z );
				
				particleSystem->AddForce( forceName, x, y, z );
				UpdateProperties();
			}
			break;
		case BUTTON_DELETE_FORCE:
			{
				if ( mListBoxForces->GetCount() > 0 )
				{
					wxString name = mListBoxForces->GetString( mCurrentForceDataIndex );
					particleSystem->DeleteForce( name );
					UpdateProperties();
				}
			}
			break;
		}
	}
}

//===========================================================================

void ParticleEditor::OnFileChanged( wxFileDirPickerEvent& event )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		switch ( event.GetId() )
		{
		case FILE_PICK_TEXTURE:
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

				if ( ::wxFileExists( ddsFilename ) )
				{			
					Texture* texture = NULL;
					texture = GameApp::Get()->GetTextureManager()->GetTexture( textureFilename.c_str() );
					particleSystem->SetTexture( texture );
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
			}
			break;
		}
	}
}

//===========================================================================

void ParticleEditor::OnListBox( wxCommandEvent& event )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		switch ( event.GetId() )
		{
		case LIST_BOX_FORCES:
			{
				ForceData* forceData = particleSystem->GetForceData( event.GetString() );
				if ( forceData )
				{
					mCurrentForceDataIndex = event.GetSelection();

					mTextForceName->SetValue( forceData->mForceName );

					wxString stringValue;
					stringValue = wxString::Format( wxT("%f"), forceData->mForce[0] );
					mTextForceX->SetValue( stringValue );
					stringValue = wxString::Format( wxT("%f"), forceData->mForce[1] );
					mTextForceY->SetValue( stringValue );
					stringValue = wxString::Format( wxT("%f"), forceData->mForce[2] );
					mTextForceZ->SetValue( stringValue );
				}
			}
			break;
		}
	}
}

//===========================================================================

void ParticleEditor::OnSliderScroll( wxScrollEvent& event )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		switch ( event.GetId() )
		{
		case SLIDER_MAX_PARTICLES:
			{	
				int value = mSliderMaxParticles->GetValue();
				UpdateMaxParticles( value );
			}
			break;
		case SLIDER_ALPHA_MIN:
			{
				int value = mSliderAlphaMin->GetValue();
				float valuef = static_cast<float>( value ) / 255.0f;
				particleSystem->SetAlphaRange( valuef, particleSystem->GetAlphaRange().GetMax() );
			}
			break;
		case SLIDER_ALPHA_MAX:
			{
				int value = mSliderAlphaMax->GetValue();
				float valuef = static_cast<float>( value ) / 255.0f;
				particleSystem->SetAlphaRange(  particleSystem->GetAlphaRange().GetMin(), valuef );
			}
			break;
		}
	}
}

//===========================================================================

void ParticleEditor::OnKeyDownSceneGraph( wxKeyEvent& event )
{
	UNUSED_ALWAYS( event );
}

//===========================================================================

void ParticleEditor::OnSizeGraphicsPanel( wxSizeEvent& event )
{
	UNUSED_ALWAYS( event );

	mGraphicsPanel->SetSize( 
        Database::Get()->GetAppScreenWidth(),
        Database::Get()->GetAppScreenHeight()
		);
}

//===========================================================================

void ParticleEditor::OnLeftDownGraphicsPanel( wxMouseEvent& event )
{
	mParticleEditorApp->OnLeftDown( event.GetX(), event.GetY() );
}

//===========================================================================

void ParticleEditor::OnLeftUpGraphicsPanel( wxMouseEvent& event )
{
	mParticleEditorApp->OnLeftUp( event.GetX(), event.GetY() );	
}

//===========================================================================

void ParticleEditor::OnMotionGraphicsPanel( wxMouseEvent& event )
{
	mParticleEditorApp->OnMotion( event.GetX(), event.GetY() );

	mGraphicsPanel->SetFocus();
}

//===========================================================================

void ParticleEditor::OnKeyDownGraphicsPanel( wxKeyEvent& event )
{
	event.Skip();
}

//===========================================================================

void ParticleEditor::OnKeyUpGraphicsPanel( wxKeyEvent& event )
{
	event.Skip();
}

//===========================================================================

void ParticleEditor::OnButtonClickAnimation( wxCommandEvent& event )
{
	switch ( event.GetId() )
	{
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
	default:
		break;
	}	
}

//===========================================================================

void ParticleEditor::UpdateMaxParticles( int value )
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		particleSystem->SetMaxParticles( value );

		UpdateMaxParticlesUI( value );
	}
}

//===========================================================================

void ParticleEditor::UpdateMaxParticlesUI( int value )
{
	wxString textValue = wxString::Format( wxT( "%i" ), value );
	mTextMaxParticles->SetValue( textValue );
	mSliderMaxParticles->SetValue( value );
}

//===========================================================================

void ParticleEditor::PlayAnimation()
{
	mParticleEditorApp->PlayAnimation();
	mButtonPlay->Enable( false );
	mButtonStop->Enable( true );
}

//===========================================================================

void ParticleEditor::StopAnimation()
{
	mParticleEditorApp->StopAnimation();
	mButtonPlay->Enable( true );
	mButtonStop->Enable( false );
}

//===========================================================================

void ParticleEditor::UpdateProperties()
{
	ParticleSystem* particleSystem = mParticleEditorApp->GetParticleSystem();
	if ( particleSystem )
	{
		mTextName->SetValue( particleSystem->GetName() );
		
		wxString valueString;		

		UpdateMaxParticlesUI( particleSystem->GetMaxParticles() );

		valueString = wxString::Format( wxT("%f"), particleSystem->GetEmitTime() );	
		mTextEmitTime->SetValue( valueString );
		
		ValueRange<float> emissionRange = particleSystem->GetEmissionRange();
		valueString = wxString::Format( wxT("%f"), emissionRange.GetMin() );	
		mTextEmissionMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), emissionRange.GetMax() );	
		mTextEmissionMax->SetValue( valueString );

		ValueRange<float> sizeRange = particleSystem->GetSizeRange();
		valueString = wxString::Format( wxT("%f"), sizeRange.GetMin() );	
		mTextSizeMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), sizeRange.GetMax() );	
		mTextSizeMax->SetValue( valueString );

		ValueRange<float> shrinkRange = particleSystem->GetShrinkRange();
		valueString = wxString::Format( wxT("%f"), shrinkRange.GetMin() );	
		mTextShrinkMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), shrinkRange.GetMax() );	
		mTextShrinkMax->SetValue( valueString );

		ValueRange<float> velocityXRange = particleSystem->GetVelocityXRange();
		valueString = wxString::Format( wxT("%f"), velocityXRange.GetMin() );	
		mTextVelocityXMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), velocityXRange.GetMax() );	
		mTextVelocityXMax->SetValue( valueString );

		ValueRange<float> velocityYRange = particleSystem->GetVelocityYRange();
		valueString = wxString::Format( wxT("%f"), velocityYRange.GetMin() );	
		mTextVelocityYMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), velocityYRange.GetMax() );	
		mTextVelocityYMax->SetValue( valueString );

		ValueRange<float> velocityZRange = particleSystem->GetVelocityZRange();
		valueString = wxString::Format( wxT("%f"), velocityZRange.GetMin() );	
		mTextVelocityZMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), velocityZRange.GetMax() );	
		mTextVelocityZMax->SetValue( valueString );

		ValueRange<float> lifeRange = particleSystem->GetLifeRange();
		valueString = wxString::Format( wxT("%f"), lifeRange.GetMin() );	
		mTextLifeMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), lifeRange.GetMax() );	
		mTextLifeMax->SetValue( valueString );

		ValueRange<float> decayRange = particleSystem->GetDecayRange();
		valueString = wxString::Format( wxT("%f"), decayRange.GetMin() );	
		mTextDecayMin->SetValue( valueString );
		valueString = wxString::Format( wxT("%f"), decayRange.GetMax() );	
		mTextDecayMax->SetValue( valueString );

		ValueRange<Math::Vector4> colorRange = particleSystem->GetColorRange();
		Math::Vector4 minColor = colorRange.GetMin();
		mColorPickerButtonMin->SetColour( wxColour( minColor[0] * 255.0f, minColor[1] * 255.0f, minColor[2] * 255.0f ) );
		Math::Vector4 maxColor = colorRange.GetMax();
		mColorPickerButtonMax->SetColour( wxColour( maxColor[0] * 255.0f, maxColor[1] * 255.0f, maxColor[2] * 255.0f ) );

		ValueRange<float> alphaRange = particleSystem->GetAlphaRange();		
		mSliderAlphaMin->SetValue( alphaRange.GetMin() * 255.0f );
		mSliderAlphaMax->SetValue( alphaRange.GetMax() * 255.0f );

		Texture* texture = particleSystem->GetTexture();
		wxString textureName = "";
		if ( texture )
		{
			textureName = texture->GetName();
			mFilePickTexture->SetPath( textureName );
		}

		mListBoxForces->Clear();

		std::list<ForceData> forceDataList = particleSystem->GetForceDataList();
		std::list<ForceData>::iterator forceIter;
		int count = 0;
		for ( forceIter = forceDataList.begin(); forceIter != forceDataList.end(); ++forceIter )
		{
			ForceData forceData = *forceIter;
			mListBoxForces->Insert( forceData.mForceName, count );

			if ( count == 0 )
			{
				mListBoxForces->SetSelection( 0 );
				mTextForceName->SetValue( forceData.mForceName );
				mCurrentForceDataIndex = 0;

				wxString stringValue;
				stringValue = wxString::Format( wxT("%f"), forceData.mForce[0] );
				mTextForceX->SetValue( stringValue );
				stringValue = wxString::Format( wxT("%f"), forceData.mForce[1] );
				mTextForceY->SetValue( stringValue );
				stringValue = wxString::Format( wxT("%f"), forceData.mForce[2] );
				mTextForceZ->SetValue( stringValue );
			}

			count++;
		}
	}
}

//===========================================================================

void ParticleEditor::SaveData( bool saveAs )
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
			mParticleEditorApp->Save( currentDocPath.c_str() );
			mCurrentSavePath = currentDocPath;
		}
	}
	else
	{
		mParticleEditorApp->Save( mCurrentSavePath );
	}
}