#ifndef RENDERTARGET_HPP
#define RENDERTARGET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <stdio.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class Texture;

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

enum RenderTargetFormat
{
	RT_FORMAT_RGBA8,
	RT_FORMAT_RGBA16,
	RT_FORMAT_RGBA32,
	RT_FORMAT_RG32,
    RT_FORMAT_R32,
	RT_FORMAT_DEPTH
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RenderTarget
{
public:

	RenderTarget();
	virtual ~RenderTarget();
	
	virtual void CreateColorTarget( 
		RenderTargetFormat renderTargetFormat,
		bool renderToTexture = false 
		) = 0;
	virtual void CreateDepthTarget(
		RenderTargetFormat renderTargetFormat,
		bool renderToTexture = false 
		) = 0;
	virtual void SetColorTarget( RenderTarget* renderTarget ) = 0;
	virtual void SetDepthTarget( RenderTarget* renderTarget ) = 0;
	virtual Texture* GetColorTexture() = 0;
	virtual Texture* GetDepthTexture() = 0;
	
	void SetWidth( int width );
	void SetHeight( int height );
	int GetWidth();
	int GetHeight();

protected:

	int mWidth;
	int mHeight;

private:

};

#endif