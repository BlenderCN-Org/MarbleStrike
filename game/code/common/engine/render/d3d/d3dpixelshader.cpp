#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d/d3dpixelshader.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DPixelShader::D3DPixelShader() 
:	mPixelShader( NULL )
,	mPixelConstantTable( NULL )
{    
}

//============================================================================

D3DPixelShader::~D3DPixelShader()
{
	if ( mPixelShader != NULL )
	{
		mPixelShader->Release();
		mPixelShader = NULL;
	}

	if ( mPixelConstantTable != NULL )
	{
		mPixelConstantTable->Release();
		mPixelConstantTable = NULL;
	}
}

//============================================================================

void D3DPixelShader::Load( const char* path, const char* name )
{
	char fullFilename[256];
	System::StringCopy( fullFilename, 256, Database::Get()->GetShaderResourcePath() );
	System::StringConcat( fullFilename, 256, path );
	System::StringConcat( fullFilename, 256, "bind3d\\" );
	System::StringConcat( fullFilename, 256, name );
	System::StringConcat( fullFilename, 256, ".psh.o" );

	System::StringCopy( mFilename, 256, fullFilename );
	FILE* file = System::OpenFile( fullFilename, "rb" );
	int fileSize = System::GetFileSize( file );		
	char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];	
	fread( dataBuffer, sizeof( char ), fileSize, file );
	fclose( file );

	D3DXGetShaderConstantTable( (DWORD*)dataBuffer, &mPixelConstantTable );

	HRESULT hr = D3DRenderer::GetDevice()->CreatePixelShader( (DWORD*)dataBuffer, &mPixelShader );
	Assert( hr == D3D_OK, "" );
    UNUSED_ALWAYS( hr );

	DELETE_PTR_ARRAY( dataBuffer );
}

//============================================================================

void D3DPixelShader::Reload()
{
	if ( mPixelShader != NULL )
	{
		mPixelShader->Release();
		mPixelShader = NULL;
	}

	if ( mPixelConstantTable != NULL )
	{
		mPixelConstantTable->Release();
		mPixelConstantTable = NULL;
	}

	FILE* file = System::OpenFile( mFilename, "rb" );
	int fileSize = System::GetFileSize( file );		
	char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];	
	fread( dataBuffer, sizeof( char ), fileSize, file );
	fclose( file );

	D3DXGetShaderConstantTable( (DWORD*)dataBuffer, &mPixelConstantTable );

	HRESULT hr = D3DRenderer::GetDevice()->CreatePixelShader( (DWORD*)dataBuffer, &mPixelShader );
	Assert( hr == D3D_OK, "" );
    UNUSED_ALWAYS( hr );

	DELETE_PTR_ARRAY( dataBuffer );
}

//============================================================================

void D3DPixelShader::Dispatch()
{	
	D3DRenderer::GetDevice()->SetPixelShader( mPixelShader );
}

#endif