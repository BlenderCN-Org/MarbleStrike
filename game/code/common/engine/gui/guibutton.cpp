///////////////////////////////////////////////////// 
// INCLUDES 
///////////////////////////////////////////////////// 

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guibutton.hpp"
#include "common/engine/gui/guiinput.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/simpledraw.hpp"

////////////////////////////////////////////////////// 
// CLASS METHODS 
////////////////////////////////////////////////////// 

GuiButton::GuiButton( GuiScreen* parentScreen )
	: GuiStatic( parentScreen )
{	
	mControlType = CONTROL_TYPE_BUTTON;
	mPressed = false;
    mTouchID = -1;
    mOnPressTexture = NULL;
    mOnPressUVSize.Set( 100.0f, 100.0f );
    mIsDirty = false;
}

//=========================================================================== 

GuiButton::~GuiButton() 
{
} 

//===========================================================================

void GuiButton::Copy( GuiControl* control )
{
	if ( control )
	{
		GuiStatic::Copy( control );

        GuiButton* guiButton = reinterpret_cast<GuiButton*>( control );
        mOnPressUVSize = guiButton->mOnPressUVSize;
		mOnPressTexture = guiButton->mOnPressTexture;		
		mOnPressUVPosition = guiButton->mOnPressUVPosition;
	}
}

//===========================================================================

void GuiButton::Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox )
{
	if ( mVisible == false )
		return;

	if ( mHasFocus == false )
	{
		mPressed = false;
	}

	ControlState state = CONTROL_STATE_NORMAL;

	if ( mVisible == false )
	{
		state = CONTROL_STATE_HIDDEN;
	}
	else if ( mEnabled == false )
	{
		state = CONTROL_STATE_DISABLED;
	}
	else if ( mPressed )
	{
		state = CONTROL_STATE_PRESSED;
	}
	else if ( mMouseOver )
	{		
		state = CONTROL_STATE_MOUSEOVER;
	}
	else if ( mHasFocus )
	{
		state = CONTROL_STATE_FOCUS;
	}

	float x = static_cast<float>( GetX() );
	float y = static_cast<float>( GetY() );
	float w = static_cast<float>( GetWidth() );
	float h = static_cast<float>( GetHeight() );

	Math::Vector3 v[4];
	v[0].Set( x, y, 0 );
	v[1].Set( x + w, y, 0 );
	v[2].Set( x + w, y + h, 0 );
	v[3].Set( x, y + h, 0 );

	Math::Vector2 texCoord[4];
	Math::Vector4 color = mCurrentColor;	

    Texture* currentTexture = mTexture;
    Math::Vector2 currentUVPosition = mUVPosition;
    Math::Vector2 currenetUVSize = mUVSize;

    if ( state == CONTROL_STATE_PRESSED && mOnPressTexture )
    {
        currentTexture = mOnPressTexture;
        currentUVPosition = mOnPressUVPosition;
        currenetUVSize = mOnPressUVSize;
    }

	if ( currentTexture )
	{
		float tx0 = currentUVPosition[0] / currentTexture->GetWidth();
		float ty0 = currentUVPosition[1] / currentTexture->GetHeight();
		float tx1 = ( currentUVPosition[0] + currenetUVSize[0] ) / currentTexture->GetWidth();
		float ty1 = ( currentUVPosition[1] + currenetUVSize[1] ) / currentTexture->GetHeight();

		texCoord[0].Set( tx0, ty0 );
		texCoord[1].Set( tx1, ty0 );
		texCoord[2].Set( tx1, ty1 );
		texCoord[3].Set( tx0, ty1 );
	}
	
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetColorTexture( currentTexture );		

	simpleDraw->RenderQuad( 
		v[0], 
		v[1], 
		v[2], 
		v[3], 
		texCoord[0], 
		texCoord[1], 
		texCoord[2], 
		texCoord[3], 
		color
		);

	if ( strcmp( mText, "" ) != 0 )
	{
		Font* font = GameApp::Get()->GetFontManager()->GetFont( mFontIndex );
		font->SetSize( mFontSize );
		font->AddToBuffer( 
			x, 
			y, 
			w, 
			h, 
			mTextHorizontalAlignment, 
			mTextVerticalAlignment, 
			mCurrentFontColor, 
			mText 
			);

		float xScale = Database::Get()->GetXScreenScaleFactor();
		float yScale = Database::Get()->GetYScreenScaleFactor();
		
		float xPos = static_cast<float>( GetX() ) * xScale;
		float yPos = static_cast<float>( GetY() ) * yScale;

		int xIntPos = static_cast<int>( xPos );
		int yIntPos = static_cast<int>( yPos );

		float width = static_cast<float>( GetWidth() ) * xScale;
		float height = static_cast<float>( GetHeight() ) * yScale;

		int widthInt = static_cast<int>( width );
		int heightInt = static_cast<int>( height );

		Math::Rectangle<int> scissorRectangle;
		scissorRectangle.SetPosition( xIntPos, yIntPos );
		scissorRectangle.SetWidth( widthInt );
		scissorRectangle.SetHeight( heightInt );
		Math::Matrix44 transform;
		font->RenderBuffers( orthoMatrix, transform, scissorRectangle );
	}	

	if ( drawBoundingBox )
	{	
		Math::Vector3 lineList[5];
		lineList[0].Set( x, y, 0 );
		lineList[1].Set( x + w, y, 0 );
		lineList[2].Set( x + w, y + h, 0 );
		lineList[3].Set( x, y + h, 0 );
		lineList[4].Set( x, y, 0 );

		simpleDraw->SetProjectionMatrix( orthoMatrix );
		simpleDraw->RenderLineStrip( lineList, 5, Math::Vector4( 1, 0, 0, 1 ) );
	}
}

//===========================================================================

bool GuiButton::HandleMouse( GuiInput* input )
{
	if ( mEnabled == false || mVisible == false )
    {
        return false;
    }

    std::vector<int> downResults = input->GetInput( GuiInput::INPUT_TYPE_ONDOWN );
    for ( size_t i = 0; i < downResults.size(); ++i )
    {
        int ID = downResults[i];

        int x = input->GetInputX( ID ); 
        int y = Database::Get()->GetAppScreenHeight() - input->GetInputY( ID ); 

        if ( ContainsPoint( x, y ) )
        {
            if ( mIsDirty )
            {
                mPressed = false;
            }
            else
            {
                mPressed = true;
            }
            mTouchID = ID;

            if ( mHasFocus == false)
            {
                mParentScreen->RequestFocus( this );
            }

            return true;
        }
    }


    std::vector<int> upResults = input->GetInput( GuiInput::INPUT_TYPE_ONUP );
    for ( size_t i = 0; i < upResults.size(); ++i )
    {	
        int ID = upResults[i];
        if ( ID == mTouchID )
        {
            int x = input->GetInputX( ID ); 
            int y = Database::Get()->GetAppScreenHeight() - input->GetInputY( ID ); 

            if ( mPressed )
            {
                mPressed = false;                

                if ( ContainsPoint( x, y ) )
                {
                    mParentScreen->OnEvent( EVENT_BUTTON_CLICKED, GetID(), this );
                }

                return true;
            }
        }
    }

	return false;	
}

//===========================================================================

bool GuiButton::ContainsPoint( int x, int y )
{	
	int x0 = GetX();
	int x1 = x0 + GetWidth();
	int y0 = GetY();
	int y1 = y0 + GetHeight();
	if ( x > x0 && x < x1 && y > y0 && y < y1 )
	{
		return true;
	}
	else
	{
		return false;
	}
}