#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_OGLTEXTURE_HPP
#define FIXED_OGLTEXTURE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/texture.hpp"
#include "common/engine/render/image.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderer.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class FixedOGLTexture : public Texture
{
public:

    FixedOGLTexture();
    ~FixedOGLTexture();

	void CreateTextureWithImage( const Image &image );	
	void CreateTexture( int width, int height, TextureFormat textureFormat );    
	void LoadTextureImage( const char* filename, TextureFileType mTextureFileType = TEXTURE_FILE_NORMAL );
	void LoadTextureCubeImage( const char* fileaname );

    void Bind( int index ) const;
	
	bool IsSamplerSet() const
	{
		return mIsSamplerSet;
	}

	void SetSamplerSet( bool value )
	{
		mIsSamplerSet = value;
	}

	int GetHandle()
	{
		return mID;
	}

    void SetID( unsigned int id )
    {
        mID = id;
    }
	
private:

    unsigned int mID;
	bool mIsSamplerSet;
};

#endif

#endif