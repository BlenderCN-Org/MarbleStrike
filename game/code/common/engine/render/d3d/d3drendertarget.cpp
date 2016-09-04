#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3drendertarget.hpp"
#include "common/engine/render/d3d/d3dtexture.hpp"
#include "common/engine/system/memory.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DRenderTarget::D3DRenderTarget()
{
}

//===========================================================================

D3DRenderTarget::~D3DRenderTarget()
{
}

//===========================================================================

void D3DRenderTarget::CreateColorTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{
	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );

	Texture* texture = NULL;
	LPDIRECT3DSURFACE9 renderSurface = 0;

	if ( renderToTexture )
	{
		D3DTexture* d3dTexture = NULL;
		d3dTexture = NEW_PTR( "D3D Texture" ) D3DTexture;
		d3dTexture->SetName( "RenderTargetTexture" );
		texture = d3dTexture;

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
		LPDIRECT3DTEXTURE9 texture2D = reinterpret_cast<LPDIRECT3DTEXTURE9>( d3dTexture->GetHandle() );
		texture2D->GetSurfaceLevel( 0, &renderSurface );
	}
	else
	{		
		D3DRenderer::GetDevice()->CreateRenderTarget(
			mWidth,
			mHeight,
			D3DFMT_A8R8G8B8,
			D3DMULTISAMPLE_NONE,
			0,
			true,
			&renderSurface,
			NULL
			);
	}

	mColorTargetData.mTexture = texture;
	mColorTargetData.mRenderSurface = renderSurface;

	D3DRenderer* d3dRenderer = reinterpret_cast<D3DRenderer*>( Renderer::Get() );
	d3dRenderer->AddColorTarget( mColorTargetData );
}

//===========================================================================

void D3DRenderTarget::CreateDepthTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{
	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );
	Assert( renderTargetFormat == RT_FORMAT_DEPTH, "Not a valid target format." );
    UNUSED_ALWAYS( renderTargetFormat );

	Texture* texture = NULL;
	LPDIRECT3DSURFACE9 renderSurface = 0;

	if ( renderToTexture )
	{
		D3DTexture* d3dTexture = NULL;
		d3dTexture = NEW_PTR( "D3D Texture" ) D3DTexture;
		d3dTexture->SetName( "RenderTargetTexture" );
		texture = d3dTexture;

		TextureFormat textureFormat = TEXTURE_FORMAT_DEPTH;		
		texture->CreateTexture( mWidth, mHeight, textureFormat );
		LPDIRECT3DTEXTURE9 texture2D = reinterpret_cast<LPDIRECT3DTEXTURE9>( d3dTexture->GetHandle() );
		texture2D->GetSurfaceLevel( 0, &renderSurface );
	}
	else
	{
		D3DRenderer::GetDevice()->CreateDepthStencilSurface(
			mWidth,
			mHeight,
			D3DFMT_D24S8,
			D3DMULTISAMPLE_NONE,
			0,
			true,
			&renderSurface,
			NULL 
			);
	}

	mDepthTargetData.mTexture = texture;
	mDepthTargetData.mRenderSurface = renderSurface;

	D3DRenderer* d3dRenderer = reinterpret_cast<D3DRenderer*>( Renderer::Get() );
	d3dRenderer->AddDepthTarget( mDepthTargetData );
}

//===========================================================================

void D3DRenderTarget::SetColorTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	D3DRenderTarget* d3dRenderTarget = reinterpret_cast<D3DRenderTarget*>( renderTarget );
	mColorTargetData.mTexture = d3dRenderTarget->GetColorTexture();
	mColorTargetData.mRenderSurface = d3dRenderTarget->GetColorRenderSurface();
}

//===========================================================================

void D3DRenderTarget::SetDepthTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	D3DRenderTarget* d3dRenderTarget = reinterpret_cast<D3DRenderTarget*>( renderTarget );
	mDepthTargetData.mTexture = d3dRenderTarget->GetDepthTexture();
	mDepthTargetData.mRenderSurface = d3dRenderTarget->GetDepthRenderSurface();
}

//===========================================================================

Texture* D3DRenderTarget::GetColorTexture()
{
	return mColorTargetData.mTexture;
}

//===========================================================================

Texture* D3DRenderTarget::GetDepthTexture()
{
	return mDepthTargetData.mTexture;
}

//===========================================================================

LPDIRECT3DSURFACE9 D3DRenderTarget::GetColorRenderSurface()
{
	return mColorTargetData.mRenderSurface;
}

//===========================================================================

LPDIRECT3DSURFACE9 D3DRenderTarget::GetDepthRenderSurface()
{
	return mDepthTargetData.mRenderSurface;
}

#endif