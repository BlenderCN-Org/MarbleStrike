#if defined( RENDER_PLAT_D3D9 )

#ifndef D3DRENDERTARGET_HPP
#define D3DRENDERTARGET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rendertarget.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/array.hpp"
#include <stdio.h>
#include <d3d9.h>
#include <d3dx9.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class Texture;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct D3DTargetData
{
	D3DTargetData()
	{
		mTexture = NULL;
		mRenderSurface = NULL;
	}

	Texture* mTexture;
	LPDIRECT3DSURFACE9 mRenderSurface;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3DRenderTarget : public RenderTarget
{
public:
    
    D3DRenderTarget();
    ~D3DRenderTarget();

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
	LPDIRECT3DSURFACE9 GetColorRenderSurface();
	LPDIRECT3DSURFACE9 GetDepthRenderSurface();

private:

	D3DTargetData mColorTargetData;
	D3DTargetData mDepthTargetData;

};

#endif

#endif