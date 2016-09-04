#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d11/d3d11pixelshader.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include <json/json.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11PixelShader::D3D11PixelShader() 
:	mPixelShader( NULL )
{
    mPixelBufferSize = 0;
}

//============================================================================

D3D11PixelShader::~D3D11PixelShader()
{
	if ( mPixelShader != NULL )
	{
		mPixelShader->Release();
		mPixelShader = NULL;
	}
}

//============================================================================

void D3D11PixelShader::Load( const char* path, const char* name )
{
	char fullFilename[256];
	System::StringCopy( fullFilename, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( fullFilename, 256, path );
	System::StringConcat( fullFilename, 256, "bind3d11\\" );
	System::StringConcat( fullFilename, 256, name );
	System::StringConcat( fullFilename, 256, ".psh.o" );

	System::StringCopy( mFilename, 256, fullFilename );
	FILE* file = System::OpenFile( fullFilename, "rb" );
	Assert( file != NULL, "" );
	int fileSize = System::GetFileSize( file );		
	char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];	
	fread( dataBuffer, sizeof( char ), fileSize, file );
	fclose( file );
	file = NULL;

	HRESULT hr = D3D11Renderer::GetDevice()->CreatePixelShader( (DWORD*)dataBuffer, fileSize, NULL, &mPixelShader );
	if ( hr != S_OK )
	{
		Assert( false, "Error creating pixel shader.")
	}
		
	DELETE_PTR_ARRAY( dataBuffer );

    System::StringConcat( fullFilename, 256, ".desc" );
    Json::Value root;
    Json::Reader reader;
    FILE* dataFile = System::OpenFile( fullFilename, "r" );
    int dataFileSize = System::GetFileSize( dataFile );		
    char* shaderDescDataBuffer = NEW_PTR( "Data Buffer" ) char[dataFileSize];	
    fread( shaderDescDataBuffer, sizeof( char ), dataFileSize, dataFile );
    fclose( dataFile );
	dataFile = NULL;

    bool parsingSuccessful = reader.parse( shaderDescDataBuffer, root );
    if ( parsingSuccessful )
    {
        {
            const Json::Value constantBuffer = root["ConstantBuffer"];
            ConstantData data;
            mPixelBufferSize = 0;
            for ( Json::Value::ArrayIndex i = 0; i < constantBuffer.size(); ++i )
            {   
                const Json::Value currentBuffer = constantBuffer[i];
                std::string constantName = currentBuffer["ConstantName"].asString();
                const char* name = constantName.c_str();
                data.mNameHash = System::StringHash( name );
                data.mOffset = currentBuffer["ConstantStartOffset"].asInt();
                data.mSize = currentBuffer["ConstantSize"].asInt();
                mPixelBufferSize += data.mSize;

                mPixelConstantList.push_back( data );            
            }
        }

        {
            const Json::Value textureResources = root["TextureResource"];
            TextureData data;
            for ( Json::Value::ArrayIndex i = 0; i < textureResources.size(); ++i )
            {                
                const Json::Value currentTexture = textureResources[i];
                std::string constantName = currentTexture["ResourceName"].asString();
                const char* name = constantName.c_str();
                data.mNameHash = System::StringHash( name );
                data.mSamplerIndex = currentTexture["ResourceBindPoint"].asInt();
                mPixelTextureList.push_back( data );
            }
        }
    }
    DELETE_PTR_ARRAY( shaderDescDataBuffer );
}

//============================================================================

void D3D11PixelShader::Reload()
{
	if ( mPixelShader != NULL )
	{
		mPixelShader->Release();
		mPixelShader = NULL;
	}

	FILE* file = System::OpenFile( mFilename, "rb" );
	int fileSize = System::GetFileSize( file );		
	char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];	
	fread( dataBuffer, sizeof( char ), fileSize, file );
	fclose( file );
	file = NULL;

	HRESULT hr = D3D11Renderer::GetDevice()->CreatePixelShader( (DWORD*)dataBuffer,fileSize, NULL, &mPixelShader );
	if ( hr != S_OK )
	{
		Assert( false, "Error creating pixel shader.")
	}

	DELETE_PTR_ARRAY( dataBuffer );
}

//============================================================================

void D3D11PixelShader::Dispatch( void* context )
{	
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	deviceContext->PSSetShader( mPixelShader, 0, NULL );
}

#endif