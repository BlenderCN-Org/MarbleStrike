//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "textureuvdialog.hpp"
#include "common/engine//math/mathfunctions.hpp"
#include "copydatamanager.hpp"
#include <wx/file.h>
#include <wx/wx.h>
#include <wx/dcgraph.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

const float gRadiusHandle = 10.0f;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

TextureUVDialog::TextureUVDialog( wxWindow* parent )
	:	TextureUVDialogGui( parent )
{
	mTextUVWidth->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextUVHeight->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextUVX->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );
	mTextUVY->SetValidator( wxTextValidator ( wxFILTER_NUMERIC ) );

	mOnDown = false;
	mSelected = false;

	mScale = 1.0;

	mPanelPosition.Set( 0, 0 );

	mOnPanelDown = false;

	mSelectMode = SELECT_NONE;

	mScaleHandleSize = 20;
}

//===========================================================================

TextureUVDialog::~TextureUVDialog()
{

}

//===========================================================================

void TextureUVDialog::SetTextureImage( wxString path )
{
	wxImage pngImage;
	pngImage.LoadFile( path, wxBITMAP_TYPE_PNG );
	if ( pngImage.IsOk() )
	{
		int width = pngImage.GetWidth();
		int height = pngImage.GetHeight();

		mImage = wxBitmap( pngImage );
				
		width = Math::Max<int>( width, 1024 );
		height = Math::Max<int>( height, 1024 );		

		mImagePanel->SetMinSize( wxSize( 1024, 800 ) );
		Fit();	
	}	
}

//===========================================================================

Math::Vector2 TextureUVDialog::GetUVSize()
{
	double value = 0;

	mTextUVWidth->GetValue().ToDouble( &value );
	float width = static_cast<float>( value );

	mTextUVHeight->GetValue().ToDouble( &value );
	float height = static_cast<float>( value );

	Math::Vector2 size( width, height );

	return size;
}

//===========================================================================

Math::Vector2 TextureUVDialog::GetUVPosition()
{
	double value = 0;

	mTextUVX->GetValue().ToDouble( &value );
	float x = static_cast<float>( value );

	mTextUVY->GetValue().ToDouble( &value );
	float y = static_cast<float>( value );	

	Math::Vector2 position( x, y );

	return position;
}

//===========================================================================

void TextureUVDialog::OnInitDialog( wxInitDialogEvent& event )
{
	UNUSED_ALWAYS( event );

	UpdateFields();	
}

//===========================================================================

void TextureUVDialog::OnEraseBackground( wxEraseEvent& event )
{
	UNUSED_ALWAYS( event );
}

//===========================================================================

void TextureUVDialog::OnPaint( wxPaintEvent& event )
{	
	UNUSED_ALWAYS( event );

	// Create paint DC
	wxBufferedPaintDC dc(mImagePanel);
	PrepareDC( dc );

	// Create graphics context from it
	wxGraphicsContext *gc = wxGraphicsContext::Create( dc );

	if (gc)
	{
		wxBrush clearBrush( *wxLIGHT_GREY );
		gc->SetBrush( clearBrush );	
		wxSize dialogSize = this->GetSize();
		gc->DrawRectangle(
			0, 
			0, 
			dialogSize.GetWidth(),
			dialogSize.GetHeight()
			);

		gc->Scale( mScale, mScale );

		gc->SetPen( wxNullPen );

		wxBrush brushBackground( *wxLIGHT_GREY );
		gc->SetBrush( brushBackground );	

		int textureHeight = mImage.GetHeight();
		int textureWidth = mImage.GetWidth();

		gc->DrawRectangle(
			0, 
			0, 
			textureWidth,
			textureHeight
			);

		if ( mImage.Ok() )
		{	
			gc->DrawBitmap( 
				mImage, 
				mPanelPosition[0], 
				-textureHeight/mScale * ( mScale - 1.0 ) - mPanelPosition[1], 
				textureWidth, 
				textureHeight 
				);
		}

		wxPen pen( *wxRED, 1, wxSHORT_DASH );
		gc->SetPen( pen );

		wxBrush brush( wxColour( 255, 255, 255 ), wxTRANSPARENT );
		gc->SetBrush( brush );	

		gc->DrawRectangle( 
			mCurrentUVPosition[0] + mPanelPosition[0],
			( textureHeight - mCurrentUVPosition[1] - mCurrentUVSize[1] ) -textureHeight/mScale * ( mScale - 1.0 ) - mPanelPosition[1],
			mCurrentUVSize[0],
			mCurrentUVSize[1]
		);

		Math::Vector2 boundingBoxVertices[4];
		boundingBoxVertices[0].Set( mCurrentUVPosition );
		boundingBoxVertices[1].Set( mCurrentUVPosition[0] + mCurrentUVSize[0], mCurrentUVPosition[1] );
		boundingBoxVertices[2].Set( mCurrentUVPosition + mCurrentUVSize );
		boundingBoxVertices[3].Set( mCurrentUVPosition[0], mCurrentUVPosition[1] + mCurrentUVSize[1] );

		for ( int i = 0; i < 4; i++ )
		{
			float width = gRadiusHandle * 2.0f;
			float height = gRadiusHandle * 2.0f;
			gc->DrawEllipse(
				boundingBoxVertices[i][0] + mPanelPosition[0] - width / 2.0f,				
				( textureHeight - boundingBoxVertices[i][1] - height ) -textureHeight/mScale * ( mScale - 1.0 ) - mPanelPosition[1] + height/2.0f,
				width,
				height
				);
		}

		gc->SetPen( wxNullPen );	
		gc->SetBrush( wxNullBrush );


		delete gc;
	}	
}

