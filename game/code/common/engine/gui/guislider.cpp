///////////////////////////////////////////////////// 
// INCLUDES 
///////////////////////////////////////////////////// 

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guiinput.hpp"
#include "common/engine/gui/guiscreen.hpp"
#include "common/engine/gui/guislider.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/system/utilities.hpp"

////////////////////////////////////////////////////// 
// CLASS METHODS 
////////////////////////////////////////////////////// 

GuiSlider::GuiSlider( GuiScreen* parentScreen ) 
	:	GuiControl( parentScreen )
{   
	mControlType = CONTROL_TYPE_SLIDER;
	mValue = 0;
	mMin = 0;
	mMax = 0;
	mStartDragX = 0;
	mOffsetDrag = 0;
	mPressed = false;
	mTextureBar = NULL;
	mTextureButton = NULL;

	mUVSizeBar.Set( 100.0f, 100.0f );
	mUVSizeButton.Set( 100.0f, 100.0f );
} 

//=========================================================================== 

GuiSlider::~GuiSlider() 
{ 

} 

//===========================================================================

void GuiSlider::Copy( GuiControl* control )
{
	if ( control )
	{
		GuiControl::Copy( control );
		GuiSlider* guiSlider = reinterpret_cast<GuiSlider*>( control );

		mValue = guiSlider->mValue;
		mMin = guiSlider->mMin;
		mMax = guiSlider->mMax;
		mTextureBar = guiSlider->mTextureBar;
		mTextureButton = guiSlider->mTextureButton;
		mUVPositionBar = guiSlider->mUVPositionBar;
		mUVSizeBar = guiSlider->mUVSizeBar;
		mUVPositionButton = guiSlider->mUVPositionButton;
		mUVSizeButton = guiSlider->mUVSizeButton;
	}
}

//===========================================================================

void GuiSlider::Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox )
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
	
	float x = static_cast<float>( GetX() );
	float y = static_cast<float>( GetY() );
	float w = static_cast<float>( GetWidth() );
	float h = static_cast<float>( GetHeight() );

	Math::Vector2 texCoord[4];

	if ( mTextureBar )
	{
		float tx0 = mUVPositionBar[0] / mTextureBar->GetWidth();
		float ty0 = mUVPositionBar[1] / mTextureBar->GetHeight();
		float tx1 = ( mUVPositionBar[0] + mUVSizeBar[0] ) / mTextureBar->GetWidth();
		float ty1 = ( mUVPositionBar[1] + mUVSizeBar[1] ) / mTextureBar->GetHeight();

		texCoord[0].Set( tx0, ty0 );
		texCoord[1].Set( tx1, ty0 );
		texCoord[2].Set( tx1, ty1 );
		texCoord[3].Set( tx0, ty1 );
	}

	Math::Vector3 v0( x, y, 0 );
	Math::Vector3 v1( x + w, y, 0 );
	Math::Vector3 v2( x + w, y + h, 0 );
	Math::Vector3 v3( x, y + h, 0 );

	Math::Matrix44 transform;
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetViewMatrix( transform );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetColorTexture( mTextureBar );	

	simpleDraw->RenderQuad( 
		v0, 
		v1, 
		v2, 
		v3, 
		texCoord[0], 
		texCoord[1], 
		texCoord[2], 
		texCoord[3], 
		buttonColor
		);

	if ( mTextureButton )
	{
		float tx0 = mUVPositionButton[0] / mTextureButton->GetWidth();
		float ty0 = mUVPositionButton[1] / mTextureButton->GetHeight();
		float tx1 = ( mUVPositionButton[0] + mUVSizeButton[0] ) / mTextureButton->GetWidth();
		float ty1 = ( mUVPositionButton[1] + mUVSizeButton[1] ) / mTextureButton->GetHeight();

		texCoord[0].Set( tx0, ty0 );
		texCoord[1].Set( tx1, ty0 );
		texCoord[2].Set( tx1, ty1 );
		texCoord[3].Set( tx0, ty1 );
	}

	float buttonX = x;
	float buttonY = y;
	float buttonWidth = h;
	float buttonHeight = h;

	buttonX -= buttonWidth / 2.0f;
	buttonX += static_cast<float>( mValue - mMin ) * ( w / static_cast<float>(  mMax - mMin ) );

	Math::Vector3 v4( buttonX, buttonY, 0 );
	Math::Vector3 v5( buttonX + buttonWidth, buttonY, 0 );
	Math::Vector3 v6( buttonX + buttonWidth, buttonY + buttonHeight, 0 );
	Math::Vector3 v7( buttonX, buttonY + buttonHeight, 0 );	

	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetViewMatrix( transform );
	simpleDraw->SetTransformMatrix( transform );	
	simpleDraw->SetColorTexture( mTextureButton );	

	simpleDraw->RenderQuad( 
		v4, 
		v5, 
		v6, 
		v7, 
		texCoord[0], 
		texCoord[1], 
		texCoord[2], 
		texCoord[3], 
		Math::Vector4( 1, 0, 0, 1 )
		);

	if ( drawBoundingBox )
	{	
		Math::Vector3 lineList[5];
		lineList[0].Set( x, y, 0 );
		lineList[1].Set( x + w, y, 0 );
		lineList[2].Set( x + w, y + h, 0 );
		lineList[3].Set( x, y + h, 0 );
		lineList[4].Set( x, y, 0 );

		simpleDraw->SetProjectionMatrix( orthoMatrix );
		simpleDraw->SetViewMatrix( transform );
		simpleDraw->SetTransformMatrix( transform );
		
		simpleDraw->RenderLineStrip( lineList, 5, Math::Vector4( 1, 0, 0, 1 ) );
	}
}

