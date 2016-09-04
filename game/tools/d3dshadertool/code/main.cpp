//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include <windows.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <D3Dcompiler.h>
#include "json\json.h"

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

void CreateFileList( const std::string &d3dShaderDir, std::vector<std::string> &shaderFiles )
{
    WIN32_FIND_DATA wfd;
    HANDLE hFind;

    char strFind[_MAX_PATH];
    sprintf_s( strFind, "%s%s", d3dShaderDir.c_str(), "\\*" );	
    hFind = FindFirstFile( strFind, &wfd );

    if ( INVALID_HANDLE_VALUE != hFind )
    {
        do
        {
            if ( strcmp( wfd.cFileName, "." ) !=0 && strcmp( wfd.cFileName, ".." ) != 0 )
            {
                if ( !( wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )				
                {
                    std::string foundFileName = wfd.cFileName;

                    if ( foundFileName.find( ".desc" ) == std::string::npos )
                    {
                        shaderFiles.push_back( foundFileName );
                    }
                }
            }
        } 
        while( FindNextFile( hFind, &wfd ) );

        FindClose( hFind );
    }
}


//////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////

void main( int argc, char **argv )
{
    if ( argc > 1 )
    {
        std::string d3dShaderDir = argv[1];

        std::vector< std::string > shaderFiles;
        CreateFileList( d3dShaderDir, shaderFiles );

        size_t numShaderFiles = shaderFiles.size();
        for ( size_t i = 0; i < numShaderFiles; ++i )
        {
            std::string currentShaderFile = d3dShaderDir;
            currentShaderFile += "\\";
            currentShaderFile += shaderFiles[i];

            FILE* file = NULL;
            fopen_s( &file, currentShaderFile.c_str(), "rb" );

            fseek( file, 0L, SEEK_END );
            int fileSize = ftell( file );
            fseek( file, 0L, SEEK_SET);

            char* dataBuffer = new char[fileSize];	
            fread( dataBuffer, sizeof( char ), fileSize, file );
            fclose( file );

            ID3D11ShaderReflection* d3d11Reflector;
            D3DReflect( dataBuffer, fileSize, IID_ID3D11ShaderReflection, (void**) &d3d11Reflector);
            delete [] dataBuffer;

            FILE* outputFile = NULL;
            currentShaderFile += ".desc";
            fopen_s( &outputFile, currentShaderFile.c_str(), "w+" );

            Json::Value root;            

            D3D11_SHADER_DESC desc;
            d3d11Reflector->GetDesc( &desc );
            for ( unsigned int i = 0; i < desc.ConstantBuffers; ++i )
            {
                ID3D11ShaderReflectionConstantBuffer* constantBuf = d3d11Reflector->GetConstantBufferByIndex( i );
                D3D11_SHADER_BUFFER_DESC constantBufferDesc;
                constantBuf->GetDesc( &constantBufferDesc );

                for ( unsigned int v = 0; v < constantBufferDesc.Variables; ++v )
                {
                    ID3D11ShaderReflectionVariable* var = constantBuf->GetVariableByIndex( v );
                    D3D11_SHADER_VARIABLE_DESC varDesc;
                    var->GetDesc( &varDesc );
                                        
                    Json::Value constantBuffer;
                    constantBuffer["ConstantName"] = varDesc.Name;
                    constantBuffer["ConstantStartOffset"] = varDesc.StartOffset;
                    constantBuffer["ConstantSize"] = varDesc.Size;
                    root["ConstantBuffer"].append( constantBuffer );
                }
            }

            for ( unsigned int i = 0; i < desc.BoundResources; ++i )
            {
                D3D11_SHADER_INPUT_BIND_DESC boundDesc;
                d3d11Reflector->GetResourceBindingDesc( i, &boundDesc );

                switch( boundDesc.Type )
                {
                case D3D_SIT_TEXTURE:
                    {
                        Json::Value textureResource;
                        textureResource["ResourceName"] = boundDesc.Name;
                        textureResource["ResourceBindPoint"] = boundDesc.BindPoint;
                        root["TextureResource"].append( textureResource );
                    }
                    break;
                default:
                    break;
                }
            }

            Json::StyledWriter writer;
            std::string outputConfig = writer.write( root );

            fprintf( outputFile, "%s\n", outputConfig.c_str() );

            fclose( outputFile );

            d3d11Reflector->Release();
        }        
    }
}