///////////////////////////////////////////////////// 
// INCLUDES 
///////////////////////////////////////////////////// 

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guistatic.hpp" 
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/system/stringutilities.hpp"

////////////////////////////////////////////////////// 
// CLASS METHODS 
////////////////////////////////////////////////////// 

GuiStatic::GuiStatic( GuiScreen* parentScreen )
	: GuiControl( parentScreen )
{   
	mControlType = CONTROL_TYPE_STATIC;
	mTexture = NULL;
	mColor.Set( 1, 1, 1, 1 );
	mFontColor.Set( 0, 0, 0, 1 );
	mFontSize = 1.0f;
	mFontIndex = 0;

	mUVSize.Set( 100.0f, 100.0f );

	mTextHorizontalAlignment = TEXT_HORIZONTAL_ALIGN_LEFT;
	mTextVerticalAlignment = TEXT_VERTICAL_ALIGN_TOP;

	SetText( "Label" );

    mDisabledAlpha = 0.1f;
} 

//=========================================================================== 

GuiStatic::~GuiStatic() 
{ 
}

//===========================================================================

void GuiStatic::Update( float elapsedTime )
{
	GuiControl::Update( elapsedTime );

	Math::Vector4 destColor = mColor;
	Math::Vector4 destFontColor = mFontColor;
	float rate = 0.01f;
	if ( mEnabled == false )
	{	
		rate = 0.01f;
		destColor[3] = mDisabledAlpha;
		destFontColor[3] = mDisabledAlpha;
	}

	float t = powf( rate, elapsedTime * 30.0f );
	mCurrentColor = mCurrentColor + ( destColor - mCurrentColor ) * t;
	mCurrentFontColor = mCurrentFontColor + ( destFontColor - mCurrentFontColor ) * t;
}

//=========================================================================== 

void GuiStatic::Copy( GuiControl* control )
{
	if ( control )
	{
		GuiControl::Copy( control );
		GuiStatic* guiStatic = reinterpret_cast<GuiStatic*>( control );

		SetText( guiStatic->GetText() );
		mUVSize = guiStatic->mUVSize;
		mFontIndex = guiStatic->mFontIndex;
		mTexture = guiStatic->mTexture;
		mColor = guiStatic->mColor;
		mCurrentColor = guiStatic->mCurrentColor;
		mFontColor = guiStatic->mFontColor;
		mCurrentFontColor = guiStatic->mCurrentFontColor;
		mFontSize = guiStatic->mFontSize;
		mUVPosition = guiStatic->mUVPosition;
		mTextHorizontalAlignment = guiStatic->mTextHorizontalAlignment;
		mTextVerticalAlignment = guiStatic->mTextVerticalAlignment;
	}
}

//=========================================================================== 

void GuiStatic::Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox )
{
	if ( mVisible == false )
	{
		return;
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
	
	if ( mTexture )
	{
		float tx0 = mUVPosition[0] / mTexture->GetWidth();
		float ty0 = mUVPosition[1] / mTexture->GetHeight();
		float tx1 = ( mUVPosition[0] + mUVSize[0] ) / mTexture->GetWidth();
		float ty1 = ( mUVPosition[1] + mUVSize[1] ) / mTexture->GetHeight();

		texCoord[0].Set( tx0, ty0 );
		texCoord[1].Set( tx1, ty0 );
		texCoord[2].Set( tx1, ty1 );
		texCoord[3].Set( tx0, ty1 );
	}

	Math::Matrix44 transform;
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetViewMatrix( transform );
	simpleDraw->SetTransformMatrix( transform );
	simpleDraw->SetColorTexture( mTexture );	


	Math::Vector4 currentColor = mCurrentColor;
	currentColor[3] = GetAlpha();

	simpleDraw->RenderQuad( 
		v[0], 
		v[1], 
		v[2], 
		v[3], 
		texCoord[0], 
		texCoord[1], 
		texCoord[2], 
		texCoord[3], 
		currentColor
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
		simpleDraw->SetViewMatrix( transform );
		simpleDraw->SetTransformMatrix( transform );
		
		simpleDraw->RenderLineStrip( lineList, 5, Math::Vector4( 1, 0, 0, 1 ) );
	}
}

//=========================================================================== 

void GuiStatic::SetText( const char* text )
{
	System::StringCopy( mText, MAX_STATIC_TEXT, text );
}

//=========================================================================== 

const char* GuiStatic::GetText()
{
	return mText;
}