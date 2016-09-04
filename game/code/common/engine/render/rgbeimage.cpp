//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rgbeimage.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/math/vector4.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RGBEImage::RGBEImage() 
	:	mImageData( NULL )
	,	mWidth( 0 )
	,	mHeight( 0 )
	,	mExposure( 0 )
{    
}

//===========================================================================

RGBEImage::~RGBEImage()
{
	DELETE_PTR_ARRAY( mImageData );

	for ( int i = 0; i < CUBE_MAP_FACE_MAX; ++i )
	{
		DELETE_PTR_ARRAY( mCubeMapFaceImage[i].mImageData );
	}
}

//===========================================================================

void RGBEImage::Load( const char* filename )
{
	DELETE_PTR_ARRAY( mImageData );

	FILE* file = System::OpenFile( filename, "rb" );
	Assert( file != NULL, "Cannot open file." );

	bool finished = false;
	char buffer[1024];
	int i = 0;
	do 
	{
		buffer[i] = static_cast<char>( fgetc( file ) );
		if ( buffer[i] == static_cast<char>( 10 ) )
		{			
			buffer[i++] = '\0';
			if ( strstr( buffer, "EXPOSURE" ) )
			{
				sscanf_s( buffer, "EXPOSURE=%f", &mExposure );
			}
			else if ( strstr( buffer, "FORMAT" ) )
			{
				char formatString[1024];
				sscanf_s( buffer, "FORMAT=%s", formatString );
				Assert( strcmp( formatString, "32-bit_rle_rgbe" ) == 0, "" );
			}
			else if ( strstr( buffer, "-Y" ) || strstr( buffer, "+X" )  )
			{
				sscanf_s( buffer, "-Y %i +X %i", &mHeight, &mWidth );
				finished = true;
			}

			i = 0;
		}
		else if ( buffer[i])
			i++;
	} 
	while ( finished == false );

	int size = mWidth * mHeight * 3;

	mImageData = NEW_PTR( "Image Data" ) float[size];

	for ( int heightIndex = 0; heightIndex < mHeight; ++heightIndex )
	{
		Math::Vector4Base<unsigned char>* newScanline = NEW_PTR( "RGBE Scanline" ) Math::Vector4Base<unsigned char>[mWidth];

		unsigned char byte0 = static_cast<unsigned char>( fgetc( file ) );
		unsigned char byte1 = static_cast<unsigned char>( fgetc( file ) );
		unsigned char byte2 = static_cast<unsigned char>( fgetc( file ) );
		unsigned char byte3 = static_cast<unsigned char>( fgetc( file ) );
		int widthCheck = byte2 << 8;
		widthCheck |= byte3;
		if ( byte0 != 2 || byte1 != 2 || widthCheck != mWidth )
		{
			Assert( false, "" );
		}

		int numElements = 4;
		for ( int colorIndex = 0; colorIndex < numElements; ++colorIndex )
		{
			int widthIndex = 0;
			while ( widthIndex < mWidth )
			{
				unsigned char code = static_cast<unsigned char>( fgetc( file ) );

				if ( code > 128 )
				{
					code &= 127;
					unsigned char value = static_cast<unsigned char>( fgetc( file ) );
					for ( int index = 0; index < code; index++ )
					{
						newScanline[widthIndex][colorIndex] = static_cast<unsigned char>( value );
						widthIndex++;
					}
				}
				else
				{
					for ( int index = 0; index < code; index++ )
					{
						unsigned char value = static_cast<unsigned char>( fgetc( file ) );
						newScanline[widthIndex][colorIndex] = static_cast<unsigned char>( value );
						widthIndex++;
					}
				}
			}

			Assert( widthIndex == mWidth, "" );
		}

		mNumColorElements = 3;
		for ( int widthIndex = 0; widthIndex < mWidth; ++widthIndex )
		{
			Math::Vector4Base<unsigned char> rgbe = newScanline[widthIndex];
			float r = ConvertComponent( rgbe[3], rgbe[0] );
			float g = ConvertComponent( rgbe[3], rgbe[1] );
			float b = ConvertComponent( rgbe[3], rgbe[2] );

			int stride = heightIndex * mWidth * mNumColorElements;
			int index0 = stride + widthIndex * mNumColorElements;
			int index1 = stride + ( widthIndex * mNumColorElements + 1 );
			int index2 = stride + ( widthIndex * mNumColorElements + 2 );
			mImageData[index0] = r;
			mImageData[index1] = g;
			mImageData[index2] = b;
		}
		DELETE_PTR_ARRAY( newScanline );
	}

	fclose( file );
	file = NULL;
}

//===========================================================================

void RGBEImage::SplitCubeMap()
{
	Assert( mImageData != NULL, "No Image Data to Split" );

	int cubeFaceWidth = mWidth / 3;
	int cubeFaceHeight = mHeight / 4;

	int imageSize = cubeFaceWidth * cubeFaceHeight * mNumColorElements;

	for ( int i = 0; i < CUBE_MAP_FACE_MAX; ++i )
	{
		mCubeMapFaceImage[i].mWidth = cubeFaceWidth;
		mCubeMapFaceImage[i].mHeight = cubeFaceHeight;
		mCubeMapFaceImage[i].mImageData = NEW_PTR( "Image Data" ) float[imageSize];
	}
	FillCubeFace( mCubeMapFaceImage[CUBE_MAP_FACE_POS_Y], cubeFaceWidth, 0 );
	FillCubeFace( mCubeMapFaceImage[CUBE_MAP_FACE_NEG_Y], cubeFaceWidth, cubeFaceHeight*2 );
	FillCubeFace( mCubeMapFaceImage[CUBE_MAP_FACE_NEG_X], 0, cubeFaceHeight );
	FillCubeFace( mCubeMapFaceImage[CUBE_MAP_FACE_POS_X], cubeFaceWidth*2, cubeFaceHeight );
	FillCubeFace( mCubeMapFaceImage[CUBE_MAP_FACE_NEG_Z], cubeFaceWidth, cubeFaceHeight );
	FillCubeFace( mCubeMapFaceImage[CUBE_MAP_FACE_POS_Z], cubeFaceWidth, cubeFaceHeight*3 );
}

//===========================================================================

RGBEImage::CubeMapFaceImage RGBEImage::GetCubeMapImageData( RGBEImage::CubeMapFace face )
{
	return mCubeMapFaceImage[face];
}

//===========================================================================

float RGBEImage::ConvertComponent( unsigned char expo, unsigned char val )
{
	float v = val / 256.0f;
	float d = powf( 2.0f, static_cast<float>( expo - 128 ) );
	return v * d;
}

//===========================================================================

void RGBEImage::FillCubeFace( RGBEImage::CubeMapFaceImage face, int xOffset, int yOffset )
{
	for ( int y = 0; y < face.mHeight; ++y )
	{
		for ( int x = 0; x < face.mWidth; ++x )
		{
			int index0 = y * face.mWidth * mNumColorElements + x*mNumColorElements;
			int index1 = (y+yOffset) * mWidth * mNumColorElements + (x+xOffset)*mNumColorElements;
			face.mImageData[index0] = mImageData[index1];
			face.mImageData[index0+1] = mImageData[index1+1];
			face.mImageData[index0+2] = mImageData[index1+2];
		}
	}
}