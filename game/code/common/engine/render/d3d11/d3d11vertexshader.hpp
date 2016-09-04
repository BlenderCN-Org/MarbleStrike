#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_VERTEX_SHADER_HPP
#define D3D11_VERTEX_SHADER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d11/d3d11shaderprogram.hpp"
#include "common/engine/render/vertexshader.hpp"
#include <d3d11.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11VertexShader : public VertexShader
{
public:

	D3D11VertexShader();
	~D3D11VertexShader();

	void Load( const char* path, const char* name );
	void Reload();
	void Dispatch( void * context = NULL );

    ConstantList* GetConstantList()
    {
        return &mVertexConstantList;
    }

    int GetBufferSize()
    {
        return mVertexBufferSize;
    }

    char* GetShaderByteCode()
    {
        return mShaderByteCode;
    }

    int GetShaderByteCodeSize()
    {
        return mShaderByteCodeSize;
    }
		
private:
    	
	ID3D11VertexShader* mVertexShader;

	ConstantList mVertexConstantList;
    int mVertexBufferSize;

    TextureList mVertexTextureList;

    char* mShaderByteCode;
    int mShaderByteCodeSize;

};

#endif

#endif