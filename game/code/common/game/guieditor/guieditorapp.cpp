//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/render/texture.hpp"
#include "common/game/guieditor/guieditorapp.hpp"
#include <algorithm>
#include <stdlib.h>
#include <vector>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

GuiEditorApp::GuiEditorApp()
{
	mViewCamera = NULL;
	mOnDown = false;
	mRenderObjectCamera = NULL;
	mBoundingBoxColor.Set( 1, 0, 0, 1 );
	mBoundingBoxColorSelect.Set( 0, 1, 0, 1 );
	mGuiControlSelected = NULL;
	mZoomScale = 1.0f;
}

//===========================================================================

GuiEditorApp::~GuiEditorApp()
{	
	DELETE_PTR( mRenderObjectCamera );
	DELETE_PTR( mViewCamera );	
	DELETE_PTR( mGuiScreen );
}

//===========================================================================

void GuiEditorApp::Initialize()
{
	mGuiScreen = NEW_PTR( "Gui Screen" ) GuiScreen( 0 );    
	mViewCamera = NEW_PTR( "Camera" ) Camera;	
	mRenderObjectCamera = NEW_PTR( "RenderObject" ) RenderObject;
}

//===========================================================================

void GuiEditorApp::Update( float elapsedTime )
{	
	float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );
	float cameraHeight = screenHeightSize/2.0f;

	Math::Matrix44 transform = Camera::CreateTransform(
		Math::Vector3( 0, cameraHeight, 10 ), 
		Math::Vector3( 0, 1, 0 ), 
		Math::Vector3( 0, cameraHeight, 0 ) 
		);
	mRenderObjectCamera->SetTransform( transform );

	mGuiScreen->Update( elapsedTime );
}

//===========================================================================

void GuiEditorApp::RenderUpdate()
{
	Math::Vector4 backgroundColor = mGuiScreen->GetBackgroundColor();
	Renderer::Get()->SetRenderTarget( NULL, NULL );
	Renderer::Get()->SetBackgroundColor( backgroundColor[0], backgroundColor[1], backgroundColor[2] );
	Renderer::Get()->ClearRenderer( NULL );
	GameApp::Get()->GetRenderQueue()->ClearStates();

	float screenWidthSize = static_cast<float>( Database::Get()->GetAppScreenWidth() );
	float screenHeightSize = static_cast<float>( Database::Get()->GetAppScreenHeight() );

	Math::Matrix44 orthoMatrix = Renderer::Get()->GetOrthoProjection( 
		0,
		0,
		screenWidthSize,
		screenHeightSize,
		-100,
		100
		);

	mGuiScreen->Render( orthoMatrix, true );	
}

//===========================================================================

void GuiEditorApp::CopyGuiControl( GuiControl* guiControl )
{
	mGuiScreen->CopyControl( guiControl );
}

//===========================================================================

GuiStatic* GuiEditorApp::AddGuiStatic()
{
	GuiStatic* guiStatic = NULL;
	mGuiScreen->AddStatic( "Label", 50, 50, 200, 50, &guiStatic );	

	return guiStatic;
}

//===========================================================================

GuiButton* GuiEditorApp::AddGuiButton()
{	
	GuiButton* guiButton = NULL;
	mGuiScreen->AddButton( "Label", 50, 50, 100, 100, &guiButton );

	return guiButton;
}

//===========================================================================

GuiSlider* GuiEditorApp::AddGuiSlider()
{
	GuiSlider* guiSlider = NULL;
	mGuiScreen->AddSlider( 50, 50, 100, 25, 0, 100, 50, &guiSlider );	

	return guiSlider;
}

//===========================================================================

GuiSprite* GuiEditorApp::AddGuiSprite()
{
	GuiSprite* guiSprite = NULL;
	mGuiScreen->AddSprite( 25, 0, 100, 50, &guiSprite );	

	return guiSprite;
}

//===========================================================================

GuiStatic* GuiEditorApp::GetGuiStatic( int id )
{
	GuiStatic* guiStatic = mGuiScreen->GetStatic( id );
	return guiStatic;
}

//===========================================================================

GuiButton* GuiEditorApp::GetGuiButton( int id )
{
	GuiButton* guiButton = mGuiScreen->GetButton( id );
	return guiButton;
}

//===========================================================================

