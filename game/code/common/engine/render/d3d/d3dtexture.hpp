#if defined( RENDER_PLAT_D3D9 )

#ifndef D3DTEXTURE_HPP
#define D3DTEXTURE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/texture.hpp"

#include <d3d9.h>
#include <d3dx9.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3DTexture : public Texture
{
public:

	D3DTexture();
	~D3DTexture();

	void CreateTextureWithImage( const Image &image );	
	void CreateTexture( int width, int height, TextureFormat textureFormat );

	void LoadTextureImage( const char* filename, TextureFileType mTextureFileType = TEXTURE_FILE_NORMAL );  
	void LoadTextureCubeImage( const char* filename );

    void* GetHandle() const
    {
        return mTexture;
    }

private:

	void* mTexture;
     
};

#endif

#endif