#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogl/ogltexture.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/system/pathutilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLTexture::OGLTexture()
{

}

//============================================================================

OGLTexture::~OGLTexture()
{
	OGL::oglDeleteTextures( 1, &mID );
}

//===========================================================================

void OGLTexture::LoadTextureImage( const char* filename, TextureFileType mTextureFileType )
{
	char fullPathFilename[256];
	System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( fullPathFilename, 256, filename );

	if ( mTextureFileType == TEXTURE_FILE_NORMAL)
	{
		System::StringConcat( fullPathFilename, 256, ".dds" );
	}
	else if ( mTextureFileType == TEXTURE_FILE_HDR )
	{
		System::StringConcat( fullPathFilename, 256, ".hdr" );
	}

	SetName( System::Path::SystemSlash( filename ) );

	Image image;
	image.Load( fullPathFilename, true );
	
	Image::MipMapData largestMipMap = image.GetMipMapData( 0 );

	mWidth = largestMipMap.mWidth;
	mHeight = largestMipMap.mHeight;

	OGL::oglGenTextures( 1, &mID );
	OGL::oglBindTexture( GL_TEXTURE_2D, mID );

	GLint pixelFormat = GL_RGB;
	GLint internalFormat = GL_RGB;
	GLenum dataType = GL_UNSIGNED_BYTE;
	if ( image.GetImageDataType() == Image::IMAGE_DATA_TYPE_FLOAT )
	{
		dataType = GL_FLOAT;
	}

	switch ( image.GetPixelFormat() )
	{
	case Image::PF_RGB:
		pixelFormat = GL_RGB;
		internalFormat = GL_RGB;
		break;
	case Image::PF_RGBA:
		pixelFormat = GL_RGBA;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_BGRA:
		pixelFormat = GL_BGRA;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_COMPRESSED_DXT1:
		pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_COMPRESSED_DXT3:
		pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_COMPRESSED_DXT5:
		pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		internalFormat = GL_RGBA;
		break;
	default:
		break;
	}

	if ( 
		image.GetPixelFormat() == Image::PF_COMPRESSED_DXT1 ||
		image.GetPixelFormat() == Image::PF_COMPRESSED_DXT3 ||
		image.GetPixelFormat() == Image::PF_COMPRESSED_DXT5
		)
	{	
		for ( int i = 0; i < image.GetNumMipMaps(); ++i )
		{
			Image::MipMapData mipMapData = image.GetMipMapData( i );
			OGL::oglCompressedTexImage2D(
				GL_TEXTURE_2D,
				i,
				pixelFormat,
				mipMapData.mWidth,
				mipMapData.mHeight,
				0,
				mipMapData.mImageDataSize,
				mipMapData.mImageData
				);
		}
	}
	else
	{
		OGL::oglTexImage2D( 
			GL_TEXTURE_2D, 
			0, 
			internalFormat, 
			mWidth, 
			mHeight, 
			0, 
			pixelFormat, 
			dataType, 
			largestMipMap.mImageData 
			);	
	}
}

//============================================================================

void OGLTexture::CreateTextureWithImage( const Image &image )
{
	Image::MipMapData largestMipMap = image.GetMipMapData( 0 );

	mWidth = largestMipMap.mWidth;
	mHeight = largestMipMap.mHeight;

	OGL::oglGenTextures( 1, &mID );
	OGL::oglBindTexture( GL_TEXTURE_2D, mID );

	GLint pixelFormat = GL_RGB;
	GLint internalFormat = GL_RGB;
	GLenum dataType = GL_UNSIGNED_BYTE;
	if ( image.GetImageDataType() == Image::IMAGE_DATA_TYPE_FLOAT )
	{
		dataType = GL_FLOAT;
	}

	switch ( image.GetPixelFormat() )
	{
	case Image::PF_RGB:
		pixelFormat = GL_RGB;
		internalFormat = GL_RGB;
		break;
	case Image::PF_RGBA:
		pixelFormat = GL_RGBA;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_BGRA:
		pixelFormat = GL_BGRA;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_COMPRESSED_DXT1:
		pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_COMPRESSED_DXT3:
		pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		internalFormat = GL_RGBA;
		break;
	case Image::PF_COMPRESSED_DXT5:
		pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		internalFormat = GL_RGBA;
		break;
	default:
		break;
	}

	if ( 
		image.GetPixelFormat() == Image::PF_COMPRESSED_DXT1 ||
		image.GetPixelFormat() == Image::PF_COMPRESSED_DXT3 ||
		image.GetPixelFormat() == Image::PF_COMPRESSED_DXT5
		)
	{
		OGL::oglCompressedTexImage2D(
			GL_TEXTURE_2D,
			0,
			pixelFormat,
			mWidth,
			mHeight,
			0,
			largestMipMap.mImageDataSize,
			largestMipMap.mImageData
			);
	}
	else
	{
		OGL::oglTexImage2D( 
			GL_TEXTURE_2D, 
			0, 
			internalFormat, 
			mWidth, 
			mHeight, 
			0, 
			pixelFormat, 
			dataType, 
			largestMipMap.mImageData
			);	
	}
}

