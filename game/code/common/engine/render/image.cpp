//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/image.hpp"
#include "common/engine/database/database.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/render/bmpimage.hpp"
#include "common/engine/render/ddsimage.hpp"
#include "common/engine/render/rgbeimage.hpp"
#include <string.h>

#define PVR_TEXTURE_FLAG_TYPE_MASK    0xff

struct PVRTexHeader
{
	unsigned int headerLength;
	unsigned int height;
	unsigned int width;
	unsigned int numMipmaps;
	unsigned int flags;
	unsigned int dataLength;
	unsigned int bpp;
	unsigned int bitmaskRed;
	unsigned int bitmaskGreen;
	unsigned int bitmaskBlue;
	unsigned int bitmaskAlpha;
	unsigned int pvrTag;
	unsigned int numSurfs;
};

enum PVRTextureFlag
{
	kPVRTextureFlagTypeRGBA8888 = 18,
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4 = 13
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Image::Image() 
:	mPixelFormat( PF_NONE )
,	mMipMaps( NULL )
,	mNumMipMaps( 0 )
,	mImageDataType( IMAGE_DATA_TYPE_UNSIGNED_BYTE )
{    
}

//===========================================================================

Image::~Image()
{
	ClearImageData();
}

//===========================================================================

void Image::Load( const char* filename, bool flip )
{
	ClearImageData();

	ImageFileFormatType fileFormat = ParseImageExtension( filename );

	if ( fileFormat == FORMAT_DDS )
	{
		LoadDDSFile( filename, flip );
		return;
	}
	else if ( fileFormat == FORMAT_PVR )
	{
		LoadPVRFile( filename );
		return;
	}
	else if ( fileFormat == FORMAT_BMP )
	{
		LoadBMPFile( filename, flip );
		return;
	}
	else if ( fileFormat == FORMAT_HDR )
	{
		LoadHDRFile( filename, flip );
		return;
	}

#if defined PLAT_PC
#if defined PLAT_PC
	FREE_IMAGE_FORMAT format = FIF_UNKNOWN;
	switch ( fileFormat )
	{
	case FORMAT_GIF:
		format = FIF_GIF;
		break;
	case FORMAT_JPEG:
		format = FIF_JPEG;
		break;
	case FORMAT_TARGA:
		format = FIF_TARGA;
		break;
	case FORMAT_TIFF:
		format = FIF_TIFF;
		break;
	default:
		break;
	}

	FIBITMAP* freeImageData = FreeImage_Load( format, filename, BMP_DEFAULT );
	Assert( freeImageData != NULL, "Error loading image.\n" );

	mNumMipMaps = 1;
	mMipMaps = NEW_PTR( "Mip Maps" ) MipMapData[mNumMipMaps];
	MipMapData* largestMipMap = &mMipMaps[0];

	largestMipMap->mWidth = FreeImage_GetWidth( freeImageData );
	largestMipMap->mHeight = FreeImage_GetHeight( freeImageData );

	BYTE* pixels = FreeImage_GetBits( freeImageData );        
	int bbp = FreeImage_GetBPP( freeImageData );
	int bitsPerPixel = bbp / 8;

	int size = largestMipMap->mWidth * largestMipMap->mHeight * bitsPerPixel;
	largestMipMap->mImageData = NEW_PTR( "Image Data" ) unsigned char[size];
	mImageDataType = IMAGE_DATA_TYPE_UNSIGNED_BYTE;

	unsigned char* imageData = reinterpret_cast<unsigned char*>( largestMipMap->mImageData );

	switch ( bbp ) 
	{
	case 8:
		{                
			for ( int i = 0; i < size; i += bitsPerPixel ) 
			{
				imageData[i] = static_cast<unsigned char>( pixels[i] );
			}
		}
		break;
	case 24:
		{
			for ( int i = 0; i < size; i += bitsPerPixel ) 
			{
				imageData[i] = static_cast<unsigned char>( pixels[FI_RGBA_RED] );
				imageData[i+1] = static_cast<unsigned char>( pixels[FI_RGBA_GREEN] );
				imageData[i+2] = static_cast<unsigned char>( pixels[FI_RGBA_BLUE] );

				pixels += bitsPerPixel;
			}

			mPixelFormat = PF_RGB;
		}            
		break;
	case 32:
		{
			for ( int i = 0; i < size; i += bitsPerPixel ) 
			{
				imageData[i] = static_cast<unsigned char>( pixels[FI_RGBA_RED] );
				imageData[i+1] = static_cast<unsigned char>( pixels[FI_RGBA_GREEN] );
				imageData[i+2] = static_cast<unsigned char>( pixels[FI_RGBA_BLUE] );
				imageData[i+3] = static_cast<unsigned char>( pixels[FI_RGBA_ALPHA] );

				pixels += bitsPerPixel;
			}

			mPixelFormat = PF_RGBA;
		}            
		break;
	default:
		break;
	}

	FreeImage_Unload( freeImageData );
#endif
#endif
}

//===========================================================================

void Image::LoadDDSFile( const char* filename, bool flip )
{
	DDSImage ddsImage;
	ddsImage.Load( filename, flip );

	mPixelFormat = ddsImage.GetImageFormatType();
	mNumMipMaps = ddsImage.GetNumMipMaps();
	mMipMaps = NEW_PTR( "Mip Maps" ) MipMapData[mNumMipMaps];

	for ( int i = 0; i < mNumMipMaps; ++i )
	{
		MipMapData ddsMipMap = ddsImage.GetMipMapData( i );

		mMipMaps[i].mWidth = ddsMipMap.mWidth;
		mMipMaps[i].mHeight = ddsMipMap.mHeight;
		mMipMaps[i].mImageDataSize = ddsMipMap.mImageDataSize;
		mMipMaps[i].mImageData = NEW_PTR( "DDS Image" ) unsigned char[ddsMipMap.mImageDataSize];
		memcpy( mMipMaps[i].mImageData, ddsMipMap.mImageData, ddsMipMap.mImageDataSize );		
	}
}

//===========================================================================

void Image::LoadPVRFile( const char* filename )
{
	char filenameFullPath[256];
	System::StringCopy( filenameFullPath, 256, "" );
	
	System::StringConcat( filenameFullPath, 256, filename );
	
	System::FixedString<256> systemFullPathName = System::Path::SystemSlash( filenameFullPath );
	
	FILE* file = System::OpenFile( systemFullPathName, "rb" );

	std::string assertMsg;
	assertMsg = "Filename: ";
	assertMsg += systemFullPathName.GetRawData();	
	Assert( file != NULL, assertMsg.c_str() );

	PVRTexHeader header;
	fread( &header, sizeof ( PVRTexHeader ), 1, file );

	static char PVRTexIdentifier[5] = "PVR!";
	if (
		PVRTexIdentifier[0] != static_cast<char>( ( header.pvrTag >>  0 ) & 0xff ) ||
		PVRTexIdentifier[1] != static_cast<char>( ( header.pvrTag >>  8 ) & 0xff ) ||
		PVRTexIdentifier[2] != static_cast<char>( ( header.pvrTag >> 16 ) & 0xff ) ||
		PVRTexIdentifier[3] != static_cast<char>( ( header.pvrTag >> 24 ) & 0xff ) )
	{
		Assert( false, "" );
	}

	unsigned int formatFlags;
	formatFlags = header.flags & PVR_TEXTURE_FLAG_TYPE_MASK;

	switch( formatFlags )
	{
	case kPVRTextureFlagTypeRGBA8888:
		mPixelFormat = PF_RGBA;
		break;
	case kPVRTextureFlagTypePVRTC_4:
		mPixelFormat = PF_PVRTC_4BPPV1;
		break;
	case kPVRTextureFlagTypePVRTC_2:
		mPixelFormat = PF_PVRTC_2BPPV1;
		break;
	default:
		break;
	}
	
	int blockSize = 0;
	int widthBlocks = 0;
	int heightBlocks = 0;
	int bpp = 0;
	int width = header.width;
	int height = header.height;

	switch( formatFlags )
	{
	case kPVRTextureFlagTypeRGBA8888:
		blockSize = 1;
		widthBlocks = width;
		heightBlocks = height;
		bpp = 32;
		break;
	case kPVRTextureFlagTypePVRTC_4:
		blockSize = 4 * 4;
		widthBlocks = width / 4;
		heightBlocks = height / 4;
		bpp = 4;
		break;
	case kPVRTextureFlagTypePVRTC_2:
		blockSize = 8 * 4;
		widthBlocks = width / 8;
		heightBlocks = height / 4;
		bpp = 2;
		break;
	default:
		break;
	}

	mNumMipMaps = 1;
	mMipMaps = NEW_PTR( "Mip Maps" ) MipMapData[mNumMipMaps];
	MipMapData* largestMipMap = &mMipMaps[0];

	largestMipMap->mWidth = width;
	largestMipMap->mHeight = height;

	// Clamp to minimum number of blocks
	if ( widthBlocks < 2 )
	{
		widthBlocks = 2;
	}

	if ( heightBlocks < 2 )
	{
		heightBlocks = 2;
	}

	int dataSize = widthBlocks * heightBlocks * ( ( blockSize  * bpp ) / 8 );

	largestMipMap->mImageDataSize = dataSize;

	largestMipMap->mImageData = NEW_PTR( "PVR Image" ) unsigned char[dataSize];
	mImageDataType = IMAGE_DATA_TYPE_UNSIGNED_BYTE;
	fread( largestMipMap->mImageData, sizeof( unsigned char ), dataSize, file );

	fclose( file );
	file = NULL;
}

//===========================================================================

void Image::LoadBMPFile( const char* filename, bool flip )
{
	char filenameFullPath[256];
	System::StringCopy( filenameFullPath, 256, "" );

#if defined PLAT_IOS
	System::StringCopy( filenameFullPath, 256, Database::Get()->GetResourcePath() );
#endif
	
	System::StringConcat( filenameFullPath, 256, filename );

	BMPImage bmpImage;
	bmpImage.Load( filenameFullPath, flip );

	mNumMipMaps = 1;
	mMipMaps = NEW_PTR( "Mip Maps" ) MipMapData[mNumMipMaps];
	MipMapData* largestMipMap = &mMipMaps[0];

	largestMipMap->mWidth = bmpImage.GetWidth();
	largestMipMap->mHeight = bmpImage.GetHeight();

	int bitsPerPixel = bmpImage.GetBitsPerPixel();
	int numBytesPerPixel = bitsPerPixel / 8;
	int size = largestMipMap->mWidth * largestMipMap->mHeight * numBytesPerPixel;

	largestMipMap->mImageData = NEW_PTR( "BMP Image" ) unsigned char[size];
	mImageDataType = IMAGE_DATA_TYPE_UNSIGNED_BYTE;

	memcpy( largestMipMap->mImageData, bmpImage.GetImageData(), size );

	if ( bitsPerPixel == 32 )
	{
		mPixelFormat = PF_RGBA;
	}
	else
	{
		mPixelFormat = PF_RGB;
	}

	largestMipMap->mImageDataSize = size;
}

//===========================================================================

void Image::LoadHDRFile( const char* filename, bool /*flip*/ )
{
	char filenameFullPath[256];
	System::StringCopy( filenameFullPath, 256, "" );

#if defined PLAT_IOS
	System::StringCopy( filenameFullPath, 256, Database::Get()->GetResourcePath() );
#endif

	System::StringConcat( filenameFullPath, 256, filename );

	RGBEImage rgbeImage;
	rgbeImage.Load( filenameFullPath );

	rgbeImage.SplitCubeMap();
	RGBEImage::CubeMapFaceImage faceImage = rgbeImage.GetCubeMapImageData( RGBEImage::CUBE_MAP_FACE_NEG_Z );
	int width = faceImage.mWidth;
	int height = faceImage.mHeight;
	//int width = rgbeImage.GetWidth();
	//int height = rgbeImage.GetHeight();
	float* hdrImageData = faceImage.mImageData;
	//float* hdrImageData = rgbeImage.GetImageData();

	mNumMipMaps = 1;
	mMipMaps = NEW_PTR( "Mip Maps" ) MipMapData[mNumMipMaps];
	MipMapData* largestMipMap = &mMipMaps[0];

	largestMipMap->mWidth = width;
	largestMipMap->mHeight = height;

	int numColorElements = 3;
	int size = largestMipMap->mWidth * largestMipMap->mHeight * numColorElements;
	largestMipMap->mImageData = NEW_PTR( "HDR Image" ) float[size];
	float* imageData = reinterpret_cast<float*>( largestMipMap->mImageData );
	memcpy( imageData, hdrImageData, size * sizeof( float ) );
	mImageDataType = IMAGE_DATA_TYPE_FLOAT;
	mPixelFormat = PF_RGB;

	largestMipMap->mImageDataSize = size * sizeof ( float );
}

//===========================================================================

void Image::CreateImage( 
				 unsigned char* imageData,
				 PixelFormatType pixelFormat,
				 int width,
				 int height,
				 int imageDataSize
				 )
{
	Assert( imageData != NULL, "" );
	Assert( imageDataSize > 0, "" );

	mNumMipMaps = 1;
	mMipMaps = NEW_PTR( "Mip Maps" ) MipMapData[mNumMipMaps];
	MipMapData* largestMipMap = &mMipMaps[0];

	largestMipMap->mImageDataSize = imageDataSize;
	largestMipMap->mImageData = NEW_PTR( "Created Image" ) unsigned char[largestMipMap->mImageDataSize];
	mImageDataType = IMAGE_DATA_TYPE_UNSIGNED_BYTE;
	memcpy(largestMipMap->mImageData, imageData, largestMipMap->mImageDataSize );

	mPixelFormat = pixelFormat;
	largestMipMap->mWidth = width;
	largestMipMap->mHeight = height;
}

//===========================================================================

Image::ImageFileFormatType Image::ParseImageExtension( const char* filename ) const
{
	int stringLength = static_cast<int>( strlen( filename ) );

	char extension[5];

	int i = 0;
	do
	{
		++i;
		Assert( i <= stringLength, "" );
	}
	while ( filename[stringLength-i] != '.' );

	for ( int j = 0; j < i; ++j )
	{
		extension[j] = filename[stringLength-i+j+1];
	}

	Image::ImageFileFormatType format = FORMAT_UNKNOWN;

	if ( strcmp( extension, "gif" ) == 0 ) 
	{
		format = FORMAT_GIF;
	}
	else if ( strcmp( extension, "jpg" ) == 0 )
	{
		format = FORMAT_JPEG;
	}
	else if ( strcmp( extension, "tga" ) == 0 )
	{
		format = FORMAT_TARGA;
	}
	else if ( strcmp( extension, "dds" ) == 0 )
	{
		format = FORMAT_DDS;
	}
	else if ( strcmp( extension, "tif" ) == 0 || strcmp( extension, "tiff" ) == 0 )
	{
		format = FORMAT_TIFF;
	}
	else if ( strcmp( extension, "pvr" ) == 0 )
	{
		format = FORMAT_PVR;
	}
	else if ( strcmp( extension, "bmp" ) == 0 )
	{
		format = FORMAT_BMP;
	}
	else if ( strcmp( extension, "hdr" ) == 0 )
	{
		format = FORMAT_HDR;
	}

	return format;
}

//===========================================================================

void Image::ClearImageData()
{
	for ( int i = 0; i < mNumMipMaps; ++i )
	{
        if ( mImageDataType == IMAGE_DATA_TYPE_UNSIGNED_BYTE )
        {
            unsigned int* data = reinterpret_cast<unsigned int*>( mMipMaps[i].mImageData );
            DELETE_PTR_ARRAY( data );
        }
        else if ( mImageDataType == IMAGE_DATA_TYPE_FLOAT )
        {
            float* data = reinterpret_cast<float*>( mMipMaps[i].mImageData );
            DELETE_PTR_ARRAY( data );
        }
	}
	DELETE_PTR_ARRAY( mMipMaps );
}
