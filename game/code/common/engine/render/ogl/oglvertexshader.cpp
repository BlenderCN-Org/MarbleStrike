#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogl/oglvertexshader.hpp"
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

OGLVertexShader::OGLVertexShader() 
{    
}

//============================================================================

OGLVertexShader::~OGLVertexShader()
{
}

//============================================================================

void OGLVertexShader::Load( const char* path, const char* name )
{
	char fileName[256];
	System::StringCopy( fileName, 256, Database::Get()->GetShaderResourcePath() );
	System::StringConcat( fileName, 256, path );
	System::StringConcat( fileName, 256, "binogl\\" );
	System::StringConcat( fileName, 256, name );
	System::StringConcat( fileName, 256, ".vert" );

	CreateShaderObject( GL_VERTEX_SHADER_ARB );	

	System::StringCopy( mFilename, 256, fileName );
	LoadShader( mFilename );	
}

//============================================================================

void OGLVertexShader::Reload()
{
	LoadShader( mFilename );	
}

#endif