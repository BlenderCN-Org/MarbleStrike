#ifndef IMAGE_HPP
#define IMAGE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#if defined PLAT_PC
#if defined PLAT_PC
#include <windows.h>
#include <FreeImage.h>
#endif
#endif

#include <stdio.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Image
{
public:

	enum PixelFormatType 
	{ 
		PF_NONE = -1,
		PF_RGB, 
		PF_RGBA,
		PF_BGRA,
		PF_PVRTC_4BPPV1,
		PF_PVRTC_2BPPV1,
		PF_COMPRESSED_DXT1,
		PF_COMPRESSED_DXT3,
		PF_COMPRESSED_DXT5,
		PF_MAX_QUANTITY
	};

	enum ImageFileFormatType
	{
		FORMAT_GIF,
		FORMAT_JPEG,		
		FORMAT_TIFF,
		FORMAT_TARGA,
		FORMAT_DDS,
		FORMAT_PVR,
		FORMAT_BMP,
		FORMAT_HDR,
		FORMAT_UNKNOWN
	};

	enum ImageDataType
	{
		IMAGE_DATA_TYPE_UNSIGNED_BYTE,
		IMAGE_DATA_TYPE_FLOAT
	};

	struct MipMapData
	{
		MipMapData()
		{
			mImageData = NULL;
			mWidth = 0;
			mHeight = 0;
			mImageDataSize = 0;
		}

		void* mImageData;
		int mWidth;
		int mHeight;
		int mImageDataSize;
	};

	Image();
	~Image();
	void Load( const char* filename, bool flip = false );
	void LoadDDSFile( const char* filename, bool flip );
	void LoadPVRFile( const char* filename );
	void LoadBMPFile( const char* filename, bool flip );
	void LoadHDRFile( const char* filename, bool flip );

	void CreateImage( 
		unsigned char* imageData,
		PixelFormatType pixelFormat,
		int width,
		int height,
		int imageDataSize
	);

	ImageFileFormatType ParseImageExtension( const char* filename ) const;

	int GetNumMipMaps()
	{
		return mNumMipMaps;
	}

	MipMapData GetMipMapData( int mipMapIndex ) const
	{
		return mMipMaps[mipMapIndex];
	}

	PixelFormatType GetPixelFormat() const
	{
		return mPixelFormat;
	}

	ImageDataType GetImageDataType() const
	{
		return mImageDataType;
	}

private:    

	void ClearImageData();
	
	PixelFormatType mPixelFormat;
	ImageDataType mImageDataType;
	int mNumMipMaps;
	MipMapData* mMipMaps;
};

#endif