//===========================================================================

void TextureUVDialog::OnButtonClickOK( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	EndModal( wxOK );
}

//===========================================================================

void TextureUVDialog::OnButtonClickCancel( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	EndModal( wxCANCEL );
}

//===========================================================================

void TextureUVDialog::OnButtonClickCopySize( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

    CopyDataManager::GetInstance()->SetCopySizeData( mCurrentUVSize );
}

//===========================================================================

void TextureUVDialog::OnButtonClickCopyPosition( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

    CopyDataManager::GetInstance()->SetCopySizeData( mCurrentUVPosition );
}

//===========================================================================

void TextureUVDialog::OnButtonClickPasteSize( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

    mCurrentUVSize = CopyDataManager::GetInstance()->GetCopySizeData();

    Refresh();
    UpdateFields();
}

//===========================================================================

void TextureUVDialog::OnButtonClickPastePosition( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

    mCurrentUVPosition = CopyDataManager::GetInstance()->GetCopySizeData();

    Refresh();
    UpdateFields();
}

//===========================================================================

void TextureUVDialog::OnTextEnter( wxCommandEvent& event )
{
	double value = 0;

	switch ( event.GetId() )
	{
	case UV_PROPERTIES_WIDTH:
		{
			mTextUVWidth->GetValue().ToDouble( &value );
			float floatValue = static_cast<float>( value );
			mCurrentUVSize[0] = floatValue;
		}
		break;
	case UV_PROPERTIES_HEIGHT:
		{
			mTextUVHeight->GetValue().ToDouble( &value );
			float floatValue = static_cast<float>( value );
			mCurrentUVSize[1] = floatValue;
		}
		break;
	case UV_PROPERTIES_POSITION_X:
		{
			mTextUVX->GetValue().ToDouble( &value );
			float floatValue = static_cast<float>( value );
			mCurrentUVPosition[0] = floatValue;
		}
		break;
	case UV_PROPERTIES_POSITION_Y:
		{
			mTextUVY->GetValue().ToDouble( &value );
			float floatValue = static_cast<float>( value );
			mCurrentUVPosition[1] = floatValue;
		}
		break;
	}

	Refresh();
}

//===========================================================================

void TextureUVDialog::OnLeftDownImagePanel( wxMouseEvent& event )
{
	mImagePanel->SetFocus();

	mSelected = false;

	float textureHeight = static_cast<float>( mImage.GetHeight() );

	float motionX = event.GetX() / mScale;
	float motionY = (textureHeight - event.GetY() )/ mScale;

	mSelectMode = SELECT_NONE;

	Math::Vector2 boundingBoxVertices[4];		
	Math::Vector2 pos = mCurrentUVPosition + mPanelPosition;
	boundingBoxVertices[0].Set( pos );
	boundingBoxVertices[1].Set( pos[0] + mCurrentUVSize[0], pos[1] );
	boundingBoxVertices[2].Set( pos + mCurrentUVSize );
	boundingBoxVertices[3].Set( pos[0], pos[1] + mCurrentUVSize[1] );

	for ( int i = 0; i < 4; ++i )
	{
		Math::Vector2 mousePosition( motionX, motionY );
		Math::Vector2 v = mousePosition - boundingBoxVertices[i];
		float vLength = Math::Vector2::Magnitude( v );

		if ( vLength < gRadiusHandle )
		{
			mSelectMode = static_cast<SelectMode>( static_cast<int>( SELECT_CORNER1 ) + i );
			mSelected = true;
			mOnDown = true;
			break;
		}
	}

	if ( mSelectMode == SELECT_NONE )
	{
		Math::Vector2 minSize = mCurrentUVPosition;
		minSize[0] += mPanelPosition[0];
		minSize[1] += mPanelPosition[1];		
		Math::Vector2 maxSize = minSize + mCurrentUVSize;

		if ( 
			motionX >= minSize[0] && 
			motionX <= maxSize[0] && 
			motionY >= minSize[1] && 
			motionY <= maxSize[1] 
		)
		{	
			mSelected = true;
			mOnDown = true;
			mSelectMode = SELECT_MOVE;
		}			
	}

	if ( mSelectMode == SELECT_NONE )
	{
		mOnPanelDown = true;
	}

	mOnDownPosition.Set( motionX, motionY );
}

