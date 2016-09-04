///////////////////////////////////////////////////// 
// INCLUDES 
///////////////////////////////////////////////////// 

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/gui/guisprite.hpp" 
#include "common/engine/render/font.hpp"
#include "common/engine/render/fontmanager.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/render/spriteanimated.hpp"

////////////////////////////////////////////////////// 
// CLASS METHODS 
////////////////////////////////////////////////////// 

GuiSprite::GuiSprite( GuiScreen* parentScreen )
	: GuiControl( parentScreen )
{   
	mControlType = CONTROL_TYPE_SPRITE;
	mColor.Set( 1, 1, 1, 1 );
	mSpriteAnimated = NULL;
	mDisabledAlpha = 0.1f;
} 

//=========================================================================== 

GuiSprite::~GuiSprite() 
{ 
	DELETE_PTR( mSpriteAnimated );
}

//===========================================================================

void GuiSprite::Update( float elapsedTime )
{
	GuiControl::Update( elapsedTime );

	Math::Vector4 destColor = mColor;
	float rate = 0.01f;
	if ( mEnabled == false )
	{	
		rate = 0.01f;
		destColor[3] = mDisabledAlpha;
	}

	float t = powf( rate, elapsedTime * 30.0f );
	mCurrentColor = mCurrentColor + ( destColor - mCurrentColor ) * t;

	if ( mSpriteAnimated )
	{
		mSpriteAnimated->Update( elapsedTime );
	}
}

//=========================================================================== 

void GuiSprite::Copy( GuiControl* control )
{
	if ( control )
	{
		GuiControl::Copy( control );
		GuiSprite* guiSprite = reinterpret_cast<GuiSprite*>( control );

		mColor = guiSprite->mColor;
		mCurrentColor = guiSprite->mCurrentColor;

		Clear();

		if ( guiSprite->mSpriteAnimated )
		{
			mSpriteAnimated = NEW_PTR( "SpriteAnimated" ) SpriteAnimated;
			mSpriteAnimated->Copy( *( guiSprite->mSpriteAnimated ) );
		}
	}
}

//=========================================================================== 

void GuiSprite::Render( const Math::Matrix44 &orthoMatrix, bool drawBoundingBox )
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

	Math::Matrix44 transform;
	SimpleDraw* simpleDraw = GameApp::Get()->GetSimpleDraw();
	simpleDraw->SetProjectionMatrix( orthoMatrix );
	simpleDraw->SetViewMatrix( transform );
	simpleDraw->SetTransformMatrix( transform );

	Texture* currentTexture = NULL;
	if ( mSpriteAnimated )
	{
		SpriteAnimated::FrameTexCoord currentTexCoord = mSpriteAnimated->GetCurrentFrameTexCoord();

		texCoord[0] = currentTexCoord.mTexCoords[0];
		texCoord[1] = currentTexCoord.mTexCoords[1];
		texCoord[2] = currentTexCoord.mTexCoords[2];
		texCoord[3] = currentTexCoord.mTexCoords[3];

		currentTexture = mSpriteAnimated->GetTexture();
	}
	
	simpleDraw->SetColorTexture( currentTexture );	


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

void GuiSprite::LoadSprite( const char* filename )
{
	Clear();

	mSpriteAnimated = NEW_PTR( "Sprite Animated" ) SpriteAnimated;
	mSpriteAnimated->Load( filename );
}

//===========================================================================

void GuiSprite::Clear()
{
	DELETE_PTR( mSpriteAnimated );
	mSpriteAnimated = NULL;
}

//===========================================================================

Texture* GuiSprite::GetTexture()
{
	if ( mSpriteAnimated )
	{
		return mSpriteAnimated->GetTexture();
	}
	else
	{
		return NULL;
	}
}

//===========================================================================

void GuiSprite::SetFramesPerSecond( float fps )
{
	if ( mSpriteAnimated )
	{
		mSpriteAnimated->SetFramesPerSecond( fps );
	}
}

//===========================================================================

float GuiSprite::GetFramesPerSecond()
{
	if ( mSpriteAnimated )
	{
		return mSpriteAnimated->GetFramesPerSecond();
	}

	return 24.0f;
}