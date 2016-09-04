#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_PIXEL_SHADER_HPP
#define D3D11_PIXEL_SHADER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d11/d3d11shaderprogram.hpp"
#include "common/engine/render/pixelshader.hpp"
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

class D3D11PixelShader : public PixelShader
{
public:

    D3D11PixelShader();
    ~D3D11PixelShader();

    void Load( const char* path, const char* name );
    void Reload();
    void Dispatch( void* context );

    ConstantList* GetConstantList()
    {
        return &mPixelConstantList;
    }

    int GetBufferSize()
    {
        return mPixelBufferSize;
    }

    TextureList* GetTextureList()
    {
        return &mPixelTextureList;
    }

private:

    ID3D11PixelShader* mPixelShader;

    ConstantList mPixelConstantList;
    int mPixelBufferSize;

    TextureList mPixelTextureList;
};

#endif

#endif