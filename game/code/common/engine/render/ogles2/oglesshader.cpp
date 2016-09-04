#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogles2/oglesshader.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/rawfile.hpp"
#include "common/engine/system/string.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdio.h>
#include <string>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLESShader::OGLESShader() 
:	mShaderHandle( 0 )
{    
}

//============================================================================

OGLESShader::~OGLESShader()
{
}

//============================================================================

void OGLESShader::CreateShaderObject( GLenum shaderType )
{
	mShaderHandle = OGLES::oglCreateShader( shaderType );
}

//===========================================================================

void OGLESShader::LoadShader( const char* filename )
{
	char errorString[256];

	RawFile originalShader;
	originalShader.OpenAsString( filename );	

	const int shaderCount = 2;
	char* sourceData[shaderCount];
	GLint lengthData[shaderCount];

	int i = 0;

	const char* platformDefineString = "#define PLAT_OPENGLES\n";
	size_t platformDefineStringLength = strlen( platformDefineString );
	sourceData[0] = NEW_PTR( "Shader Data" ) char[platformDefineStringLength + 1];	
	System::StringNCopy( sourceData[0], (int)platformDefineStringLength + 1, platformDefineString, (int)platformDefineStringLength + 1 );
	lengthData[0] = (int)strlen( sourceData[0] );
	i++;
	
	sourceData[1] = NEW_PTR( "Shader Data" ) char[originalShader.GetSize() + 1];	
	strncpy( sourceData[1], originalShader.GetData(), originalShader.GetSize() + 1 );	
	lengthData[1] = (int)strlen( sourceData[1] );
	i++;
	
	const char** rawData = const_cast<const char**>( sourceData );

	OGLES::oglShaderSource( mShaderHandle, shaderCount, rawData, &lengthData[0] );
	OGLES::oglCompileShader( mShaderHandle );

	for ( int i = 0; i < shaderCount; i++ )
	{
		DELETE_PTR_ARRAY( sourceData[i] );
	}
	
	//
	// Error checking: check if the shader successfully compiled
	//
	int compiled = 0;
	OGLES::oglGetShaderiv( mShaderHandle, GL_COMPILE_STATUS, &compiled );
	if ( compiled == 0 )
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		OGLES::oglGetShaderiv( mShaderHandle, GL_INFO_LOG_LENGTH, &infologLength );
		if ( infologLength > 0 )
		{
			infoLog = ( char* )malloc(infologLength);
			OGLES::oglGetShaderInfoLog( mShaderHandle, infologLength, &charsWritten, infoLog );

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