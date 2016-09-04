#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11texture.hpp"
#include "common/engine/render/image.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11Texture::D3D11Texture()
{
    mTexture = NULL;
    mShaderResourceView = NULL;
}

//============================================================================

D3D11Texture::~D3D11Texture()
{
    if ( mTexture != NULL )
    {
        mTexture->Release();
        mTexture = NULL;
    }

    if ( mShaderResourceView != NULL )
    {
        mShaderResourceView->Release();
        mShaderResourceView = NULL;
    }
}

//===========================================================================

void D3D11Texture::CreateTextureWithImage( const Image &image )
{
    Image::MipMapData largestMipMap = image.GetMipMapData( 0 );

    mWidth = largestMipMap.mWidth;
    mHeight = largestMipMap.mHeight;

    int bytesPerPixel = 4;
    int numColorElements = 4;
    Image::ImageDataType imageDataType = image.GetImageDataType();
    void* textureBuffer = NULL;
    TextureFormat textureFormat = TEXTURE_FORMAT_RGBA8;
    switch ( imageDataType )
    {
    case Image::IMAGE_DATA_TYPE_UNSIGNED_BYTE:
        {
            bytesPerPixel = 4;
            textureBuffer = NEW_PTR( "Texture Buffer" ) unsigned char[mWidth*mHeight*numColorElements];
            textureFormat = TEXTURE_FORMAT_RGBA8;

            unsigned char* bytePointer = reinterpret_cast<unsigned char*>( textureBuffer );
            unsigned char* imageData = reinterpret_cast<unsigned char*>( largestMipMap.mImageData );
            for ( int y = 0; y < mHeight; ++y )
            {
                for ( int x = 0; x < mWidth; ++x )
                {
                    int index1 = y * mWidth*numColorElements + x*numColorElements;
                    int index2 = ( mHeight - 1 - y ) * mWidth*3 + x*3;
                    bytePointer[index1] = imageData[index2];
                    bytePointer[index1+1] = imageData[index2+1];
                    bytePointer[index1+2] = imageData[index2+2];
                    bytePointer[index1+3] = 255;
                }
            }
        }
        break;
    case Image::IMAGE_DATA_TYPE_FLOAT:
        {
            bytesPerPixel = 16;
            textureBuffer = NEW_PTR( "Texture Buffer" ) float[mWidth*mHeight*numColorElements];
            textureFormat = TEXTURE_FORMAT_RGBA32;

            float* bytePointer = reinterpret_cast<float*>( textureBuffer );
            float* imageData = reinterpret_cast<float*>( largestMipMap.mImageData );
            for ( int y = 0; y < mHeight; ++y )
            {
                for ( int x = 0; x < mWidth; ++x )
                {
                    int index1 = y * mWidth*numColorElements + x*numColorElements;
                    int index2 = y * mWidth*3 + x*3;
                    bytePointer[index1] = imageData[index2];
                    bytePointer[index1+1] = imageData[index2+1];
                    bytePointer[index1+2] = imageData[index2+2];
                    bytePointer[index1+3] = 1.0f;
                }
            }
        }
        break;
    default:
        Assert( false, "" );
        break;
    }

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = mWidth;
    textureDesc.Height = mHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = GetD3D11TextureFormat( textureFormat );
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    memset( &initData, 0, sizeof( initData ) );
    initData.pSysMem = textureBuffer;
    initData.SysMemPitch = mWidth * bytesPerPixel;

    HRESULT hr = D3D11Renderer::GetDevice()->CreateTexture2D( &textureDesc, &initData, &mTexture  );
    Assert( hr == S_OK, "" );

    D3D11_TEX2D_SRV tex2DSRV;
    tex2DSRV.MipLevels = 1;
    tex2DSRV.MostDetailedMip = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
    resourceDesc.Format = GetD3D11TextureFormat( textureFormat );
    resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resourceDesc.Texture2D = tex2DSRV;
    hr = D3D11Renderer::GetDevice()->CreateShaderResourceView( mTexture, &resourceDesc, &mShaderResourceView );
    Assert( hr == S_OK, "" );

    DELETE_PTR_ARRAY( textureBuffer );
}

