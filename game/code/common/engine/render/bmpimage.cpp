//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/bmpimage.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BMPImage::BMPImage() 
:	mImageData( NULL )
,	mWidth( 0 )
,	mHeight( 0 )
,	mBitsPerPixel( 0 )
{    
}

//===========================================================================

BMPImage::~BMPImage()
{
	DELETE_PTR_ARRAY( mImageData );
}

//===========================================================================

void BMPImage::Load( const char* filename, bool flip )
{
	DELETE_PTR_ARRAY( mImageData );

	FILE* file = System::OpenFile( filename, "rb" );
	Assert( file != NULL, "Cannot open file." );

	BMPHeader bmpHeader;
	fread( &bmpHeader.mID, sizeof( char ), 2, file );
	if ( bmpHeader.mID[0] != 'B' || bmpHeader.mID[1] != 'M' )
	{
		Assert( false, "Invalid BMP file format." );
	}

	fread( &bmpHeader.mFileSize, sizeof( int ), 1, file );
	fread( &bmpHeader.mReserved1, sizeof( char ), 2, file );
	fread( &bmpHeader.mReserved2, sizeof( char ), 2, file );
	fread( &bmpHeader.mOffset, sizeof( int ), 1, file );

	BMPHeaderInfo bmpHeaderInfo;	
	fread( &bmpHeaderInfo.mHeaderSize, sizeof( int ), 1, file );
	if ( bmpHeaderInfo.mHeaderSize != 40 )
	{
		Assert( false, "Invalid BMP header size." );
	}

	fread( &bmpHeaderInfo.mWidth, sizeof( int ), 1, file );
	fread( &bmpHeaderInfo.mHeight, sizeof( int ), 1, file );
	fread( &bmpHeaderInfo.mNumColorPlanes, sizeof( short int ), 1, file );
	fread( &bmpHeaderInfo.mBitsPerPixel, sizeof( short int ), 1, file );
	if ( bmpHeaderInfo.mBitsPerPixel != 24 && bmpHeaderInfo.mBitsPerPixel != 32 )
	{
		Assert( false, "Invalid BMP bits per pixel size." );
	}

	fread( &bmpHeaderInfo.mCompressionMethod, sizeof( int ), 1, file );
	fread( &bmpHeaderInfo.mImageSize, sizeof( int ), 1, file );
	fread( &bmpHeaderInfo.mHorizontalRes, sizeof( int ), 1, file );
	fread( &bmpHeaderInfo.mVerticalRes, sizeof( int ), 1, file );
	fread( &bmpHeaderInfo.mNumColorsInPalette, sizeof( int ), 1, file );
	fread( &bmpHeaderInfo.mNumImportantColorsUsed, sizeof( int ), 1, file );

	mWidth = bmpHeaderInfo.mWidth;
	mHeight = bmpHeaderInfo.mHeight;
	mBitsPerPixel = bmpHeaderInfo.mBitsPerPixel;

	fseek( file, bmpHeader.mOffset, SEEK_SET );

	int numBytesPerPixel = mBitsPerPixel / 8;
	int size = mWidth * mHeight * numBytesPerPixel;

	mImageData = NEW_PTR( "Image Data" )unsigned char[size];
	fread( mImageData, sizeof( unsigned char ), size, file );

	if ( flip )
	{
		int halfHeight = mHeight / 2;
		for ( int y = 0; y < halfHeight; ++y )
		{
			for ( int x = 0; x < mWidth; ++x )
			{			
				int indexTopR = y*mWidth*numBytesPerPixel + (x*numBytesPerPixel + 0);
				int indexTopG = y*mWidth*numBytesPerPixel + (x*numBytesPerPixel + 1);
				int indexTopB = y*mWidth*numBytesPerPixel + (x*numBytesPerPixel + 2);
				int indexTopA = 0;
				if ( mBitsPerPixel == 32 )
				{
					indexTopA = y*mWidth*numBytesPerPixel + (x*numBytesPerPixel + 3);
				}

				int indexBottomR = (mHeight - 1 - y)*mWidth*numBytesPerPixel + (x*numBytesPerPixel + 0);
				int indexBottomG = (mHeight - 1 - y)*mWidth*numBytesPerPixel + (x*numBytesPerPixel + 1);
				int indexBottomB = (mHeight - 1 - y)*mWidth*numBytesPerPixel + (x*numBytesPerPixel + 2);
				int indexBottomA = 0;
				if ( mBitsPerPixel == 32 )
				{
					indexBottomA = (mHeight - 1 - y)*mWidth*4 + (x*4 + 3);			
				}

				unsigned char r = mImageData[indexTopR];
				unsigned char g = mImageData[indexTopG];
				unsigned char b = mImageData[indexTopB];
				unsigned char a = 0;
				if ( mBitsPerPixel == 32 )
				{
					a = mImageData[indexTopA];
				}

				mImageData[indexTopR] = mImageData[indexBottomR];
				mImageData[indexTopG] = mImageData[indexBottomG];
				mImageData[indexTopB] = mImageData[indexBottomB];
				if ( mBitsPerPixel == 32 )
				{
					mImageData[indexTopA] = mImageData[indexBottomA];
				}

				mImageData[indexBottomR] = r;
				mImageData[indexBottomG] = g;
				mImageData[indexBottomB] = b;

				if ( mBitsPerPixel == 32 )
				{
					mImageData[indexBottomA] = a;
				}
			}
		}
	}

	fclose( file );
	file = NULL;
}
