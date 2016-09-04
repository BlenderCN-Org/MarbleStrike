#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglrendertarget.hpp"
#include "iphone/render/ogles1.1/fixedogltexture.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

SamplerState* FixedOGLRenderTarget::mDefaultSamplerState = NULL;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLRenderTarget::FixedOGLRenderTarget()
{
	if ( mDefaultSamplerState == NULL )
	{
		mDefaultSamplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerState( "default" );
	}
}

//===========================================================================

FixedOGLRenderTarget::~FixedOGLRenderTarget()
{
}

//===========================================================================

void FixedOGLRenderTarget::CreateColorTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{   
	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );

	Texture* texture = NULL;
	unsigned int renderBuffer = 0;

	if ( renderToTexture )
	{
		texture = NEW_PTR( "Render Target OGL Texture" ) FixedOGLTexture;
		texture->SetName( "RenderTargetTexture" );

		TextureFormat textureFormat = TEXTURE_FORMAT_RGBA8;
		switch ( renderTargetFormat )
		{
		case RT_FORMAT_RGBA8:
			textureFormat = TEXTURE_FORMAT_RGBA8;
			break;
		case RT_FORMAT_RGBA16:
			textureFormat = TEXTURE_FORMAT_RGBA16;
			break;
		case RT_FORMAT_RGBA32:
			textureFormat = TEXTURE_FORMAT_RGBA32;
			break;
		case RT_FORMAT_DEPTH:
			textureFormat = TEXTURE_FORMAT_DEPTH;
			break;
		default:
			Assert( false, "Cannot find format." );
			break;
		}			
		texture->CreateTexture( mWidth, mHeight, textureFormat );

		Assert( mDefaultSamplerState->GetData()->mMipFilter == Filter_None, "" );
		RenderStateShadowing renderStateShadowing;
		mDefaultSamplerState->Dispatch( &renderStateShadowing, NULL, 0, true );		
	}
	else
	{
		FixedOGL::oglGenRenderbuffers( 1, &renderBuffer );
		FixedOGL::oglBindRenderbuffer( GL_RENDERBUFFER_OES, renderBuffer );

		GLenum internalFormat = GL_RGBA8_OES;
		FixedOGL::oglRenderbufferStorage( GL_RENDERBUFFER_OES, internalFormat, mWidth, mHeight );				
	}

	mColorTargetData.mTexture = texture;
	mColorTargetData.mRenderBuffer = renderBuffer;

	FixedOGLRenderer* fixedOGLRenderer = reinterpret_cast<FixedOGLRenderer*>( Renderer::Get() );
	fixedOGLRenderer->AddColorTarget( mColorTargetData );
}

//===========================================================================

void FixedOGLRenderTarget::CreateDepthTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{   
	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );
	Assert( renderTargetFormat == RT_FORMAT_DEPTH, "Not a valid target format." );
    UNUSED_ALWAYS( renderTargetFormat );

	Texture* texture = NULL;
	unsigned int renderBuffer = 0;

	if ( renderToTexture )
	{
		texture = NEW_PTR( "Render Target OGL Texture" ) FixedOGLTexture;
		texture->SetName( "RenderTargetTexture" );

		TextureFormat textureFormat = TEXTURE_FORMAT_DEPTH;
		texture->CreateTexture( mWidth, mHeight, textureFormat );

		Assert( mDefaultSamplerState->GetData()->mMipFilter == Filter_None, "" );
		RenderStateShadowing renderStateShadowing;
		mDefaultSamplerState->Dispatch( &renderStateShadowing, NULL, 0, true );		
	}
	else
	{
		FixedOGL::oglGenRenderbuffers( 1, &renderBuffer );
		FixedOGL::oglBindRenderbuffer( GL_RENDERBUFFER_OES, renderBuffer );

		GLenum internalFormat = GL_DEPTH_COMPONENT24_OES;
		FixedOGL::oglRenderbufferStorage( GL_RENDERBUFFER_OES, internalFormat, mWidth, mHeight );	
	}

	mDepthTargetData.mTexture = texture;
	mDepthTargetData.mRenderBuffer = renderBuffer;

	FixedOGLRenderer* fixedOGLRenderer = reinterpret_cast<FixedOGLRenderer*>( Renderer::Get() );
	fixedOGLRenderer->AddDepthTarget( mDepthTargetData );
}

//===========================================================================

void FixedOGLRenderTarget::SetColorTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	FixedOGLRenderTarget* fixedOGLRenderer = reinterpret_cast<FixedOGLRenderTarget*>( renderTarget );
	mColorTargetData.mTexture = fixedOGLRenderer->GetColorTexture();
	mColorTargetData.mRenderBuffer = fixedOGLRenderer->GetColorRenderBuffer();
}

//===========================================================================

void FixedOGLRenderTarget::SetDepthTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	FixedOGLRenderTarget* fixedOGLRenderer = reinterpret_cast<FixedOGLRenderTarget*>( renderTarget );
	mDepthTargetData.mTexture = fixedOGLRenderer->GetDepthTexture();
	mDepthTargetData.mRenderBuffer = fixedOGLRenderer->GetDepthRenderBuffer();
}

//===========================================================================

Texture* FixedOGLRenderTarget::GetColorTexture()
{
	return mColorTargetData.mTexture;
}

//===========================================================================

Texture* FixedOGLRenderTarget::GetDepthTexture()
{
	return mDepthTargetData.mTexture;
}

//===========================================================================

unsigned int FixedOGLRenderTarget::GetColorRenderBuffer()
{
	return mColorTargetData.mRenderBuffer;
}

//===========================================================================

unsigned int FixedOGLRenderTarget::GetDepthRenderBuffer()
{
	return mDepthTargetData.mRenderBuffer;
}

#endif