//===========================================================================

bool GuiSlider::HandleMouse( GuiInput* /*input*/ )
{
	//////if ( mEnabled == false || mVisible == false )
	//////{
	//////	return false;
	//////}

	//////int mouseX = input->GetInputX(); 
	//////int mouseY = Database::Get()->GetScreenHeight() - input->GetInputY(); 

	//////if ( input->GetInput( GuiInput::INPUT_TYPE_ONDOWN ) )
	//////{
	//////	int buttonX = GetX();
	//////	int buttonY = GetY();
	//////	int buttonWidth = GetHeight();
	//////	int buttonHeight = GetHeight();

	//////	buttonX -= buttonWidth / 2;
	//////	buttonX += static_cast<int>( static_cast<float>( mValue - mMin ) * ( static_cast<float>( GetWidth() ) / static_cast<float>(  mMax - mMin ) ) );
	//////	
	//////	if (
	//////		mouseX > buttonX && 
	//////		mouseX < buttonX + buttonWidth && 
	//////		mouseY > buttonY && 
	//////		mouseY < buttonY + buttonHeight
	//////		)
	//////	{		
	//////		mPressed = true;

	//////		mStartDragX = mouseX;			
	//////		mOffsetDrag = buttonX - mStartDragX;

	//////		if ( mHasFocus == false )
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

	//////		//if ( ContainsPoint( mouseX, mouseY ) )
	//////		//{
	//////		//	m_pDialog->SendEvent( EVENT_BUTTON_CLICKED, true, this );
	//////		//}

	//////		return true;
	//////	}
	//////}

	//////if ( mPressed )
	//////{
	//////	mOffsetDrag = mouseX - mStartDragX;
	//////	mStartDragX = mouseX;

	//////	float x = static_cast<float>( GetX() );
	//////	float buttonX = x;		
	//////	float fValuePerPixel = static_cast<float>( mMax - mMin ) / static_cast<float>( GetWidth() );
	//////	buttonX += static_cast<float>( mValue - mMin ) / fValuePerPixel;
	//////	buttonX += static_cast<float>( mOffsetDrag );
	//////			
	//////	mValue = static_cast<int>( 0.5f + ( buttonX - x ) * fValuePerPixel );
	//////	mValue = Math::Max<int>( mMin, mValue );
	//////	mValue = Math::Min<int>( mMax, mValue );

	//////	//SetValueInternal( ValueFromPos( m_x + pt.x + m_nDragOffset ), true );
	//////	//m_pDialog->SendEvent( EVENT_SLIDER_VALUE_CHANGED, bFromInput, this );
	//////	return true;
	//////}

	return false;	
}

//===========================================================================

bool GuiSlider::ContainsPoint( int x, int y )
{
	int buttonX = GetX();
	int buttonY = GetY();
	int buttonWidth = GetHeight();
	int buttonHeight = GetHeight();

	buttonX -= buttonWidth / 2;
	buttonX += static_cast<int>( static_cast<float>( mValue - mMin ) * ( static_cast<float>( GetWidth() ) / static_cast<float>(  mMax - mMin ) ) );
	
	if ( 
		( x > GetX() && 
		x < GetX() + GetWidth() && 
		y > GetY() && 
		y < GetY() + GetHeight() ) ||
		( x > buttonX && 
		x < buttonX + buttonWidth && 
		y > buttonY && 
		y < buttonY + buttonHeight )
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}