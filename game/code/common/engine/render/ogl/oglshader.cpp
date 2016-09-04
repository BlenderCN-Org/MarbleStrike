#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogl/oglshader.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/rawfile.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLShader::OGLShader() 
:	mShaderHandle( 0 )
{    
}

//============================================================================

OGLShader::~OGLShader()
{
	OGL::oglDeleteObject( mShaderHandle );
}

//============================================================================

void OGLShader::CreateShaderObject( GLenum shaderType )
{
	mShaderHandle = OGL::oglCreateShaderObject( shaderType );
}

//===========================================================================

void OGLShader::LoadShader( const char* filename )
{
	char errorString[256];

	RawFile originalShader;
	originalShader.OpenAsString( filename );
		
	const int shaderCount = 2;
	char* sourceData[shaderCount];
		
	const char* platformDefineString = "#define PLAT_OPENGL\n";
	int platformDefineStringLength = strlen( platformDefineString );
	sourceData[0] = NEW_PTR( "Shader Data" ) char[platformDefineStringLength + 1];
	System::StringNCopy( sourceData[0], platformDefineStringLength + 1, platformDefineString, platformDefineStringLength + 1 );		

	sourceData[1] = NEW_PTR( "Shader Data" ) char[originalShader.GetSize() + 1];
	System::StringNCopy( sourceData[1], originalShader.GetSize() + 1, originalShader.GetData(), originalShader.GetSize() + 1 );	

	const char** rawData = const_cast<const char**>( sourceData );
	OGL::oglShaderSource( mShaderHandle, shaderCount, rawData, NULL );
	OGL::oglCompileShader( mShaderHandle );

	for ( int i = 0; i < shaderCount; i++ )
	{
		DELETE_PTR_ARRAY( sourceData[i] );
	}

	//
	// Error checking: check if the shader successfully compiled
	//
	int compiled = 0;
	OGL::oglGetObjectParameteriv( mShaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &compiled );
	if ( compiled == 0 )
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		OGL::oglGetObjectParameteriv( mShaderHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &infologLength );
		if ( infologLength > 0 )
		{
			infoLog = ( char* )malloc(infologLength);
			OGL::oglGetInfoLog( mShaderHandle, infologLength, &charsWritten, infoLog );

			System::PrintToConsole( "%s\n", infoLog );

			snprintf( errorString, 256, "Error compiling shader: %s.", filename );
			Assert( false, errorString );
			free( infoLog );
		}

		snprintf( errorString, 256, "Error compiling shader: %s.", filename );
		Assert( false, errorString );
	}	
}

#endif