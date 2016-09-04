///////////////////////////////////////////////////// 
// INCLUDES 
///////////////////////////////////////////////////// 

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guicheckbox.hpp"
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

GuiCheckBox::GuiCheckBox( GuiScreen* parentScreen )
	:	GuiButton( parentScreen )
{ 
	mChecked = false;
	mControlType = CONTROL_TYPE_CHECKBOX;
} 

//=========================================================================== 

GuiCheckBox::~GuiCheckBox() 
{ 
} 

//===========================================================================

void GuiCheckBox::Render()
{
	Math::Vector4 buttonColor( 0.3f, 0.3f, 0.3f, 1.0f );

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
		buttonColor.Set( 0.0f, 1.0f, 0.0f, 1.0f );
		state = CONTROL_STATE_PRESSED;
	}
	else if ( mMouseOver )
	{
		buttonColor.Set( 1.0f, 0.0f, 0.0f, 1.0f );
		state = CONTROL_STATE_MOUSEOVER;
	}
	else if ( mHasFocus )
	{
		state = CONTROL_STATE_FOCUS;
	}

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

	float x = static_cast<float>( GetX() );
	float y = static_cast<float>( GetY() );
	float w = static_cast<float>( GetWidth() );
	float h = static_cast<float>( GetHeight() );

	float buttonX = x;
	float buttonY = y;
	float buttonWidth = h;
	float buttonHeight = h;

	Math::Vector3 v0( buttonX, buttonY, 0 );
	Math::Vector3 v1( buttonX + buttonWidth, buttonY, 0 );
	Math::Vector3 v2( buttonX + buttonWidth, buttonY + buttonHeight, 0 );
	Math::Vector3 v3( buttonX, buttonY + buttonHeight, 0 );
	Math::Vector2 t0;
	Math::Vector2 t1;
	Math::Vector2 t2;
	Math::Vector2 t3;
	
	Math::Matrix44 transform;
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();

	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetViewMatrix( transform );
	simpleDraw->SetTransformMatrix( transform );

	simpleDraw->RenderQuad( v0, v1, v2, v3, t0, t1, t2, t3, buttonColor );
	if ( mChecked )
	{
		simpleDraw->RenderQuad( v0, v1, v2, v3, t0, t1, t2, t3, Math::Vector4( 0, 1, 1, 1 ) );
	}
	
	float textX = x + buttonWidth * 1.25f;
	float textY = y;
	float textWidth = w;
	float textHeight = h;
		
	Font* font = GameApp::Get()->GetFontManager()->GetFont( 0 );
	font->AddToBuffer( 
		textX, 
		textY, 
		textWidth, 
		textHeight, 
		TEXT_HORIZONTAL_ALIGN_LEFT, 
		TEXT_VERTICAL_ALIGN_CENTRE, 
		Math::Vector4( 1, 0, 0, 1 ), 
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
	font->RenderBuffers( orthoMatrix, transform, scissorRectangle );
}

//===========================================================================

bool GuiCheckBox::HandleMouse( GuiInput* /*input*/ )
{
	//////if ( mEnabled == false || mVisible == false )
	//////{
	//////	return false;
	//////}

	//////int mouseX = input->GetInputX(); 
	//////int mouseY = Database::Get()->GetScreenHeight() - input->GetInputY(); 

	//////if ( input->GetInput( GuiInput::INPUT_TYPE_ONDOWN ) )
	//////{
	//////	if ( ContainsPoint( mouseX, mouseY ) )
	//////	{
	//////		mPressed = true;

	//////		if ( mHasFocus == false)
	//////		{
	//////			mParentScreen->RequestFocus( this );
	//////		}

	//////		return true;
	//////	}
	//////}

	//////if ( input->GetInput( GuiInput::INPUT_TYPE_ONUP ) )
	//////{

	//////	if ( mPressed )
	//////	{
	//////		mPressed = false;

	//////		if ( ContainsPoint( mouseX, mouseY ) )
	//////		{
	//////			mChecked = !mChecked;
	//////			//m_pDialog->SendEvent( EVENT_CHECKBOX_CHANGED, bFromInput, this );
	//////		}

	//////		return true;
	//////	}
	//////}

	return false;	
}

//===========================================================================

bool GuiCheckBox::ContainsPoint( int x, int y )
{
	int buttonX = GetX();
	int buttonY = GetY();
	int buttonWidth = GetHeight();
	int buttonHeight = GetHeight();

	int textX = GetX() + static_cast<int>( static_cast<float>( buttonWidth ) * 1.25f );
	int textY = GetY();
	int textWidth = GetWidth();
	int textHeight = GetHeight();

	if ( 
		( x > buttonX && 
		x < buttonX + buttonWidth && 
		y > buttonY && 
		y < buttonY + buttonHeight ) ||
		( x > textX && 
		x < textX + textWidth && 
		y > textY && 
		y < textY + textHeight )
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}