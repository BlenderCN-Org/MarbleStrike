#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogles2/oglespixelshader.hpp"
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

OGLESPixelShader::OGLESPixelShader() 
:	mCurrentTextureHandle( 0 )
{    
}

//============================================================================

OGLESPixelShader::~OGLESPixelShader()
{
}

//============================================================================

void OGLESPixelShader::Load( const char* path, const char* name )
{
	char fullFilename[256];
	System::StringCopy( fullFilename, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( fullFilename, 256, path );
	System::StringConcat( fullFilename, 256, "binogl\\" );
	System::StringConcat( fullFilename, 256, name );
	System::StringConcat( fullFilename, 256, ".frag" );

	CreateShaderObject( GL_FRAGMENT_SHADER );

	System::StringCopy( mFilename, 256,  System::Path::SystemSlash( fullFilename ) );
	LoadShader( mFilename );
}

//============================================================================

int OGLESPixelShader::GetCurrentSamplerIndex()
{
	int currentHandle = mCurrentTextureHandle;
	mCurrentTextureHandle++;
	return currentHandle;	
}

//============================================================================

void OGLESPixelShader::Reload()
{
	LoadShader( mFilename );
}

#endif