#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLRENDERTARGET_HPP
#define OGLRENDERTARGET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rendertarget.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class Texture;
class SamplerState;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct OGLESTargetData
{
	OGLESTargetData()
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

class OGLESRenderTarget : public RenderTarget
{
public:

    OGLESRenderTarget();
    ~OGLESRenderTarget();

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

	OGLESTargetData mColorTargetData;
	OGLESTargetData mDepthTargetData;
};

#endif

#endif