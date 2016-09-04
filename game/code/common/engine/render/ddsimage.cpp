//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ddsimage.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum FourCCFlag
{
	FOURCC_ID_DXT1 = 0x31545844,
	FOURCC_ID_DXT3 = 0x33545844,
	FOURCC_ID_DXT5 = 0x35545844,
	FOURCC_ID_NONE = 0
};

struct DDSHeader
{
	unsigned int dwSize;
	unsigned int dwFlags;
	unsigned int dwHeight;
	unsigned int dwWidth;
	unsigned int dwPitchOrLinearSize;
	unsigned int dwDepth;
	unsigned int dwMipMapCount;
	unsigned int dwReserved1[11];

	//  DDPIXELFORMAT
	struct DDSPixelFormat
	{
		unsigned int dwSize;
		unsigned int dwFlags;
		unsigned int dwFourCC;
		unsigned int dwRGBBitCount;
		unsigned int dwRBitMask;
		unsigned int dwGBitMask;
		unsigned int dwBBitMask;
		unsigned int dwAlphaBitMask;

		void EndianSwap()
		{
			System::EndianSwap( dwSize );
			System::EndianSwap( dwFlags );
			System::EndianSwap( dwFourCC );
			System::EndianSwap( dwRGBBitCount );
			System::EndianSwap( dwRBitMask );
			System::EndianSwap( dwGBitMask );
			System::EndianSwap( dwBBitMask );
			System::EndianSwap( dwAlphaBitMask );
		}
	} ddpfPixelFormat;

	//  DDCAPS2
	struct DDSCaps
	{
		unsigned int dwCaps1;
		unsigned int dwCaps2;
		unsigned int dwDDSX;
		unsigned int dwReserved;

		void EndianSwap()
		{
			System::EndianSwap( dwCaps1 );
			System::EndianSwap( dwCaps2 );
			System::EndianSwap( dwDDSX );
			System::EndianSwap( dwReserved );
		}
	} ddsCaps;

	unsigned int dwReserved2;

