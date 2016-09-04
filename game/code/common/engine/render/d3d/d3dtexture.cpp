#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3dtexture.hpp"
#include "common/engine/render/image.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DTexture::D3DTexture()
:	mTexture( NULL )
{

}
//============================================================================

D3DTexture::~D3DTexture()
{
	switch ( mTextureType )
	{
	case TEXTURE_TYPE_2D:
		{

			if ( mTexture != NULL )
			{
				LPDIRECT3DTEXTURE9 d3dTexture = reinterpret_cast<LPDIRECT3DTEXTURE9>( mTexture );
				d3dTexture->Release();
				mTexture = NULL;
			}
		}
		break;
	case TEXTURE_TYPE_CUBEMAP:
		{

			if ( mTexture != NULL )
			{
				LPDIRECT3DCUBETEXTURE9 d3dTexture = reinterpret_cast<LPDIRECT3DCUBETEXTURE9>( mTexture );
				d3dTexture->Release();
				mTexture = NULL;
			}
		}
		break;
	default:
		break;
	}
}

//===========================================================================

void D3DTexture::CreateTextureWithImage( const Image &image )
{
	Image::MipMapData largestMipMap = image.GetMipMapData( 0 );

	mWidth = largestMipMap.mWidth;
	mHeight = largestMipMap.mHeight;

	D3DFORMAT d3dFormat = D3DFMT_A8R8G8B8;
	if ( image.GetImageDataType() == Image::IMAGE_DATA_TYPE_FLOAT )
	{
		d3dFormat = D3DFMT_A32B32G32R32F;
	}

	LPDIRECT3DTEXTURE9 d3dTexture = NULL;
	D3DRenderer::GetDevice()->CreateTexture(
		mWidth,
		mHeight,
		1,
		D3DUSAGE_DYNAMIC,
		d3dFormat,
		D3DPOOL_DEFAULT,
		&d3dTexture,
		NULL
		);
	D3DLOCKED_RECT d3dLockedRect;	
	int flags = 0;
	d3dTexture->LockRect( 0, &d3dLockedRect, NULL, flags );

	if ( image.GetImageDataType() == Image::IMAGE_DATA_TYPE_FLOAT )
	{
		int stride = d3dLockedRect.Pitch / sizeof( float );
		float* bytePointer = (float*)d3dLockedRect.pBits;
		float* imageData = (float*)largestMipMap.mImageData;

		for ( int y = 0; y < mHeight; ++y )
		{
			for ( int x = 0; x < mWidth; ++x )
			{
				int index1 = y * stride + x*4;
				int index2 = y * mWidth*3 + x*3;
				bytePointer[index1] = imageData[index2];
				bytePointer[index1+1] = imageData[index2+1];
				bytePointer[index1+2] = imageData[index2+2];
				bytePointer[index1+3] = 1.0f;
			}
		}	
	}
	else
	{
		int stride = d3dLockedRect.Pitch / sizeof( unsigned char );
		unsigned char* bytePointer = (unsigned char*)d3dLockedRect.pBits;
		unsigned char* imageData = (unsigned char*)largestMipMap.mImageData;

		for ( int y = 0; y < mHeight; ++y )
		{
			for ( int x = 0; x < mWidth; ++x )
			{
				int index1 = y * stride + x*4;
				int index2 = ( mHeight - 1 - y ) * mWidth*3 + x*3;
				bytePointer[index1] = imageData[index2];
				bytePointer[index1+1] = imageData[index2+1];
				bytePointer[index1+2] = imageData[index2+2];
				bytePointer[index1+3] = 255;
			}
		}	
	}

	d3dTexture->UnlockRect( 0 );
	mTexture = reinterpret_cast<void*>( d3dTexture );
}

//===========================================================================

