#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_OGL_RENDERTARGET_HPP
#define FIXED_OGL_RENDERTARGET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rendertarget.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class Texture;
class SamplerState;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct FixedOGLTargetData
{
	FixedOGLTargetData()
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

class FixedOGLRenderTarget : public RenderTarget
{
public:

	FixedOGLRenderTarget();
	~FixedOGLRenderTarget();

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

	static SamplerState* mDefaultSamplerState;

	FixedOGLTargetData mColorTargetData;
	FixedOGLTargetData mDepthTargetData;
};

#endif

#endif