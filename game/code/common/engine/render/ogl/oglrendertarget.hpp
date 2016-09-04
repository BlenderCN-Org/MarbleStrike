#if defined( RENDER_PLAT_OPENGL )

#ifndef OGLRENDERTARGET_HPP
#define OGLRENDERTARGET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rendertarget.hpp"
#include "common/engine/system/array.hpp"
#include "common/engine/system/resourcehandle.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class Texture;
class SamplerState;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct OGLTargetData
{
	OGLTargetData()
	{
		mTexture = NULL;
		mRenderBuffer = 0;
	}

	Texture* mTexture;
	unsigned int mRenderBuffer;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLRenderTarget : public RenderTarget
{
public:

    OGLRenderTarget();
    ~OGLRenderTarget();

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
	unsigned int GetColorRenderBuffer();
	unsigned int GetDepthRenderBuffer();

private:

	static ResourceHandle<SamplerState> mDefaultSamplerState;
    
	OGLTargetData mColorTargetData;
	OGLTargetData mDepthTargetData;
};

#endif

#endif