	void EndianSwap()
	{
		System::EndianSwap( dwSize );
		System::EndianSwap( dwFlags );
		System::EndianSwap( dwHeight );
		System::EndianSwap( dwWidth );
		System::EndianSwap( dwPitchOrLinearSize );
		System::EndianSwap( dwDepth );
		System::EndianSwap( dwMipMapCount );
		System::EndianSwap( dwReserved2 );

		ddpfPixelFormat.EndianSwap();
		ddsCaps.EndianSwap();

	}
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

DDSImage::DDSImage()
:	mMipMaps( NULL )
,	mNumMipMaps( 0 )
,	mPixelFormatType( Image::PF_NONE )
,	mImageDataType( Image::IMAGE_DATA_TYPE_UNSIGNED_BYTE )
{    
}

//===========================================================================

DDSImage::~DDSImage()
{
	for ( int i = 0; i < mNumMipMaps; ++i )
	{
        if ( mImageDataType == Image::IMAGE_DATA_TYPE_UNSIGNED_BYTE )
        {
            unsigned int* data = reinterpret_cast<unsigned int*>( mMipMaps[i].mImageData );
            DELETE_PTR_ARRAY( data );
        }
        else if ( mImageDataType == Image::IMAGE_DATA_TYPE_FLOAT )
        {
            float* data = reinterpret_cast<float*>( mMipMaps[i].mImageData );
            DELETE_PTR_ARRAY( data );
        }
	}
	DELETE_PTR_ARRAY( mMipMaps );
}

//===========================================================================

void DDSImage::Load( const char* filename, bool flip )
{
	FILE* file = System::OpenFile( filename, "rb" );
	Assert( file != NULL, "Cannot open file." );

	const unsigned int DDS_MAGIC = 0x20534444;

	unsigned int dwMagicWord;
	fread( &dwMagicWord, sizeof( unsigned int ), 1, file );
	System::EndianSwap( dwMagicWord );

	Assert( dwMagicWord == DDS_MAGIC, "" );
    UNUSED_ALWAYS( DDS_MAGIC );

	DDSHeader ddsHeader;
	fread( &ddsHeader, sizeof( DDSHeader ), 1, file );
	ddsHeader.EndianSwap();

	Assert( ddsHeader.dwSize == 124, "" );

	mNumMipMaps = ddsHeader.dwMipMapCount;
	if ( mNumMipMaps == 0 )
	{
		mNumMipMaps = 1;
	}
	mMipMaps = NEW_PTR( "DDS Mip Maps" ) Image::MipMapData[mNumMipMaps];

	FourCCFlag fourCCFlag = static_cast<FourCCFlag>( ddsHeader.ddpfPixelFormat.dwFourCC );
	if ( fourCCFlag == FOURCC_ID_NONE )
	{
		for ( int i = 0; i < mNumMipMaps; ++i )
		{
			int width = ddsHeader.dwWidth;
			int height = ddsHeader.dwHeight;
			int size = width * height * 4;
			
			Image::MipMapData* currentMipMap = &mMipMaps[i];
			currentMipMap->mWidth = width;
			currentMipMap->mHeight = height;
			currentMipMap->mImageDataSize = size;
			currentMipMap->mImageData = NEW_PTR( "DDS Texture" ) unsigned char[size];
			fread( currentMipMap->mImageData, sizeof( unsigned char ), size, file );
			mPixelFormatType = Image::PF_BGRA;			

			if ( flip )
			{
				int widthLength = width * ddsHeader.ddpfPixelFormat.dwSize / 8;
				int heightHalf = height / 2;
				for ( int j = 0; j < heightHalf; j++ )
				{
					for ( int i = 0; i < widthLength; i++ )
					{      
						unsigned int index1 = ( height - j - 1 ) * widthLength + i;
						unsigned int index2 = ( j * widthLength ) + i;
						unsigned char* imageData = reinterpret_cast<unsigned char*>( currentMipMap->mImageData );
						unsigned char temp = imageData[ index1 ];        
						imageData[ index1 ] = imageData[ index2 ];
						imageData[ index2 ] = temp;
					}
				}
			}
		}
	}
	else if ( 
		fourCCFlag == FOURCC_ID_DXT1 || 
		fourCCFlag == FOURCC_ID_DXT3 ||
		fourCCFlag == FOURCC_ID_DXT5 
		)
	{	
		int divSize = 0;
		int blockSizeBytes = 0;

		void (*flipFunction)( unsigned char* ) = NULL;

		switch ( fourCCFlag )
		{
		case FOURCC_ID_DXT1:
			mPixelFormatType = Image::PF_COMPRESSED_DXT1;
			divSize = 4;
			blockSizeBytes = 8;
			flipFunction = FlipDXT1BlockFull;
			break;
		case FOURCC_ID_DXT3:
			mPixelFormatType = Image::PF_COMPRESSED_DXT3;
			divSize = 4;
			blockSizeBytes = 16;
			flipFunction = FlipDXT3BlockFull;
			break;
		case FOURCC_ID_DXT5:
			mPixelFormatType = Image::PF_COMPRESSED_DXT5;
			divSize = 4;
			blockSizeBytes = 16;
			flipFunction = FlipDXT5BlockFull;
			break;
        default:
            break;
		}
		
		int width = ddsHeader.dwWidth;
		int height = ddsHeader.dwHeight;								
		for( int i = 0; i < mNumMipMaps; ++i ) 
		{
			Image::MipMapData* currentMipMap = &mMipMaps[i];
			int mipMapSize = Math::Max( divSize, width )/divSize * Math::Max( divSize, height )/divSize * blockSizeBytes;
			currentMipMap->mWidth = width;
			currentMipMap->mHeight = height;
			currentMipMap->mImageDataSize = mipMapSize;
			currentMipMap->mImageData = NEW_PTR( "DDS Image Data" )unsigned char[mipMapSize];
			fread( currentMipMap->mImageData, 1, mipMapSize, file );
			width = ( width ) >> 1;
			if ( width == 0 )
			{
				width = 1;
			}
			height = ( height ) >> 1;	
			if ( height == 0 )
			{
				height = 1;
			}

			//
			// Flip image
			//
			if ( flip )
			{
				int numWidthBlocks = currentMipMap->mWidth / divSize;
				int numHeightBlocks = currentMipMap->mHeight / divSize;
				int widthBytes = numWidthBlocks * blockSizeBytes;

				for ( int yIndex = 0; yIndex < numHeightBlocks / 2; ++yIndex )
				{
					for ( int xIndex = 0; xIndex < numWidthBlocks; ++xIndex )
					{
						int curBlockIndex = (yIndex * widthBytes) + (xIndex * blockSizeBytes);
						int mirrorBlockIndex = (numHeightBlocks - yIndex - 1) * widthBytes + xIndex * blockSizeBytes;

						unsigned char* imageData = reinterpret_cast<unsigned char*>( currentMipMap->mImageData );
						unsigned char* currentBlock = &imageData[curBlockIndex];
						unsigned char* mirrorBlock = &imageData[mirrorBlockIndex];
						flipFunction( currentBlock );
						flipFunction( mirrorBlock );

						for ( int blockIndex = 0; blockIndex < blockSizeBytes; ++blockIndex )
						{
							unsigned char tempByte = currentBlock[blockIndex];
							currentBlock[blockIndex] = mirrorBlock[blockIndex];
							mirrorBlock[blockIndex] = tempByte;							
						}
					}
				}
			}		
		}
	}

	fclose( file );	
	file = NULL;
}


//===========================================================================

void DDSImage::FlipDXT1BlockFull( unsigned char* data )
{
	// A DXT1 block layout is:
	// [0-1] color0.
	// [2-3] color1.
	// [4-7] color bitmap, 2 bits per pixel.
	// So each of the 4-7 bytes represents one line, flipping a block is just
	// flipping those bytes.

	unsigned char tmp;

	tmp = data[4];
	data[4] = data[7];
	data[7] = tmp;

	tmp = data[5];
	data[5] = data[6];
	data[6] = tmp;
}

//===========================================================================

void DDSImage::FlipDXT3BlockFull( unsigned char* block )
{
	// Flips a full DXT3 block in the y direction.
	// A DXT3 block layout is:
	// [0-7]  alpha bitmap, 4 bits per pixel.
	// [8-15] a DXT1 block.

	// We can flip the alpha bits at the byte level (2 bytes per line).
	unsigned char tmp = block[0];
	block[0] = block[6];
	block[6] = tmp;
	tmp = block[1];
	block[1] = block[7];
	block[7] = tmp;
	tmp = block[2];
	block[2] = block[4];
	block[4] = tmp;
	tmp = block[3];
	block[3] = block[5];
	block[5] = tmp;

	// And flip the DXT1 block using the above function.
	FlipDXT1BlockFull(block + 8);
}

//===========================================================================

void DDSImage::FlipDXT5BlockFull( unsigned char* block )
{
	// A DXT5 block layout is:
	// [0]    alpha0.
	// [1]    alpha1.
	// [2-7]  alpha bitmap, 3 bits per pixel.
	// [8-15] a DXT1 block.

	// The alpha bitmap doesn't easily map lines to bytes, so we have to
	// interpret it correctly.  Extracted from
	// http://www.opengl.org/registry/specs/EXT/texture_compression_s3tc.txt :
	//
	//   The 6 "bits" bytes of the block are decoded into one 48-bit integer:
	//
	//     bits = bits_0 + 256 * (bits_1 + 256 * (bits_2 + 256 * (bits_3 +
	//                   256 * (bits_4 + 256 * bits_5))))
	//
	//   bits is a 48-bit unsigned integer, from which a three-bit control code
	//   is extracted for a texel at location (x,y) in the block using:
	//
	//       code(x,y) = bits[3*(4*y+x)+1..3*(4*y+x)+0]
	//
	//   where bit 47 is the most significant and bit 0 is the least
	//   significant bit.

	unsigned int line_0_1 = block[2] + 256 * (block[3] + 256 * block[4]);
	unsigned int line_2_3 = block[5] + 256 * (block[6] + 256 * block[7]);
	// swap lines 0 and 1 in line_0_1.
	unsigned int line_1_0 = ((line_0_1 & 0x000fff) << 12) |	((line_0_1 & 0xfff000) >> 12);
	// swap lines 2 and 3 in line_2_3.
	unsigned int line_3_2 = ((line_2_3 & 0x000fff) << 12) |	((line_2_3 & 0xfff000) >> 12);
	block[2] = static_cast<unsigned char>( line_3_2 & 0xff );
	block[3] = static_cast<unsigned char>( (line_3_2 & 0xff00) >> 8 );
	block[4] = static_cast<unsigned char>( (line_3_2 & 0xff0000) >> 16 );
	block[5] = static_cast<unsigned char>( line_1_0 & 0xff );
	block[6] = static_cast<unsigned char>( (line_1_0 & 0xff00) >> 8 );
	block[7] = static_cast<unsigned char>( (line_1_0 & 0xff0000) >> 16 );

	FlipDXT1BlockFull(block+8);
}
