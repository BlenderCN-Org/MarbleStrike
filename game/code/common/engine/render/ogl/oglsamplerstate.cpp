#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogl/oglsamplerstate.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/renderstatemanager.hpp"

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
	GL_CLAMP_TO_BORDER,		// TextureAddressMode_Border,
	GL_MIRROR_CLAMP_EXT,	// TextureAddressMode_Mirror_once
};

int OGLSamplerState::MipMapFilterTranslation( FilterMode minFilter, FilterMode mipFilter )
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

OGLSamplerState::OGLSamplerState() 
{    
}

//============================================================================

OGLSamplerState::~OGLSamplerState()
{
}

//============================================================================

void OGLSamplerState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context, 
	int samplerIndex, 
	bool forceUpload 
	)
{	
	UNUSED_ALWAYS( renderStateShadowing );
	UNUSED_ALWAYS( context );	
	UNUSED_ALWAYS( samplerIndex );
	UNUSED_ALWAYS( forceUpload );
}

//============================================================================

void OGLSamplerState::SetTextureState( int textureType )
{
	OGL::oglTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, MipMapFilterTranslation( mData.mMinFilter, mData.mMipFilter ) );
	OGL::oglTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, gFilterModeTranslation[mData.mMagFilter] );
	OGL::oglTexParameteri( textureType, GL_TEXTURE_WRAP_S, gTextureAddressTranslation[mData.mAddressU] );
	OGL::oglTexParameteri( textureType, GL_TEXTURE_WRAP_T, gTextureAddressTranslation[mData.mAddressV] );	
	OGL::oglTexParameteri( textureType, GL_TEXTURE_WRAP_R, gTextureAddressTranslation[mData.mAddressW] );
	OGL::oglTexParameterfv( textureType, GL_TEXTURE_BORDER_COLOR, mData.mBorderColor );
	OGL::oglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);

	unsigned int maxAnisotropy = 1;
	if ( mData.mMipFilter != Filter_None )
	{
		maxAnisotropy = mData.mMaxAnisotropy;
	}
	OGL::oglTexParameteri( textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy );	
}

#endif