GuiSlider* GuiEditorApp::GetGuiSlider( int id )
{
	GuiSlider* guiSlider = mGuiScreen->GetSlider( id );
	return guiSlider;
}

//===========================================================================

GuiSprite* GuiEditorApp::GetGuiSprite( int id )
{
	GuiSprite* guiSprite = mGuiScreen->GetSprite( id );
	return guiSprite;
}

//===========================================================================

GuiControl* GuiEditorApp::GetGuiControl( int id )
{
	GuiControl* guiControl = mGuiScreen->GetControlByID( id );
	return guiControl;
}

//===========================================================================

int GuiEditorApp::GetNumControls()
{
	int numControls = mGuiScreen->GetNumControls();
	return numControls;
}

//===========================================================================

GuiControl* GuiEditorApp::GetGuiControlByIndex( int index )
{
	return mGuiScreen->GetControlByIndex( index );
}

//===========================================================================

void GuiEditorApp::MoveControlBack( int id )
{
	mGuiScreen->MoveControlBack( id );
}

//===========================================================================

void GuiEditorApp::MoveControlFront( int id )
{
	mGuiScreen->MoveControlFront( id );
}

//===========================================================================

void GuiEditorApp::DeleteGuiControl( int id )
{	
	mGuiScreen->DeleteControl( id );
}

//===========================================================================

void GuiEditorApp::SetSelected( GuiControl* selectedItem )
{
	mGuiControlSelected = selectedItem;
}

//===========================================================================

void GuiEditorApp::OnLeftDown( int x, int y )
{
	int screenHeightSize = static_cast<int>( static_cast<float>( Database::Get()->GetBackBufferHeight() ) / mZoomScale );

	int adjustedX = static_cast<int>( static_cast<float>( x ) / mZoomScale );
	int adjustedY = static_cast<int>( static_cast<float>( y ) / mZoomScale );

	int motionX = adjustedX;
	int motionY = screenHeightSize - adjustedY;

	mTransformMode = TRANSFORM_SELECT_NONE;

	if ( mGuiControlSelected )
	{
		float positionX = static_cast<float>( mGuiControlSelected->GetX() );
		float positionY = static_cast<float>( mGuiControlSelected->GetY() );
		float width = static_cast<float>( mGuiControlSelected->GetWidth() );
		float height = static_cast<float>( mGuiControlSelected->GetHeight() );

		Math::Vector2 boundingBoxVertices[4];		
		boundingBoxVertices[0].Set( positionX, positionY );
		boundingBoxVertices[1].Set( positionX + width, positionY );
		boundingBoxVertices[2].Set( positionX + width, positionY + height );
		boundingBoxVertices[3].Set( positionX, positionY + height );

		for ( int i = 0; i < 4; ++i )
		{
			Math::Vector2 mousePosition( static_cast<float>( motionX ), static_cast<float>( motionY ) );
			Math::Vector2 v = mousePosition - boundingBoxVertices[i];
			float vLength = Math::Vector2::Magnitude( v );

			if ( vLength < 10.0f )
			{
				mTransformMode = static_cast<TransformMode>( static_cast<int>( TRANSFORM_SELECT_CORNER1 ) + i );
				break;
			}
		}

		if ( mTransformMode == TRANSFORM_SELECT_NONE )
		{
			mTransformMode = TRANSFORM_SELECT_MOVE;
			mMotionData.Set( mGuiControlSelected->GetX(), mGuiControlSelected->GetY() );
		}        
	}

	mOnDown = true;
	mOnDownPosition.Set( motionX, motionY );
}

//===========================================================================

void GuiEditorApp::OnLeftUp( int /*x*/, int /*y*/ )
{
	mOnDown = false;
}

//===========================================================================

