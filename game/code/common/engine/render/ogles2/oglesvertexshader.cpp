#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogles2/oglesvertexshader.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLESVertexShader::OGLESVertexShader() 
{    
}

//============================================================================

OGLESVertexShader::~OGLESVertexShader()
{
}

//============================================================================

void OGLESVertexShader::Load( const char* path, const char* name )
{
	char fullFilename[256];
	System::StringCopy( fullFilename, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( fullFilename, 256, path );
	System::StringConcat( fullFilename, 256, "binogl\\" );
	System::StringConcat( fullFilename, 256, name );
	System::StringConcat( fullFilename, 256, ".vert" );

	CreateShaderObject( GL_VERTEX_SHADER );	

	System::StringCopy( mFilename, 256,  System::Path::SystemSlash( fullFilename ) );
	LoadShader( mFilename );
}

//============================================================================

void OGLESVertexShader::Reload()
{
	LoadShader( mFilename );
}

#endif