void D3DTexture::LoadTextureCubeImage( const char* filename )
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

	char fullPathFilename[256];
	System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
	System::StringConcat( fullPathFilename, 256, filename );
	System::StringConcat( fullPathFilename, 256, cubeExt[0] );
	System::StringConcat( fullPathFilename, 256, ".dds" );		

	Image image;
	image.Load( fullPathFilename );

	Image::MipMapData data = image.GetMipMapData( 0 );
	mWidth = data.mWidth;
	mHeight = data.mHeight;
	
	D3DFORMAT d3dFormat = D3DFMT_DXT3;
	LPDIRECT3DCUBETEXTURE9 d3dTexture = NULL;
	D3DRenderer::GetDevice()->CreateCubeTexture(
		mWidth,
		1,
		D3DUSAGE_DYNAMIC,
		d3dFormat,
		D3DPOOL_DEFAULT,
		&d3dTexture,
		NULL
		);

	D3DCUBEMAP_FACES cubeMapFaces[6] =
	{
		D3DCUBEMAP_FACE_POSITIVE_X,
		D3DCUBEMAP_FACE_NEGATIVE_X,
		D3DCUBEMAP_FACE_POSITIVE_Y,
		D3DCUBEMAP_FACE_NEGATIVE_Y,
		D3DCUBEMAP_FACE_POSITIVE_Z,
		D3DCUBEMAP_FACE_NEGATIVE_Z
	};
	
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

		D3DLOCKED_RECT d3dLockedRect;	
		int flags = 0;
		d3dTexture->LockRect( cubeMapFaces[i], 0, &d3dLockedRect, NULL, flags );

		//int stride = d3dLockedRect.Pitch / sizeof( unsigned char );
		unsigned char* bytePointer = (unsigned char*)d3dLockedRect.pBits;
		unsigned char* imageData = (unsigned char*)data.mImageData;
		memcpy( bytePointer, imageData, data.mImageDataSize );

		d3dTexture->UnlockRect( cubeMapFaces[i], 0 );
	}
	
	mTexture = reinterpret_cast<void*>( d3dTexture );
}

//===========================================================================

void D3DTexture::LoadTextureImage( const char* filename, TextureFileType mTextureFileType )
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

	Image image;
	image.Load( fullPathFilename );

	SetName( System::Path::SystemSlash( filename ) );

	Image::MipMapData largestMipMap = image.GetMipMapData( 0 );

	mWidth = largestMipMap.mWidth;
	mHeight = largestMipMap.mHeight;

	LPDIRECT3DTEXTURE9 d3dTexture = NULL;

	if ( mTextureFileType == TEXTURE_FILE_HDR )
	{
		CreateTextureWithImage( image );
	}
	else if ( mTextureFileType == TEXTURE_FILE_NORMAL )
	{
        wchar_t fullPathFilenameWCHAR[256];
        System::ConvertToWChar( fullPathFilename, fullPathFilenameWCHAR, 256 );

		D3DXCreateTextureFromFileEx(
			D3DRenderer::GetDevice(),
			fullPathFilenameWCHAR,
			mWidth,
			mHeight,
			D3DX_DEFAULT,
			0,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			0,
			0,
			&d3dTexture
			);	
	}

	mTexture = reinterpret_cast<void*>( d3dTexture );
	Assert( mTexture != NULL, "" );
}

//===========================================================================

void D3DTexture::CreateTexture( int width, int height, TextureFormat textureFormat )
{
	mWidth = width;
	mHeight = height;

	DWORD usageType = D3DUSAGE_RENDERTARGET;
	D3DFORMAT d3dFormat = D3DFMT_A8R8G8B8;
	
	switch ( textureFormat )
	{
	case TEXTURE_FORMAT_RGBA8:
		usageType = D3DUSAGE_RENDERTARGET;
		d3dFormat = D3DFMT_A8R8G8B8;
		break;
	case TEXTURE_FORMAT_RGBA16:
		usageType = D3DUSAGE_RENDERTARGET;
		d3dFormat = D3DFMT_A16B16G16R16F;
		break;
	case TEXTURE_FORMAT_RGBA32:
		usageType = D3DUSAGE_RENDERTARGET;
		d3dFormat = D3DFMT_A32B32G32R32F;
		break;
	case TEXTURE_FORMAT_RG32:
		usageType = D3DUSAGE_RENDERTARGET;
		d3dFormat = D3DFMT_G32R32F;
		break;
	case TEXTURE_FORMAT_DEPTH:
		usageType = D3DUSAGE_DEPTHSTENCIL;
		d3dFormat = (D3DFORMAT)(MAKEFOURCC('D','F','2','4'));		
		break;
	default:
		Assert( false, "Cannot find format." );
		break;
	}

	LPDIRECT3DTEXTURE9 d3dTexture = NULL;

    HRESULT hr = D3DRenderer::GetDevice()->CreateTexture(
        width,
        height,
        1,
        usageType,
        d3dFormat,
        D3DPOOL_DEFAULT,
        &d3dTexture,
        NULL
		);

	mTexture = reinterpret_cast<void*>( d3dTexture );
	Assert( hr == S_OK, "" );
    UNUSED_ALWAYS( hr );
}

#endif