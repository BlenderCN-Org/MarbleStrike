#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11TEXTURE_HPP
#define D3D11TEXTURE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/texture.hpp"

#include <d3d11.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11Texture : public Texture
{
public:

	D3D11Texture();
	~D3D11Texture();

	void CreateTextureWithImage( const Image &image );	
	void CreateTexture( int width, int height, TextureFormat textureFormat );
	
	void LoadTextureImage( const char* filename, TextureFileType mTextureFileType = TEXTURE_FILE_NORMAL );
	void LoadTextureCubeImage( const char* filename );

    ID3D11ShaderResourceView* GetHandle() const
    {
        return mShaderResourceView;
    }

	ID3D11Texture2D* GetD3D11Texture()
	{
		return mTexture;
	}

private:

	DXGI_FORMAT GetD3D11TextureFormat( TextureFormat textureFormat );
	DXGI_FORMAT GetD3D11SRVFormat( TextureFormat textureFormat );

	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mShaderResourceView;
     
};

#endif

#endif