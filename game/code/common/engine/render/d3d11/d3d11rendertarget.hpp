#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_RENDER_TARGET_HPP
#define D3D11_RENDER_TARGET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rendertarget.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/array.hpp"
#include <stdio.h>
#include <d3d11.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class Texture;

//////////////////////////////////////////////////////
// STRUCTURE
//////////////////////////////////////////////////////

struct D3D11ColorTargetData
{
	D3D11ColorTargetData()
	{
		mTexture = NULL;
		mColorTargetView = NULL;
	}

	Texture* mTexture;
	ID3D11RenderTargetView* mColorTargetView;
};

struct D3D11DepthTargetData
{
	D3D11DepthTargetData()
	{
		mTexture = NULL;        
		mDepthTargetView = NULL;
        mDepthTargetTexture = NULL;
	}

	Texture* mTexture;
    ID3D11Texture2D* mDepthTargetTexture;
	ID3D11DepthStencilView* mDepthTargetView;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11RenderTarget : public RenderTarget
{
public:
    
    D3D11RenderTarget();
    ~D3D11RenderTarget();

	void CreateColorTarget( 
		RenderTargetFormat renderTargetFormat,
		bool renderToTexture = false 
		);
	void CreateDepthTarget(
		RenderTargetFormat renderTargetFormat,
		bool renderToTexture = false 
		);
	void SetColorTarget( RenderTarget* renderTarget );
	void SetDepthTarget( RenderTarget* renderTarget );
	Texture* GetColorTexture();
	Texture* GetDepthTexture();
		
	ID3D11RenderTargetView* GetColorRenderTargetView();
	ID3D11DepthStencilView* GetDepthRenderTargetView();

private:

	D3D11ColorTargetData mColorTargetData;
	D3D11DepthTargetData mDepthTargetData;

};

#endif

#endif