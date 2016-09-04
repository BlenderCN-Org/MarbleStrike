#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogl/oglrenderer.hpp"
#include "common/engine/render/ogl/oglrendertarget.hpp"
#include "common/engine/render/ogl/ogltexture.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include <gl/glew.h>
#include <stdio.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

ResourceHandle<SamplerState> OGLRenderTarget::mDefaultSamplerState;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLRenderTarget::OGLRenderTarget()
{	
    if ( mDefaultSamplerState.IsNull() )
	{
        mDefaultSamplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerState( "default" );
	}
}

//===========================================================================

OGLRenderTarget::~OGLRenderTarget()
{
}

//===========================================================================

void OGLRenderTarget::CreateColorTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{   
	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );

	Texture* texture = NULL;
	unsigned int renderBuffer = 0;
		
	if ( renderToTexture )
	{
		texture = NEW_PTR( "Render Target OGL Texture" ) OGLTexture;
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
		case RT_FORMAT_RG32:
			textureFormat = TEXTURE_FORMAT_RG32;
			break;
		default:
			Assert( false, "Cannot find format." );
			break;
		}			
		texture->CreateTexture( mWidth, mHeight, textureFormat );		

        SamplerState* samplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerStateByHandle( mDefaultSamplerState );
        
		Assert( samplerState->GetData()->mMipFilter == Filter_None, "" );
		RenderStateShadowing renderStateShadowing;
		samplerState->Dispatch( &renderStateShadowing, NULL, 0, true );
	}
	else
	{
		OGL::oglGenRenderbuffersEXT( 1, &renderBuffer );
		OGL::oglBindRenderbufferEXT( GL_RENDERBUFFER_EXT, renderBuffer );

		GLenum internalFormat = GL_RGBA8_EXT;
		OGL::oglRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, internalFormat, mWidth, mHeight );	
	}

	mColorTargetData.mTexture = texture;
	mColorTargetData.mRenderBuffer = renderBuffer;

	OGLRenderer* oglRenderer = reinterpret_cast<OGLRenderer*>( Renderer::Get() );
	oglRenderer->AddColorTarget( mColorTargetData );
}


//===========================================================================

void OGLRenderTarget::CreateDepthTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{   
	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );
	Assert( renderTargetFormat == RT_FORMAT_DEPTH, "Not a valid target format." );
    UNUSED_ALWAYS( renderTargetFormat );

	Texture* texture = NULL;
	unsigned int renderBuffer = 0;

	if ( renderToTexture )
	{
		texture = NEW_PTR( "Render Target OGL Texture" ) OGLTexture;
		texture->SetName( "RenderTargetTexture" );

		TextureFormat textureFormat = TEXTURE_FORMAT_DEPTH;		
		texture->CreateTexture( mWidth, mHeight, textureFormat );

        SamplerState* samplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerStateByHandle( mDefaultSamplerState );

		Assert( samplerState->GetData()->mMipFilter == Filter_None, "" );
		RenderStateShadowing renderStateShadowing;
		samplerState->Dispatch( &renderStateShadowing, NULL, 0, true );
	}
	else
	{
		OGL::oglGenRenderbuffersEXT( 1, &renderBuffer );
		OGL::oglBindRenderbufferEXT( GL_RENDERBUFFER_EXT, renderBuffer );

		GLenum internalFormat = GL_DEPTH_COMPONENT24_ARB;
		OGL::oglRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, internalFormat, mWidth, mHeight );	
	}

	mDepthTargetData.mTexture = texture;
	mDepthTargetData.mRenderBuffer = renderBuffer;

	OGLRenderer* oglRenderer = reinterpret_cast<OGLRenderer*>( Renderer::Get() );
	oglRenderer->AddDepthTarget( mDepthTargetData );
}

//===========================================================================

void OGLRenderTarget::SetColorTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	OGLRenderTarget* oglRenderTarget = reinterpret_cast<OGLRenderTarget*>( renderTarget );
	mColorTargetData.mTexture = oglRenderTarget->GetColorTexture();
	mColorTargetData.mRenderBuffer = oglRenderTarget->GetColorRenderBuffer();
}

//===========================================================================

void OGLRenderTarget::SetDepthTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	OGLRenderTarget* oglRenderTarget = reinterpret_cast<OGLRenderTarget*>( renderTarget );
	mDepthTargetData.mTexture = oglRenderTarget->GetDepthTexture();
	mDepthTargetData.mRenderBuffer = oglRenderTarget->GetDepthRenderBuffer();
}

//===========================================================================

Texture* OGLRenderTarget::GetColorTexture()
{
	return mColorTargetData.mTexture;
}

//===========================================================================

Texture* OGLRenderTarget::GetDepthTexture()
{
	return mDepthTargetData.mTexture;
}

//===========================================================================

unsigned int OGLRenderTarget::GetColorRenderBuffer()
{
	return mColorTargetData.mRenderBuffer;
}

//===========================================================================

unsigned int OGLRenderTarget::GetDepthRenderBuffer()
{
	return mDepthTargetData.mRenderBuffer;
}

#endif