//===========================================================================

void TextureUVDialog::OnLeftUpImagePanel( wxMouseEvent& event )
{
	UNUSED_ALWAYS( event );

	mOnDown = false;
	mOnPanelDown = false;
}

//===========================================================================

void TextureUVDialog::OnMotionImagePanel( wxMouseEvent& event )
{
	int textureHeight = static_cast<float>( mImage.GetHeight() );

	float motionX = event.GetX() / mScale;
	float motionY = (textureHeight - event.GetY() )/ mScale;

	Math::Vector2 motionPosition( motionX, motionY );
	Math::Vector2 deltaPosition = motionPosition - mOnDownPosition;

	if ( mSelected && mOnDown )
	{	
		Math::Vector2 boundingBoxVertices[4];		
		boundingBoxVertices[0].Set( mCurrentUVPosition );
		boundingBoxVertices[1].Set( mCurrentUVPosition[0] + mCurrentUVSize[0], mCurrentUVPosition[1] );
		boundingBoxVertices[2].Set( mCurrentUVPosition + mCurrentUVSize );
		boundingBoxVertices[3].Set( mCurrentUVPosition[0], mCurrentUVPosition[1] + mCurrentUVSize[1] );

		if ( mSelectMode == SELECT_MOVE )
		{
			boundingBoxVertices[0] += deltaPosition;
			boundingBoxVertices[1] += deltaPosition;
			boundingBoxVertices[2] += deltaPosition;
			boundingBoxVertices[3] += deltaPosition;			
		}
		else if ( mSelectMode == SELECT_CORNER1 )
		{			
			boundingBoxVertices[0] += deltaPosition;
			boundingBoxVertices[1][1] += deltaPosition[1];
			boundingBoxVertices[3][0] += deltaPosition[0];
		}
		else if ( mSelectMode == SELECT_CORNER2 )
		{			
			boundingBoxVertices[1] += deltaPosition;
			boundingBoxVertices[0][1] += deltaPosition[1];
			boundingBoxVertices[2][0] += deltaPosition[0];
		}
		else if ( mSelectMode == SELECT_CORNER3 )
		{			
			boundingBoxVertices[2] += deltaPosition;
			boundingBoxVertices[3][1] += deltaPosition[1];
			boundingBoxVertices[1][0] += deltaPosition[0];
		}
		else if ( mSelectMode == SELECT_CORNER4 )
		{			
			boundingBoxVertices[3] += deltaPosition;
			boundingBoxVertices[2][1] += deltaPosition[1];
			boundingBoxVertices[0][0] += deltaPosition[0];
		}
		
		mCurrentUVPosition = boundingBoxVertices[0];
		mCurrentUVSize = boundingBoxVertices[2] - boundingBoxVertices[0];

		mOnDownPosition.Set( motionX, motionY );

		UpdateFields();

		Refresh();
	}

	if ( mOnPanelDown )
	{
		mPanelPosition += deltaPosition;
		mOnDownPosition.Set( motionX, motionY );
		Refresh();
	}
}

//===========================================================================

void TextureUVDialog::UpdateFields()
{
	wxString valueString;

	valueString = wxString::Format( wxT("%f"), mCurrentUVSize[0] );
	mTextUVWidth->SetValue( valueString );

	valueString = wxString::Format( wxT("%f"), mCurrentUVSize[1] );
	mTextUVHeight->SetValue( valueString );		

	valueString = wxString::Format( wxT("%f"), mCurrentUVPosition[0] );
	mTextUVX->SetValue( valueString );

	valueString = wxString::Format( wxT("%f"), mCurrentUVPosition[1] );
	mTextUVY->SetValue( valueString );
}

//===========================================================================

void TextureUVDialog::OnKeyDownImagePanel( wxKeyEvent& event )
{
	event.Skip();
}

//===========================================================================

void TextureUVDialog::OnKeyUpImagePanel( wxKeyEvent& event )
{
	event.Skip();
}

//===========================================================================

void TextureUVDialog::OnButtonClickMagnified( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	mScale += 0.1;

	Refresh();
}

//===========================================================================

void TextureUVDialog::OnButtonClickMinified( wxCommandEvent& event )
{
	UNUSED_ALWAYS( event );

	mScale -= 0.1;
	if ( mScale < 0 )
	{
		mScale = 0.1;
	}

	Refresh();
}