#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglsamplerstate.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderer.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static int gFilterModeTranslation[] =
{	
	GL_NEAREST,		// Filter_None,
	GL_NEAREST,		// Filter_Point,
	GL_LINEAR,		// Filter_Linear,
	GL_NEAREST,		// Filter_Anisotropic
};

static int gTextureAddressTranslation[] =
{	
	GL_REPEAT,				// TextureAddressMode_Wrap,
	GL_REPEAT,				// TextureAddressMode_Mirror,
	GL_CLAMP_TO_EDGE,		// TextureAddressMode_Clamp,
	GL_REPEAT,				// TextureAddressMode_Border,
	GL_REPEAT,				// TextureAddressMode_Mirror_once
};

int FixedOGLSamplerState::MipMapFilterTranslation( FilterMode minFilter, FilterMode mipFilter )
{
	if ( mipFilter == Filter_None )
	{
		return gFilterModeTranslation[minFilter];
	}
	else
	{
		if ( minFilter == Filter_Point &&  mipFilter == Filter_Point )
		{
			return GL_NEAREST_MIPMAP_NEAREST;
		}
		else if ( minFilter == Filter_Linear && mipFilter == Filter_Point )
		{
			return GL_LINEAR_MIPMAP_NEAREST;
		}
		else if ( minFilter == Filter_Point && mipFilter == Filter_Linear )
		{
			return GL_NEAREST_MIPMAP_LINEAR;
		}
		else if ( minFilter == Filter_Linear && mipFilter == Filter_Linear )
		{
			return GL_LINEAR_MIPMAP_LINEAR;
		}
	}

	return -1;
}

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLSamplerState::FixedOGLSamplerState() 
{    
}

//============================================================================

FixedOGLSamplerState::~FixedOGLSamplerState()
{
}

//============================================================================

void FixedOGLSamplerState::Dispatch( 
	RenderStateShadowing* /*renderStateShadowing*/,
	void* /*context*/,
	int /*samplerIndex*/, 
	bool /*forceUpload*/ 
	)
{	
}

//============================================================================

void FixedOGLSamplerState::SetTextureState()
{
	FixedOGL::oglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MipMapFilterTranslation( mData.mMinFilter, mData.mMipFilter ) );
	FixedOGL::oglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gFilterModeTranslation[mData.mMagFilter] );
	FixedOGL::oglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gTextureAddressTranslation[mData.mAddressU] );
	FixedOGL::oglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gTextureAddressTranslation[mData.mAddressV] );
}

#endif