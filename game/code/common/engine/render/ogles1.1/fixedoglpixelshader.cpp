#if defined( RENDER_PLAT_OGLES1_1 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglpixelshader.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLPixelShader::FixedOGLPixelShader() 
:	mShaderHandle( 0 )
,	mCurrentTextureHandle( 0 )
{    
}

//============================================================================

FixedOGLPixelShader::~FixedOGLPixelShader()
{
}

//============================================================================

void FixedOGLPixelShader::Load( const char* /*path*/, const char* name )
{
	System::StringCopy( mFilename, 256, name );
}

//============================================================================

int FixedOGLPixelShader::GetCurrentSamplerIndex()
{
	return -1;
}

//===========================================================================

void FixedOGLPixelShader::LoadShader( const char* fileName )
{
	UNUSED_ALWAYS( fileName );
}

#endif