//===========================================================================

void D3D11Texture::LoadTextureCubeImage( const char* filename )
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

    Image imageArray[6];

    for ( int i = 0; i < 6; ++i )
    {
        char fullPathFilename[256];
        System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
        System::StringConcat( fullPathFilename, 256, filename );
        System::StringConcat( fullPathFilename, 256, cubeExt[i] );
        System::StringConcat( fullPathFilename, 256, ".dds" );		

        imageArray[i].Load( fullPathFilename );
    }

    Image::MipMapData largestMipMap = imageArray[0].GetMipMapData( 0 );

    mWidth = largestMipMap.mWidth;
    mHeight = largestMipMap.mHeight;

    int bytesPerPixel = 4;
    int numColorElements = 4;
    Image::ImageDataType imageDataType = imageArray[0].GetImageDataType();
    void* textureBuffer[6];
    TextureFormat textureFormat = TEXTURE_FORMAT_RGBA8;
    switch ( imageDataType )
    {
    case Image::IMAGE_DATA_TYPE_UNSIGNED_BYTE:
        {	
            //bytesPerPixel = 4;
            //
            //textureFormat = TEXTURE_FORMAT_RGBA8;

            //for ( int i = 0; i < 6; ++i )
            //{
            //	textureBuffer[i] = NEW_PTR( "Texture Buffer" ) unsigned char[mWidth*mHeight*numColorElements];
            //	Image::MipMapData currentLargestMipMap = imageArray[i].GetMipMapData( 0 );
            //	unsigned char* bytePointer = reinterpret_cast<unsigned char*>( textureBuffer[i] );
            //	unsigned char* imageData = reinterpret_cast<unsigned char*>( currentLargestMipMap.mImageData );
            //	for ( int y = 0; y < mHeight; ++y )
            //	{
            //		for ( int x = 0; x < mWidth; ++x )
            //		{
            //			int index1 = y * mWidth*numColorElements + x*numColorElements;
            //			int index2 = ( mHeight - 1 - y ) * mWidth*4 + x*4;
            //			bytePointer[index1] = imageData[index2];
            //			bytePointer[index1+1] = imageData[index2+1];
            //			bytePointer[index1+2] = imageData[index2+2];
            //			bytePointer[index1+3] = imageData[index2+3];
            //		}
            //	}
            //}
        }
        break;
    case Image::IMAGE_DATA_TYPE_FLOAT:
        {
            bytesPerPixel = 16;
            textureBuffer[0] = NEW_PTR( "Texture Buffer" ) float[mWidth*mHeight*numColorElements];
            textureFormat = TEXTURE_FORMAT_RGBA32;

            float* bytePointer = reinterpret_cast<float*>( textureBuffer[0] );
            float* imageData = reinterpret_cast<float*>( largestMipMap.mImageData );
            for ( int y = 0; y < mHeight; ++y )
            {
                for ( int x = 0; x < mWidth; ++x )
                {
                    int index1 = y * mWidth*numColorElements + x*numColorElements;
                    int index2 = y * mWidth*3 + x*3;
                    bytePointer[index1] = imageData[index2];
                    bytePointer[index1+1] = imageData[index2+1];
                    bytePointer[index1+2] = imageData[index2+2];
                    bytePointer[index1+3] = 1.0f;
                }
            }
        }
        break;
    default:
        Assert( false, "" );
        break;
    }

    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = mWidth;
    textureDesc.Height = mHeight;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 6;
    textureDesc.Format = DXGI_FORMAT_BC2_UNORM;//GetD3D11TextureFormat( textureFormat );
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    D3D11_SUBRESOURCE_DATA initData[6];
    for ( int i = 0; i < 6; ++i )
    {
        memset( &initData[i], 0, sizeof( D3D11_SUBRESOURCE_DATA ) );
        initData[i].pSysMem = imageArray[i].GetMipMapData( 0 ).mImageData;//textureBuffer[i];
        initData[i].SysMemPitch = mWidth * 4;
    }

    HRESULT hr = D3D11Renderer::GetDevice()->CreateTexture2D( &textureDesc, initData, &mTexture  );
    Assert( hr == S_OK, "" );

    D3D11_TEXCUBE_SRV texCubeSRV;
    texCubeSRV.MipLevels = 1;
    texCubeSRV.MostDetailedMip = 0;

    D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
    resourceDesc.Format = DXGI_FORMAT_BC2_UNORM;//GetD3D11TextureFormat( textureFormat );
    resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    resourceDesc.TextureCube = texCubeSRV;
    hr = D3D11Renderer::GetDevice()->CreateShaderResourceView( mTexture, &resourceDesc, &mShaderResourceView );
    Assert( hr == S_OK, "" );

    for ( int i = 0; i < 6; ++i )
    {
        //DELETE_PTR_ARRAY( textureBuffer[i] );
    }
}

