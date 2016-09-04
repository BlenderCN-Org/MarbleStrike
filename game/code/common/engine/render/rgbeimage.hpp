#ifndef RGBE_IMAGE_HPP
#define RGBE_IMAGE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RGBEImage
{
public:
	enum CubeMapFace
	{
		CUBE_MAP_FACE_POS_X,
		CUBE_MAP_FACE_NEG_X,
		CUBE_MAP_FACE_POS_Y,
		CUBE_MAP_FACE_NEG_Y,
		CUBE_MAP_FACE_POS_Z,
		CUBE_MAP_FACE_NEG_Z,
		CUBE_MAP_FACE_MAX
	};

	struct CubeMapFaceImage
	{
		CubeMapFaceImage()
		{
			mImageData = 0;
			mWidth = 0;
			mHeight = 0;
		}

		float* mImageData;
		int mWidth;
		int mHeight;
	};

public:

	RGBEImage();
	~RGBEImage();

	void Load( const char* filename );

	int GetWidth() const
	{
		return mWidth;
	}

	int GetHeight() const
	{
		return mHeight;
	}

	float* GetImageData() const
	{
		return mImageData;
	}

	void SplitCubeMap();
	CubeMapFaceImage GetCubeMapImageData( CubeMapFace face );

private:    

	float ConvertComponent( unsigned char expo, unsigned char val );
	void FillCubeFace( CubeMapFaceImage face, int xOffset, int yOffset );

	int mNumColorElements;
	float mExposure;
	float* mImageData;
	int mWidth;
	int mHeight;
	CubeMapFaceImage mCubeMapFaceImage[CUBE_MAP_FACE_MAX];
};

#endif
