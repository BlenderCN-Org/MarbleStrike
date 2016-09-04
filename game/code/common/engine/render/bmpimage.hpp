#ifndef BMP_IMAGE_HPP
#define BMP_IMAGE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BMPImage
{
public:
	
	BMPImage();
	~BMPImage();

	void Load( const char* filename, bool flip );
	
	int GetWidth() const
	{
		return mWidth;
	}

	int GetHeight() const
	{
		return mHeight;
	}
	
	unsigned char* GetImageData() const
	{
		return mImageData;
	}

	int GetBitsPerPixel() const
	{
		return mBitsPerPixel;
	}

private:    

	struct BMPHeader
	{
		char mID[2];
		int mFileSize;
		char mReserved1[2];
		char mReserved2[2];
		int mOffset;
	};

	struct BMPHeaderInfo
	{
		int mHeaderSize;
		int mWidth;
		int mHeight;
		short int mNumColorPlanes;
		short int mBitsPerPixel;
		int mCompressionMethod;
		int mImageSize;
		int mHorizontalRes;
		int mVerticalRes;
		int mNumColorsInPalette;
		int mNumImportantColorsUsed;
	};

	unsigned char* mImageData;
	int mWidth;
	int mHeight;
	int mBitsPerPixel;
};

#endif