//===========================================================================

void D3D11Texture::LoadTextureImage( const char* filename, TextureFileType mTextureFileType )
{
    char fullPathFilename[256];
    System::StringCopy( fullPathFilename, 256, Database::Get()->GetResourcePath() );
    System::StringConcat( fullPathFilename, 256, filename );

    if ( mTextureFileType == TEXTURE_FILE_HDR )
    {
        System::StringConcat( fullPathFilename, 256, ".hdr" );
    }
    else
    {
        System::StringConcat( fullPathFilename, 256, ".dds" );

    }

    Image image;
    image.Load( fullPathFilename );
    SetName( fullPathFilename );
    Image::MipMapData largestMipMap = image.GetMipMapData( 0 );
    mWidth = largestMipMap.mWidth;
    mHeight = largestMipMap.mHeight;

    if ( mTextureFileType == TEXTURE_FILE_HDR )
    {
        CreateTextureWithImage( image );
    }
    else
    {
        TextureFormat textureFormat = TEXTURE_FORMAT_RGBA8;
        unsigned int sysMemPitch = mWidth * 4;
        switch ( image.GetPixelFormat() )
        {
        case Image::PF_RGB:
            textureFormat = TEXTURE_FORMAT_RGBA8;
            sysMemPitch = mWidth * 3;
            break;
        case Image::PF_RGBA:
            textureFormat = TEXTURE_FORMAT_RGBA8;
            sysMemPitch = mWidth * 4;
            break;
        case Image::PF_BGRA:
            textureFormat = TEXTURE_FORMAT_RGBA8;
            sysMemPitch = mWidth * 4;
            break;
        case Image::PF_PVRTC_4BPPV1:
            break;
        case Image::PF_PVRTC_2BPPV1:
            break;
        case Image::PF_COMPRESSED_DXT1:
            textureFormat = TEXTURE_FORMAT_DXT1;
            sysMemPitch = ( mWidth / 4 ) * 8;
            break;
        case Image::PF_COMPRESSED_DXT3:
            textureFormat = TEXTURE_FORMAT_DXT3;
            sysMemPitch = ( mWidth / 4 ) * 16;
            break;
        case Image::PF_COMPRESSED_DXT5:
            textureFormat = TEXTURE_FORMAT_DXT5;
            sysMemPitch = ( mWidth / 4 ) * 16;
            break;
        }


        D3D11_TEXTURE2D_DESC textureDesc;
        textureDesc.Width = mWidth;
        textureDesc.Height = mHeight;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = GetD3D11TextureFormat( textureFormat );
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        memset( &initData, 0, sizeof( initData ) );
        initData.pSysMem = largestMipMap.mImageData;        
        initData.SysMemPitch = sysMemPitch;

        HRESULT hr = D3D11Renderer::GetDevice()->CreateTexture2D( &textureDesc, &initData, &mTexture  );
        Assert( hr == S_OK, "" );

        D3D11_TEX2D_SRV tex2DSRV;
        tex2DSRV.MipLevels = 1;
        tex2DSRV.MostDetailedMip = 0;

        D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc;
        resourceDesc.Format = GetD3D11TextureFormat( textureFormat );
        resourceDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        resourceDesc.Texture2D = tex2DSRV;
        hr = D3D11Renderer::GetDevice()->CreateShaderResourceView( mTexture, &resourceDesc, &mShaderResourceView );
        Assert( hr == S_OK, "" );

        Assert( mShaderResourceView != NULL, "" );
    }
}

