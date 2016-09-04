#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11rendertarget.hpp"
#include "common/engine/render/d3d11/d3d11texture.hpp"
#include "common/engine/system/memory.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11RenderTarget::D3D11RenderTarget()
{
}

//===========================================================================

D3D11RenderTarget::~D3D11RenderTarget()
{
}

//===========================================================================

void D3D11RenderTarget::CreateColorTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{
	UNUSED_ALWAYS( renderToTexture );

	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );

	Texture* texture = NULL;
	ID3D11RenderTargetView* targetView = NULL;

	D3D11Texture* d3d11Texture = NULL;
	d3d11Texture = NEW_PTR( "D3D11 Texture" ) D3D11Texture;
	d3d11Texture->SetName( "RenderTargetTexture" );
	texture = d3d11Texture;

	DXGI_FORMAT d3dFormat = DXGI_FORMAT_R8G8B8A8_UINT;

	TextureFormat textureFormat = TEXTURE_FORMAT_RGBA8;
	switch ( renderTargetFormat )
	{
	case RT_FORMAT_RGBA8:
		textureFormat = TEXTURE_FORMAT_RGBA8;
		d3dFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case RT_FORMAT_RGBA16:
		textureFormat = TEXTURE_FORMAT_RGBA16;
		d3dFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
		break;
	case RT_FORMAT_RGBA32:
		textureFormat = TEXTURE_FORMAT_RGBA32;
		d3dFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	case RT_FORMAT_RG32:
		textureFormat = TEXTURE_FORMAT_RG32;
		d3dFormat = DXGI_FORMAT_R32G32_FLOAT;
		break;
    case RT_FORMAT_R32:
        textureFormat = TEXTURE_FORMAT_R32;
		d3dFormat = DXGI_FORMAT_R32_FLOAT;
        break;
	default:
		Assert( false, "Cannot find format." );
		break;
	}			
	texture->CreateTexture( mWidth, mHeight, textureFormat );

	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = d3dFormat;
	rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtDesc.Texture2D.MipSlice = 0;

	D3D11Renderer::GetDevice()->CreateRenderTargetView( d3d11Texture->GetD3D11Texture(), &rtDesc, &targetView );

	mColorTargetData.mTexture = texture;
	mColorTargetData.mColorTargetView = targetView;

	D3D11Renderer* d3d11Renderer = reinterpret_cast<D3D11Renderer*>( Renderer::Get() );
	d3d11Renderer->AddColorTarget( mColorTargetData );

}

//===========================================================================

void D3D11RenderTarget::CreateDepthTarget( RenderTargetFormat renderTargetFormat, bool renderToTexture )
{
	UNUSED_ALWAYS( renderTargetFormat );
	UNUSED_ALWAYS( renderToTexture );

	Assert( mWidth > 0, "Width is not valid." );
	Assert( mHeight > 0, "Height is not valid." );
		
    Texture* texture = NULL;
    ID3D11DepthStencilView* targetView = NULL;

    if ( renderToTexture )		
    {
        D3D11Texture* d3d11Texture = NULL;
        d3d11Texture = NEW_PTR( "D3D11 Texture" ) D3D11Texture;
        d3d11Texture->SetName( "RenderTargetTexture" );
        texture = d3d11Texture;
        texture->CreateTexture( mWidth, mHeight, TEXTURE_FORMAT_DEPTH );

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory( &descDSV, sizeof(descDSV) );
        descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;

        D3D11Renderer::GetDevice()->CreateDepthStencilView( d3d11Texture->GetD3D11Texture(), &descDSV, &targetView );
        mDepthTargetData.mTexture = texture;        
    }
    else
    {
        ID3D11Texture2D* depthStencilTexture;

        D3D11_TEXTURE2D_DESC descDepth;
        ZeroMemory( &descDepth, sizeof(descDepth) );
        descDepth.Width = mWidth;
        descDepth.Height = mHeight;
        descDepth.MipLevels = 1;
        descDepth.ArraySize = 1;
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 1;
        descDepth.SampleDesc.Quality = 0;
        descDepth.Usage = D3D11_USAGE_DEFAULT;
        descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        descDepth.CPUAccessFlags = 0;
        descDepth.MiscFlags = 0;
        HRESULT hr = D3D11Renderer::GetDevice()->CreateTexture2D( &descDepth, NULL, &depthStencilTexture );
        UNUSED_ALWAYS( hr );
        Assert( hr == S_OK, "" );

        D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
        ZeroMemory( &descDSV, sizeof(descDSV) );
        descDSV.Format = descDepth.Format;
        descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        descDSV.Texture2D.MipSlice = 0;
        D3D11Renderer::GetDevice()->CreateDepthStencilView( depthStencilTexture, &descDSV, &targetView );
        Assert( hr == S_OK, "" );
        mDepthTargetData.mTexture = NULL;
        mDepthTargetData.mDepthTargetTexture = depthStencilTexture;
    }

    mDepthTargetData.mDepthTargetView = targetView;    

	D3D11Renderer* d3d11Renderer = reinterpret_cast<D3D11Renderer*>( Renderer::Get() );
	d3d11Renderer->AddDepthTarget( mDepthTargetData );
}

//===========================================================================

void D3D11RenderTarget::SetColorTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	D3D11RenderTarget* d3d11RenderTarget = reinterpret_cast<D3D11RenderTarget*>( renderTarget );
	mColorTargetData.mTexture = d3d11RenderTarget->GetColorTexture();
	mColorTargetData.mColorTargetView = d3d11RenderTarget->GetColorRenderTargetView();
}

//===========================================================================

void D3D11RenderTarget::SetDepthTarget( RenderTarget* renderTarget )
{
	Assert( mWidth == renderTarget->GetWidth(), "Width does not match" );
	Assert( mHeight == renderTarget->GetHeight(), "Width does not match" );

	D3D11RenderTarget* d3d11RenderTarget = reinterpret_cast<D3D11RenderTarget*>( renderTarget );
	mDepthTargetData.mTexture = d3d11RenderTarget->GetDepthTexture();
	mDepthTargetData.mDepthTargetView = d3d11RenderTarget->GetDepthRenderTargetView();
}

//===========================================================================

Texture* D3D11RenderTarget::GetColorTexture()
{
	return mColorTargetData.mTexture;
}

//===========================================================================

Texture* D3D11RenderTarget::GetDepthTexture()
{
	return mDepthTargetData.mTexture;
}

//===========================================================================

ID3D11RenderTargetView* D3D11RenderTarget::GetColorRenderTargetView()
{
	return mColorTargetData.mColorTargetView;
}

//===========================================================================

ID3D11DepthStencilView* D3D11RenderTarget::GetDepthRenderTargetView()
{
	return mDepthTargetData.mDepthTargetView;
}

#endif