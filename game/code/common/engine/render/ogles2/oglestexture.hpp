#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLTEXTURE_HPP
#define OGLTEXTURE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/image.hpp"
#include "common/engine/render/ogles2/oglesrenderer.hpp"
#include "common/engine/render/texture.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLESTexture : public Texture
{
public:

    OGLESTexture();
    ~OGLESTexture();
		
    void CreateTextureWithImage( const Image &image );    	
    void CreateTexture( int width, int height, TextureFormat textureFormat );

	void LoadTextureImage( const char* filename, TextureFileType mTextureFileType = TEXTURE_FILE_NORMAL );
	void LoadTextureCubeImage( const char* filename );
    
	int GetHandle() const
	{
		return mID;
	}

    void SetID( unsigned int id )
    {
        mID = id;
    }

private:

    unsigned int mID;
};

#endif

#endif