//===========================================================================

void D3D11Texture::CreateTexture( int width, int height, TextureFormat textureFormat )
{
    mWidth = width;
    mHeight = height;

    UINT usageType = D3D11_BIND_RENDER_TARGET;
    DXGI_FORMAT d3dFormat = GetD3D11TextureFormat( textureFormat );
    DXGI_FORMAT srvFormat = GetD3D11SRVFormat( textureFormat );

    switch ( textureFormat )
    {
    case TEXTURE_FORMAT_RGBA8:
        usageType = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        break;
    case TEXTURE_FORMAT_RGBA16:
        usageType = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        break;
    case TEXTURE_FORMAT_RGBA32:
        usageType = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        break;
    case TEXTURE_FORMAT_R32:
        usageType = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        break;
    case TEXTURE_FORMAT_RG32:
        usageType = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        break;
    case TEXTURE_FORMAT_DEPTH:
        usageType = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
        break;
    default:
        Assert( false, "Cannot find format." );
        break;
    }

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory( &desc, sizeof(desc) );
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = d3dFormat;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = usageType;	
    D3D11Renderer::GetDevice()->CreateTexture2D( &desc, NULL, &mTexture );

    D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
    srDesc.Format = srvFormat;
    srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srDesc.Texture2D.MostDetailedMip = 0;
    srDesc.Texture2D.MipLevels = 1;

    D3D11Renderer::GetDevice()->CreateShaderResourceView( mTexture, &srDesc, &mShaderResourceView );
}

//===========================================================================

DXGI_FORMAT D3D11Texture::GetD3D11TextureFormat( TextureFormat textureFormat )
{
    switch ( textureFormat )
    {
    case TEXTURE_FORMAT_RGBA8:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    case TEXTURE_FORMAT_RGBA16:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        break;
    case TEXTURE_FORMAT_RGBA32:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        break;
    case TEXTURE_FORMAT_RG32:
        return DXGI_FORMAT_R32G32_FLOAT;
        break;
    case TEXTURE_FORMAT_R32:
        return DXGI_FORMAT_R32_FLOAT;
        break;
    case TEXTURE_FORMAT_DXT1:
        return DXGI_FORMAT_BC1_UNORM;
        break;
    case TEXTURE_FORMAT_DXT3:
        return DXGI_FORMAT_BC2_UNORM;
        break;
    case TEXTURE_FORMAT_DXT5:
        return DXGI_FORMAT_BC3_UNORM;
        break;
    case TEXTURE_FORMAT_DEPTH:
        return DXGI_FORMAT_R24G8_TYPELESS;
        break;
    default:
        Assert( false, "Cannot find format." );
        return DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    }
}

//===========================================================================

DXGI_FORMAT D3D11Texture::GetD3D11SRVFormat( TextureFormat textureFormat )
{
    switch ( textureFormat )
    {
    case TEXTURE_FORMAT_RGBA8:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    case TEXTURE_FORMAT_RGBA16:
        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        break;
    case TEXTURE_FORMAT_RGBA32:
        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        break;
    case TEXTURE_FORMAT_RG32:
        return DXGI_FORMAT_R32G32_FLOAT;
        break;
    case TEXTURE_FORMAT_R32:
        return DXGI_FORMAT_R32_FLOAT;
        break;
    case TEXTURE_FORMAT_DEPTH:
        return DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
        break;
    default:
        Assert( false, "Cannot find format." );
        return DXGI_FORMAT_R8G8B8A8_UNORM;
        break;
    }
}

#endif