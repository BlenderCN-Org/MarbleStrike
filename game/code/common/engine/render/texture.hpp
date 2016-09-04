#ifndef TEXTURE_HPP
#define TEXTURE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/stringutilities.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Image;

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

const int MAX_TEXTURE_NAME = 256;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum TextureType
{
	TEXTURE_TYPE_2D,
	TEXTURE_TYPE_CUBEMAP
};

enum TextureFileType
{
	TEXTURE_FILE_NORMAL,
	TEXTURE_FILE_HDR
};

enum TextureFormat
{
	TEXTURE_FORMAT_RGBA8,
	TEXTURE_FORMAT_RGBA16,
	TEXTURE_FORMAT_RGBA32,
	TEXTURE_FORMAT_RG32,
    TEXTURE_FORMAT_R32,
    TEXTURE_FORMAT_DXT1,
    TEXTURE_FORMAT_DXT3,
    TEXTURE_FORMAT_DXT5,
	TEXTURE_FORMAT_DEPTH
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Texture
{
public:

    Texture();
	virtual ~Texture();

	virtual void CreateTextureWithImage( const Image &image ) = 0;	
    virtual void CreateTexture( int width, int height, TextureFormat textureFormat ) = 0;    
		
    virtual void LoadTextureImage( const char* filename, TextureFileType mTextureFileType = TEXTURE_FILE_NORMAL ) = 0;    
	virtual void LoadTextureCubeImage( const char* filename ) = 0;

	void SetName( const char* name )
	{
		System::StringCopy( mName, MAX_TEXTURE_NAME, name );
	}

	const char* GetName() const
	{
		return mName;
	}

	int GetWidth() const
	{
		return mWidth;
	}

	int GetHeight() const
	{
		return mHeight;
	}

	TextureType GetTextureType() const
	{
		return mTextureType;
	}

protected:
	int mWidth;
	int mHeight;
	TextureType mTextureType;

private:

	char mName[MAX_TEXTURE_NAME];		
};

#endif
