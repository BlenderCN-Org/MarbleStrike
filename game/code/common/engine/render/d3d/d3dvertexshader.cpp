#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3dvertexshader.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DVertexShader::D3DVertexShader()
:	mVertexShader( NULL )
,	mVertexConstantTable( NULL )
{    
}

//============================================================================

D3DVertexShader::~D3DVertexShader()
{
	if ( mVertexShader != NULL )
	{
		mVertexShader->Release();
		mVertexShader = NULL;
	}

	if ( mVertexConstantTable != NULL )
	{
		mVertexConstantTable->Release();
		mVertexConstantTable = NULL;
	}
}

//============================================================================

void D3DVertexShader::Load( const char* path, const char* name )
{
	char fullFilename[256];
	System::StringCopy( fullFilename, 256, Database::Get()->GetShaderResourcePath() );
	System::StringConcat( fullFilename, 256, path );
	System::StringConcat( fullFilename, 256, "bind3d\\" );
	System::StringConcat( fullFilename, 256, name );
	System::StringConcat( fullFilename, 256, ".vsh.o" );

	System::StringCopy( mFilename, 256, fullFilename );

	FILE* file = System::OpenFile( fullFilename, "rb" );
	int fileSize = System::GetFileSize( file );	
	char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];	
	fread( dataBuffer, sizeof( char ), fileSize, file );
	fclose( file );

	D3DXGetShaderConstantTable( (DWORD*)dataBuffer, &mVertexConstantTable );

	HRESULT hr = D3DRenderer::GetDevice()->CreateVertexShader( (DWORD*)dataBuffer, &mVertexShader );
	Assert( hr == D3D_OK, "" );
    UNUSED_ALWAYS( hr );

	DELETE_PTR_ARRAY( dataBuffer );
}

//============================================================================

void D3DVertexShader::Reload()
{
	if ( mVertexShader != NULL )
	{
		mVertexShader->Release();
		mVertexShader = NULL;
	}

	if ( mVertexConstantTable != NULL )
	{
		mVertexConstantTable->Release();
		mVertexConstantTable = NULL;
	}

	FILE* file = System::OpenFile( mFilename, "rb" );
	int fileSize = System::GetFileSize( file );	
	char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];	
	fread( dataBuffer, sizeof( char ), fileSize, file );
	fclose( file );

	D3DXGetShaderConstantTable( (DWORD*)dataBuffer, &mVertexConstantTable );

	HRESULT hr = D3DRenderer::GetDevice()->CreateVertexShader( (DWORD*)dataBuffer, &mVertexShader );
	Assert( hr == D3D_OK, "" );
    UNUSED_ALWAYS( hr );

	DELETE_PTR_ARRAY( dataBuffer );
}

//============================================================================

void D3DVertexShader::Dispatch()
{
	D3DRenderer::GetDevice()->SetVertexShader( mVertexShader );
}

#endif