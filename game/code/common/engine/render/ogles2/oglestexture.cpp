#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/ogles2/oglestexture.hpp"
#include "common/engine/system/pathutilities.hpp"

#if defined PLAT_PC
#include "opengles2/include/gles2/gl2extimg.h"
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLESTexture::OGLESTexture()
{

}

//============================================================================

OGLESTexture::~OGLESTexture()
{
	OGLES::oglDeleteTextures( 1, &mID );
}

//===========================================================================

void OGLESTexture::CreateTextureWithImage( const Image &image )
{
	Image::MipMapData mipMapData = image.GetMipMapData( 0 );
	mWidth = mipMapData.mWidth;
	mHeight = mipMapData.mHeight;

	OGLES::oglGenTextures( 1, &mID );
	OGLES::oglBindTexture( GL_TEXTURE_2D, mID );

	GLint pixelFormat = GL_RGB;
	GLint internalFormat = GL_RGB;

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
#if !defined( PLAT_ANDROID )
	case Image::PF_BGRA:
		pixelFormat = GL_BGRA;
		internalFormat = GL_RGBA;
		break;
#endif
	default:
		break;
	}

	if ( image.GetPixelFormat() == Image::PF_PVRTC_4BPPV1 )
	{
		//glCompressedTexImage2D(
		//	GL_TEXTURE_2D,
		//	0,
		//	internalFormat,
		//	image.GetWidth(),
		//	image.GetHeight(),
		//	0,
		//	image.GetSize(),
		//	image.GetImageData()
		//	);
	}
	else
	{
		OGLES::oglTexImage2D( 
			GL_TEXTURE_2D, 
			0, 
			internalFormat, 
			mWidth, 
			mHeight, 
			0, 
			pixelFormat, 
			GL_UNSIGNED_BYTE, 
			mipMapData.mImageData 
			);	
	}
}

//===========================================================================

void OGLESTexture::LoadTextureCubeImage( const char* filename )
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

	OGLES::oglGenTextures( 1, &mID );
	OGLES::oglBindTexture( GL_TEXTURE_CUBE_MAP, mID );	

	for ( int i = 0; i < 6; ++i )
	{
		char fullPathFilename[256];
		System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
		System::StringConcat( fullPathFilename, 256, filename );
		System::StringConcat( fullPathFilename, 256, cubeExt[i] );
		System::StringConcat( fullPathFilename, 256, ".pvr" );		

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
#if !defined( PLAT_ANDROID )
		case Image::PF_BGRA:
			pixelFormat = GL_BGRA;
			internalFormat = GL_RGBA;
			break;
#endif
		case Image::PF_PVRTC_4BPPV1:
			internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
			break;
		case Image::PF_PVRTC_2BPPV1:
			internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
			break;
		default:
			break;
		}

		if ( image.GetPixelFormat() == Image::PF_PVRTC_4BPPV1 || image.GetPixelFormat() == Image::PF_PVRTC_2BPPV1 )
		{
			OGLES::oglCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, mWidth, mHeight, 0, data.mImageDataSize, data.mImageData );
		}
		else
		{
			OGLES::oglTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, mWidth, mHeight, 0, pixelFormat, dataType, data.mImageData );
		}
	}
}

//===========================================================================

void OGLESTexture::LoadTextureImage( const char* filename, TextureFileType /*textureFileType*/ )
{

	char fullPathFilename[256];	
	System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( fullPathFilename, 256, filename );

	System::StringConcat( fullPathFilename, 256, ".pvr" );
	
	SetName( fullPathFilename );

	Image image;
	image.Load( System::Path::SystemSlash( fullPathFilename ) );
	
	Image::MipMapData mipMapData = image.GetMipMapData( 0 );
	mWidth = mipMapData.mWidth;
	mHeight = mipMapData.mHeight;

	OGLES::oglGenTextures( 1, &mID );
	OGLES::oglBindTexture( GL_TEXTURE_2D, mID );

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
	case Image::PF_PVRTC_4BPPV1:
		internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		break;
	case Image::PF_PVRTC_2BPPV1:
		internalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		break;
	default:
		Assert( false, "" );
		break;
	}

	if ( image.GetPixelFormat() == Image::PF_PVRTC_4BPPV1 || image.GetPixelFormat() == Image::PF_PVRTC_2BPPV1 )
	{
		OGLES::oglCompressedTexImage2D(
			GL_TEXTURE_2D,
			0,
			internalFormat,
			mWidth,
			mHeight,
			0,
			mipMapData.mImageDataSize,
			mipMapData.mImageData
			);
	}
	else
	{
		OGLES::oglTexImage2D( 
			GL_TEXTURE_2D, 
			0, 
			internalFormat, 
			mWidth, 
			mHeight, 
			0, 
			pixelFormat, 
			dataType, 
			mipMapData.mImageData
			);	
	}
}

//============================================================================

void OGLESTexture::CreateTexture( int width, int height, TextureFormat textureFormat )
{

	mWidth = width;
	mHeight = height;
	OGLES::oglGenTextures(1, &mID );
	OGLES::oglBindTexture( GL_TEXTURE_2D, mID );

	GLenum oglTextureFormat = GL_RGBA;
	GLenum oglDataFormat = GL_RGBA;
	GLenum oglDataSizeType = GL_UNSIGNED_BYTE;

	switch ( textureFormat )
	{
	case TEXTURE_FORMAT_RGBA8:
		oglTextureFormat = GL_RGBA;
		oglDataFormat = GL_RGBA;
		oglDataSizeType = GL_UNSIGNED_BYTE;
		break;
	case TEXTURE_FORMAT_RGBA16:
		oglTextureFormat = GL_RGBA;
		oglDataFormat = GL_RGBA;
		oglDataSizeType = GL_HALF_FLOAT_OES;
		break;
	case TEXTURE_FORMAT_RGBA32:
		oglTextureFormat = GL_RGBA;
		oglDataFormat = GL_RGBA;
		oglDataSizeType = GL_FLOAT;
		break;
	case TEXTURE_FORMAT_RG32:
		oglTextureFormat = GL_LUMINANCE;
		oglDataFormat = GL_LUMINANCE;
		oglDataSizeType = GL_FLOAT;
		break;
	case TEXTURE_FORMAT_DEPTH:
		oglTextureFormat = GL_DEPTH_COMPONENT;
		oglDataFormat = GL_DEPTH_COMPONENT;		
		oglDataSizeType = GL_UNSIGNED_INT;
		break;
	default:
		Assert( false, "Cannot find format." );
		break;
	}

	OGLES::oglTexImage2D( 
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