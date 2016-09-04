#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11vertexshader.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include <json/json.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11VertexShader::D3D11VertexShader()
    :	mVertexShader( NULL )
{    
    mVertexBufferSize = 0;
    mShaderByteCode = NULL;
    mShaderByteCodeSize = 0;
}

//============================================================================

D3D11VertexShader::~D3D11VertexShader()
{
    if ( mVertexShader != NULL )
    {
        mVertexShader->Release();
        mVertexShader = NULL;
    }

    DELETE_PTR_ARRAY( mShaderByteCode );
}

//============================================================================

void D3D11VertexShader::Load( const char* path, const char* name )
{
    char fullFilename[256];
    System::StringCopy( fullFilename, 256, Database::Get()->GetResourcePath() );
    System::StringConcat( fullFilename, 256, path );
    System::StringConcat( fullFilename, 256, "bind3d11\\" );
    System::StringConcat( fullFilename, 256, name );
    System::StringConcat( fullFilename, 256, ".vsh.o" );
	
    System::StringCopy( mFilename, 256, fullFilename );
    FILE* file = System::OpenFile( fullFilename, "rb" );
	Assert( file != NULL, "" );
    mShaderByteCodeSize = System::GetFileSize( file );		
    Assert( mShaderByteCode == NULL, "" );
    mShaderByteCode = NEW_PTR( "Data Buffer" ) char[mShaderByteCodeSize];	
    fread( mShaderByteCode, sizeof( char ), mShaderByteCodeSize, file );
    fclose( file );
	file = NULL;

    HRESULT hr = D3D11Renderer::GetDevice()->CreateVertexShader( (DWORD*)mShaderByteCode, mShaderByteCodeSize, NULL, &mVertexShader );
    if ( hr != S_OK )
    {
        Assert( false, "Error loading vertex shader.")
    }    

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
            mVertexBufferSize = 0;
            for ( Json::Value::ArrayIndex i = 0; i < constantBuffer.size(); ++i )
            {

                const Json::Value currentBuffer = constantBuffer[i];
                std::string constantName = currentBuffer["ConstantName"].asString();
                const char* name = constantName.c_str();
                data.mNameHash = System::StringHash( name );
                data.mOffset = currentBuffer["ConstantStartOffset"].asInt();
                data.mSize = currentBuffer["ConstantSize"].asInt();
                mVertexBufferSize += data.mSize;

                mVertexConstantList.push_back( data );            
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
                mVertexTextureList.push_back( data );
            }
        }
    }
    DELETE_PTR_ARRAY( shaderDescDataBuffer );
}

//============================================================================

void D3D11VertexShader::Reload()
{
    if ( mVertexShader != NULL )
    {
        mVertexShader->Release();
        mVertexShader = NULL;
    }

    FILE* file = System::OpenFile( mFilename, "rb" );
    int fileSize = System::GetFileSize( file );	
    char* dataBuffer = NEW_PTR( "Data Buffer" ) char[fileSize];	
    fread( dataBuffer, sizeof( char ), fileSize, file );
    fclose( file );
	file = NULL;

    HRESULT hr = D3D11Renderer::GetDevice()->CreateVertexShader( (DWORD*)dataBuffer, fileSize, NULL, &mVertexShader );
    if ( hr != S_OK )
    {
        Assert( false, "Error creating vertex shader.")
    }

    DELETE_PTR_ARRAY( dataBuffer );
}

//============================================================================

void D3D11VertexShader::Dispatch( void* context )
{
    ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
    if ( context )
    {
        deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
    }

    deviceContext->VSSetShader( mVertexShader, 0, NULL );
}

#endif