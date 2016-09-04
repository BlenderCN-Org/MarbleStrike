#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglvertexshader.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLVertexShader::FixedOGLVertexShader() 
{    
}

//============================================================================

FixedOGLVertexShader::~FixedOGLVertexShader()
{
}

//============================================================================

void FixedOGLVertexShader::Load( const char* path, const char* name )
{
	UNUSED_ALWAYS( path );
	System::StringCopy( mFilename, 256, name );
}

//===========================================================================

void FixedOGLVertexShader::LoadShader( const char* fileName )
{
	UNUSED_ALWAYS( fileName );
}

#endif