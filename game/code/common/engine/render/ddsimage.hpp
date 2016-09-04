#ifndef DDS_IMAGE_HPP
#define DDS_IMAGE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/image.hpp"

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class DDSImage
{
public:
	DDSImage();
	~DDSImage();

	void Load( const char* filename, bool flip );

	int GetNumMipMaps()
	{
		return mNumMipMaps;
	}

	Image::MipMapData GetMipMapData( int mipMapIndex ) const
	{
		return mMipMaps[mipMapIndex];
	}

	Image::PixelFormatType GetImageFormatType() const
	{
		return mPixelFormatType;
	}

	Image::ImageDataType GetImageDataType() const
	{
		return mImageDataType;
	}

private:

	static void FlipDXT1BlockFull( unsigned char* data );
	static void FlipDXT3BlockFull( unsigned char* block );
	static void FlipDXT5BlockFull( unsigned char* block );

	Image::PixelFormatType mPixelFormatType;
	int mNumMipMaps;
	Image::MipMapData* mMipMaps;
	Image::ImageDataType mImageDataType;
};

#endif
