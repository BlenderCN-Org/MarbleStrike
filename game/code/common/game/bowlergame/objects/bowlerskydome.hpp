#ifndef BOWLER_SKY_DOME_HPP
#define BOWLER_SKY_DOME_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BlenderAsset;
class Effect;
class Material;
class RenderObject;
class RenderTarget;
class Texture;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerSkyDome
{
public:

	BowlerSkyDome();
	~BowlerSkyDome();

    void Initialize();
    void Update( float elapsedTime );
	void RenderUpdate( const Math::Matrix44 &cameraTransform, const Camera* camera );		
    void SetRenderTarget( RenderTarget* target )
    {
        mRenderTarget = target;
    }
	
private:

	Effect* mSkyDomeEffect;
	BlenderAsset* mBlenderAsset;
    RenderTarget* mRenderTarget;
	Texture* mEmptyTexture;
		
    int mDiffuseColorHash;
    int mObjectToProjectionMatrixHash;
	int mObjectToCameraMatirxHash;
    int mColorTextureHash;
};

#endif