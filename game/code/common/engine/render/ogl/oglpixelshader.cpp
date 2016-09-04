#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogl/oglpixelshader.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLPixelShader::OGLPixelShader() 
{    
}

//============================================================================

OGLPixelShader::~OGLPixelShader()
{
}

//============================================================================

void OGLPixelShader::Load( const char* path, const char* name )
{
	char fileName[256];
	System::StringCopy( fileName, 256, Database::Get()->GetShaderResourcePath() );
	System::StringConcat( fileName, 256, path );
	System::StringConcat( fileName, 256, "binogl\\" );
	System::StringConcat( fileName, 256, name );
	System::StringConcat( fileName, 256, ".frag" );

	CreateShaderObject( GL_FRAGMENT_SHADER_ARB );

	System::StringCopy( mFilename, 256, fileName );
	LoadShader( fileName );
}

//============================================================================

void OGLPixelShader::Reload()
{
	LoadShader( mFilename );	
}

#endif