void GuiEditorApp::OnMotion( int x, int y, TransformConstraint moveType )
{
	int screenHeightSize = static_cast<int>( static_cast<float>( Database::Get()->GetBackBufferHeight() ) / mZoomScale );

	int adjustedX = static_cast<int>( static_cast<float>( x ) / mZoomScale );
	int adjustedY = static_cast<int>( static_cast<float>( y ) / mZoomScale );

	int motionX = adjustedX;
	int motionY = screenHeightSize - adjustedY;

	Math::Vector2Base<int> motionPosition( motionX, motionY );
	Math::Vector2Base<int> deltaPosition = motionPosition - mOnDownPosition;

	if ( 
		mTransformMode == TRANSFORM_SELECT_CORNER1 || 
		mTransformMode == TRANSFORM_SELECT_CORNER2 ||
		mTransformMode == TRANSFORM_SELECT_CORNER3 ||
		mTransformMode == TRANSFORM_SELECT_CORNER4 
		)
	{
		if ( mGuiControlSelected && mOnDown )
		{
			mOnDownPosition.Set( motionX, motionY );

			int positionX = mGuiControlSelected->GetX();
			int positionY = mGuiControlSelected->GetY();
			int width = mGuiControlSelected->GetWidth();
			int height = mGuiControlSelected->GetHeight();

			Math::Vector2Base<int> boundingBoxVertices[4];		
			boundingBoxVertices[0].Set( positionX, positionY );
			boundingBoxVertices[1].Set( positionX + width, positionY );
			boundingBoxVertices[2].Set( positionX + width, positionY + height );
			boundingBoxVertices[3].Set( positionX, positionY + height );

			switch ( mTransformMode )
			{
			case TRANSFORM_SELECT_CORNER1:
				{
					boundingBoxVertices[0] += deltaPosition;
					boundingBoxVertices[1][1] += deltaPosition[1];
					boundingBoxVertices[3][0] += deltaPosition[0];
				}
				break;
			case TRANSFORM_SELECT_CORNER2:
				{	
					boundingBoxVertices[1] += deltaPosition;
					boundingBoxVertices[0][1] += deltaPosition[1];
					boundingBoxVertices[2][0] += deltaPosition[0];
				}
				break;
			case TRANSFORM_SELECT_CORNER3:
				{			
					boundingBoxVertices[2] += deltaPosition;
					boundingBoxVertices[3][1] += deltaPosition[1];
					boundingBoxVertices[1][0] += deltaPosition[0];
				}
				break;
			case TRANSFORM_SELECT_CORNER4:
				{
					boundingBoxVertices[3] += deltaPosition;
					boundingBoxVertices[2][1] += deltaPosition[1];
					boundingBoxVertices[0][0] += deltaPosition[0];
				}
				break;
			default:
				break;
			}

			Math::Vector2Base<int> position = boundingBoxVertices[0];
			Math::Vector2Base<int> size = boundingBoxVertices[2] - boundingBoxVertices[0];

			mGuiControlSelected->SetPosition( position[0], position[1] );
			mGuiControlSelected->SetSize( size[0], size[1] );
		}
	}
	else
	{
		switch ( moveType )
		{
		case MOVEMENT_X:
			deltaPosition[1] = 0;
			break;
		case MOVEMENT_Y:
			deltaPosition[0] = 0;
			break;		
		default:
			break;
		}

		if ( mGuiControlSelected && mOnDown )
		{	
			int positionX = mGuiControlSelected->GetX();
			int positionY = mGuiControlSelected->GetY();

			mMotionData += deltaPosition;
			positionX += deltaPosition[0];
			positionY += deltaPosition[1];
			mOnDownPosition.Set( motionX, motionY );

			mGuiControlSelected->SetPosition( positionX, positionY );
		}	
	}

}

//===========================================================================

void GuiEditorApp::Open( const char* filename )
{	
	Clear();
	mGuiScreen->LoadTool( filename );
}

//===========================================================================

void GuiEditorApp::Save( const char* filename )
{
	mGuiScreen->Save( filename );
}

//===========================================================================

void GuiEditorApp::Clear()
{
	mGuiControlSelected = NULL;
	DELETE_PTR( mGuiScreen );
	mGuiScreen = NEW_PTR( "Gui Screen" ) GuiScreen( 0 );
}

//===========================================================================

void GuiEditorApp::PlayAnimation()
{
	mGuiScreen->PlayAnimation();
}

//===========================================================================

void GuiEditorApp::StopAnimation()
{
	// mGuiScreen->StopAnimation();
}

//===========================================================================

void GuiEditorApp::SetBackgroundColor( Math::Vector4 color )
{
	mGuiScreen->SetBackgroundColor( color );
}

//===========================================================================

int GuiEditorApp::GetGuiWidth()
{
	if ( mGuiScreen )
	{
		return mGuiScreen->GetWidth();
	}

	return 0;
}

//===========================================================================

int GuiEditorApp::GetGuiHeight()
{
	if ( mGuiScreen )
	{
		return mGuiScreen->GetHeight();
	}

	return 0;
}