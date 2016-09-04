#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogles2/oglesrendertarget.hpp"
#include "common/engine/render/ogles2/oglestexture.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/system/memory.hpp"

#if defined PLAT_PC
#include "opengles2/include/gles2/gl2ext.h"
#endif

#include <stdio.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////


//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//===========================================================================

OGLESRenderTarget::OGLESRenderTarget()
{
}

//===========================================================================

OGLESRenderTarget::~OGLESRenderTarget()
{	
}

//===========================================================================

void OGLESRenderTarget::CreateColorTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{   
	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );

	Texture* texture = NULL;
	unsigned int renderBuffer = 0;

	if ( renderToTexture )
	{
		texture = NEW_PTR( "Render Target OGL ES Texture" ) OGLESTexture;
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
	}
	else
	{
		OGLES::oglGenRenderbuffers( 1, &renderBuffer );
		OGLES::oglBindRenderbuffer( GL_RENDERBUFFER, renderBuffer );

		GLenum internalFormat = GL_RGBA8_OES;
		OGLES::oglRenderbufferStorage( GL_RENDERBUFFER, internalFormat, mWidth, mHeight );	
	}
		
	mColorTargetData.mTexture = texture;
	mColorTargetData.mRenderBuffer = renderBuffer;

	OGLESRenderer* oglesRenderer = reinterpret_cast<OGLESRenderer*>( Renderer::Get() );
	oglesRenderer->AddColorTarget( mColorTargetData );
}

//===========================================================================

void OGLESRenderTarget::CreateDepthTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{   
	UNUSED_ALWAYS( renderTargetFormat );

	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );
	Assert( renderTargetFormat == RT_FORMAT_DEPTH, "Not a valid target format." );

	Texture* texture = NULL;
	unsigned int renderBuffer = 0;

	if ( renderToTexture )
	{
		texture = NEW_PTR( "Render Target OGL ES Texture" ) OGLESTexture;
		texture->SetName( "RenderTargetTexture" );

		TextureFormat textureFormat = TEXTURE_FORMAT_DEPTH;		
		texture->CreateTexture( mWidth, mHeight, textureFormat );
	}
	else
	{
		OGLES::oglGenRenderbuffers( 1, &renderBuffer );
		OGLES::oglBindRenderbuffer( GL_RENDERBUFFER, renderBuffer );

		GLenum internalFormat = GL_DEPTH_COMPONENT24_OES;
		OGLES::oglRenderbufferStorage( GL_RENDERBUFFER, internalFormat, mWidth, mHeight );	
	}

	mDepthTargetData.mTexture = texture;
	mDepthTargetData.mRenderBuffer = renderBuffer;

	OGLESRenderer* oglesRenderer = reinterpret_cast<OGLESRenderer*>( Renderer::Get() );
	oglesRenderer->AddDepthTarget( mDepthTargetData );
}

//===========================================================================

void OGLESRenderTarget::SetColorTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	OGLESRenderTarget* oglesRenderTarget = reinterpret_cast<OGLESRenderTarget*>( renderTarget );
	mColorTargetData.mTexture = oglesRenderTarget->GetColorTexture();
	mColorTargetData.mRenderBuffer = oglesRenderTarget->GetColorRenderBuffer();
}

//===========================================================================

void OGLESRenderTarget::SetDepthTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	OGLESRenderTarget* oglesRenderTarget = reinterpret_cast<OGLESRenderTarget*>( renderTarget );
	mDepthTargetData.mTexture = oglesRenderTarget->GetDepthTexture();
	mDepthTargetData.mRenderBuffer = oglesRenderTarget->GetDepthRenderBuffer();
}

//===========================================================================

Texture* OGLESRenderTarget::GetColorTexture()
{
	return mColorTargetData.mTexture;
}

//===========================================================================

Texture* OGLESRenderTarget::GetDepthTexture()
{
	return mDepthTargetData.mTexture;
}

//===========================================================================

unsigned int OGLESRenderTarget::GetColorRenderBuffer()
{
	return mColorTargetData.mRenderBuffer;
}

//===========================================================================

unsigned int OGLESRenderTarget::GetDepthRenderBuffer()
{
	return mDepthTargetData.mRenderBuffer;
}

#endif