//===========================================================================

void OGLTexture::LoadTextureCubeImage( const char* filename )
{	
	char cubeExt[6][4] =
	{
		"_px",
		"_nx",
		"_py",
		"_ny",
		"_pz",
		"_nz"
	};

	mTextureType = TEXTURE_TYPE_CUBEMAP;

	OGL::oglGenTextures( 1, &mID );
	OGL::oglBindTexture( GL_TEXTURE_CUBE_MAP, mID );	

	for ( int i = 0; i < 6; ++i )
	{
		char fullPathFilename[256];
		System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
		System::StringConcat( fullPathFilename, 256, filename );
		System::StringConcat( fullPathFilename, 256, cubeExt[i] );
		System::StringConcat( fullPathFilename, 256, ".dds" );		
		
		Image image;
		image.Load( fullPathFilename );

		Image::MipMapData data = image.GetMipMapData( 0 );
		mWidth = data.mWidth;
		mHeight = data.mHeight;

		GLint pixelFormat = GL_RGB;
		GLint internalFormat = GL_RGB;
		GLenum dataType = GL_UNSIGNED_BYTE;
		switch ( image.GetPixelFormat() )
		{
		case Image::PF_RGB:
			pixelFormat = GL_RGB;
			internalFormat = GL_RGB;
			break;
		case Image::PF_RGBA:
			pixelFormat = GL_RGBA;
			internalFormat = GL_RGBA;
			break;
		case Image::PF_BGRA:
			pixelFormat = GL_BGRA;
			internalFormat = GL_RGBA;
			break;
		case Image::PF_COMPRESSED_DXT1:
			pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			internalFormat = GL_RGBA;
			break;
		case Image::PF_COMPRESSED_DXT3:
			pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			internalFormat = GL_RGBA;
			break;
		case Image::PF_COMPRESSED_DXT5:
			pixelFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			internalFormat = GL_RGBA;
			break;
		default:
			break;
		}

		if ( 
			image.GetPixelFormat() == Image::PF_COMPRESSED_DXT1 ||
			image.GetPixelFormat() == Image::PF_COMPRESSED_DXT3 ||
			image.GetPixelFormat() == Image::PF_COMPRESSED_DXT5
			)
		{
			OGL::oglCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, pixelFormat, mWidth, mHeight, 0, data.mImageDataSize, data.mImageData );
		}
		else
		{
			OGL::oglTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, mWidth, mHeight, 0, pixelFormat, dataType, data.mImageData );
		}
	}
}

//============================================================================

void OGLTexture::CreateTexture( int width, int height, TextureFormat textureFormat )
{
	mWidth = width;
	mHeight = height;
	OGL::oglGenTextures(1, &mID );
	OGL::oglBindTexture( GL_TEXTURE_2D, mID );

	GLenum oglTextureFormat = GL_RGBA8;
	GLenum oglDataFormat = GL_RGBA;
	GLenum oglDataSizeType = GL_UNSIGNED_BYTE;

	switch ( textureFormat )
	{
	case TEXTURE_FORMAT_RGBA8:
		oglTextureFormat = GL_RGBA8;
		oglDataFormat = GL_RGBA;
		oglDataSizeType = GL_UNSIGNED_BYTE;
		break;
	case TEXTURE_FORMAT_RGBA16:
		oglTextureFormat = GL_RGBA16F_ARB;
		oglDataFormat = GL_RGBA;
		oglDataSizeType = GL_FLOAT;
		break;
	case TEXTURE_FORMAT_RGBA32:
		oglTextureFormat = GL_RGBA32F_ARB;
		oglDataFormat = GL_RGBA;
		oglDataSizeType = GL_FLOAT;
		break;
	case TEXTURE_FORMAT_DEPTH:
		oglTextureFormat = GL_DEPTH_COMPONENT24;
		oglDataFormat = GL_DEPTH_COMPONENT;		
		oglDataSizeType = GL_UNSIGNED_BYTE;
		break;
	case TEXTURE_FORMAT_RG32:
		oglTextureFormat = GL_RG32F;
		oglDataFormat = GL_RGBA;
		oglDataSizeType = GL_FLOAT;
		break;
	default:
		Assert( false, "Cannot find format." );
		break;
	}

	OGL::oglTexImage2D( 
		GL_TEXTURE_2D, 
		0, 
		oglTextureFormat,  
		width, 
		height, 
		0, 
		oglDataFormat, 
		oglDataSizeType, 
		NULL
		);
}

#endif