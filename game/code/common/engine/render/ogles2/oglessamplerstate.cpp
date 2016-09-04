#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogles2/oglessamplerstate.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/render/renderstatemanager.hpp"
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
	GL_MIRRORED_REPEAT,		// TextureAddressMode_Mirror,
	GL_CLAMP_TO_EDGE,		// TextureAddressMode_Clamp,
	GL_CLAMP_TO_EDGE,		// TextureAddressMode_Border,
	GL_REPEAT,				// TextureAddressMode_Mirror_once
};

int OGLESSamplerState::MipMapFilterTranslation( FilterMode minFilter, FilterMode mipFilter )
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

OGLESSamplerState::OGLESSamplerState() 
{    
}

//============================================================================

OGLESSamplerState::~OGLESSamplerState()
{
}

//============================================================================

void OGLESSamplerState::Dispatch( 
	RenderStateShadowing* /*renderStateShadowing*/,
	void* /*context*/,
	int /*samplerIndex*/, 
	bool /*forceUpload*/ 
	)
{		
}


//============================================================================

void OGLESSamplerState::SetTextureState( int textureType )
{
	OGLES::oglTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, MipMapFilterTranslation( mData.mMinFilter, mData.mMipFilter ) );
	OGLES::oglTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, gFilterModeTranslation[mData.mMagFilter] );
	OGLES::oglTexParameteri( textureType, GL_TEXTURE_WRAP_S, gTextureAddressTranslation[mData.mAddressU] );
	OGLES::oglTexParameteri( textureType, GL_TEXTURE_WRAP_T, gTextureAddressTranslation[mData.mAddressV] );
}

#endif