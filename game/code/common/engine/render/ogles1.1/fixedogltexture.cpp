#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "iphone/render/ogles1.1/fixedogltexture.hpp"
#include "common/engine/system/pathutilities.hpp"
#if defined PLAT_IOS
#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#else
#include "PVRTglesExt.h"
#endif

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLTexture::FixedOGLTexture()
:	mIsSamplerSet( false )
{

}

//============================================================================

FixedOGLTexture::~FixedOGLTexture()
{
	FixedOGL::oglDeleteTextures( 1, &mID );
}

//===========================================================================

void FixedOGLTexture::LoadTextureImage( const char* filename, TextureFileType mTextureFileType )
{
	UNUSED_ALWAYS( mTextureFileType );

	const char* resourcePath = Database::Get()->GetResourcePath();
	
	char fullPathFilename[256];
	System::StringCopy( fullPathFilename, 256, resourcePath );
	System::StringConcat( fullPathFilename, 256, filename );
	System::StringConcat( fullPathFilename, 256, ".pvr" );
	
	System::FixedString<256> loadPath = System::Path::SystemSlash( fullPathFilename );
	
	SetName( filename );

	Image image;
	image.Load( loadPath );

	Image::MipMapData mipMapData = image.GetMipMapData( 0 );

	mWidth = mipMapData.mWidth;
	mHeight = mipMapData.mHeight;

	FixedOGL::oglGenTextures( 1, &mID );
	FixedOGL::oglBindTexture( GL_TEXTURE_2D, mID );

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
		FixedOGL::oglCompressedTexImage2D(
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
		FixedOGL::oglTexImage2D( 
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

//============================================================================

void FixedOGLTexture::CreateTextureWithImage( const Image &image )
{
	Image::MipMapData mipMapData = image.GetMipMapData( 0 );

	mWidth = mipMapData.mWidth;
	mHeight = mipMapData.mHeight;

	FixedOGL::oglGenTextures( 1, &mID );
	FixedOGL::oglBindTexture( GL_TEXTURE_2D, mID );

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
		FixedOGL::oglCompressedTexImage2D(
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
		FixedOGL::oglTexImage2D( 
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

void FixedOGLTexture::LoadTextureCubeImage( const char* filename )
{
	UNUSED_ALWAYS( filename );
}

//============================================================================

void FixedOGLTexture::CreateTexture( int width, int height, TextureFormat textureFormat )
{	
	mWidth = width;
	mHeight = height;
	FixedOGL::oglGenTextures(1, &mID );
	FixedOGL::oglBindTexture( GL_TEXTURE_2D, mID );

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
	default:
		Assert( false, "Cannot find format." );
		break;
	}

	FixedOGL::oglTexImage2D( 
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

//============================================================================

void FixedOGLTexture::Bind( int index ) const
{	
	FixedOGL::oglActiveTexture( GL_TEXTURE0 + index );
	FixedOGL::oglBindTexture( GL_TEXTURE_2D, mID );
}

#endif