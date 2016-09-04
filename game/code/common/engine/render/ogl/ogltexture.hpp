#if defined( RENDER_PLAT_OPENGL )

#ifndef OGLTEXTURE_HPP
#define OGLTEXTURE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/image.hpp"
#include "common/engine/render/ogl/oglrenderer.hpp"
#include "common/engine/render/texture.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLTexture : public Texture
{
public:

    OGLTexture();
    ~OGLTexture();

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