#ifndef POSTFX_HPP
#define POSTFX_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderTarget;
class Mesh;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class PostFX
{

public:

	PostFX();
	~PostFX();

	void Initialize();
	void Update( float elapsedTime );

	void Luminance(
		RenderTarget* inputRT,
		RenderTarget* outputRT,
		Math::Vector4 luminanceOffsets[4]
	);

	void LuminanceDownSample33(
		RenderTarget* inputRT,
		RenderTarget* outputRT,
		Math::Vector4 downSampleOffsets[9] 
	);

	void DownSample44(
		RenderTarget* inputRT,
		RenderTarget* outputRT,
		Math::Vector4 downSampleOffsets[16]
	);

	void BrightPass(
		RenderTarget* inputRT,
		RenderTarget* outputRT,
		float brightnessThreshold,
		Math::Vector4 downSampleOffsets[4]
	);

	void BlurHorizontal(
		RenderTarget* inputRT,
		RenderTarget* outputRT,
		float offsets[9],
		float weights[9]
	);

	void BlurVertical(
		RenderTarget* inputRT,
		RenderTarget* outputRT,
		float offsets[9],
		float weights[9]
	);

	void Blur44(
		RenderTarget* inputRT,
		RenderTarget* outputRT
	);

	void DepthOfField(
		RenderTarget* mainSourceRT,
		RenderTarget* blurRT,
		const Math::Vector4 &dofParams,
		const Math::Vector4 &dofParams2,
		Math::Vector4 sampleOffsets[12]
		);

	void HDRComposite(
		RenderTarget* mainSourceRT,
		RenderTarget* luminanceRT,
		RenderTarget* bloomRT
		);

	void SSAO(
		RenderTarget* inputRT,
		RenderTarget* outputRT,
		Texture* randomTexture,
		Texture* normalTexture,
		Texture* depthTexture,
		Math::Matrix44 projectionMatrix,
		Math::Vector4 samples[16],
		Math::Vector4 hemiSphereKernel[16]
		);

    void EdgeOutline(
        RenderTarget* inputRT,
        const Math::Vector4 &dofParams,
        Math::Vector4 samples[9]
        );

	void PostPass( RenderTarget* inputRT );
	void PostPassSSAO( RenderTarget* inputRT, RenderTarget* ssaoRT );

	static void GenerateDownSample22Offsets( int width, int height, Math::Vector4 offsets[4] );
	static void GenerateDownSample33Offsets( int width, int height, Math::Vector4 offsets[9] );
	static void GenerateDownSample44Offsets( int width, int height, Math::Vector4 offsets[16] );
	static void GenerateGaussianOffsetsWeights( int length, float offsets[9], float weights[9] );
	static void GenerateCircleSamples( int width, int height, Math::Vector4 sampleOffsets[12] );
	static void GenerateRandomSphereSamples( Math::Vector4 samples[16] );
	static void GenerateHemiSphereSampleKernel( Math::Vector4 samples[16] );
	
private:

	Mesh* mQuad;
	Effect* mLuminanceEffect;
	Effect* mLuminanceDownSample33Effect;
	Effect* mBrightPassEffect;
	Effect* mDownSample44Effect;
	Effect* mBlurEffect;
	Effect* mBlur44Effect;
	Effect* mDOFEffect;
	Effect* mHDREffect;
	Effect* mSSAOEffect;
	Effect* mPostPassEffect;
    Effect* mEdgeOutlineEffect;

    int mBlurDirectionHash;
    int mBlurOffsetsHash;
    int mBlurWeightsHash;
    int mOffsetHash;    
    int mDownSampleOffsetsHash;
    int mPixelSizeHash;
    int mDoFParamsHash;
    int mDoFParams2Hash;
    int mSampleOffsetsHash;
    int mSamplesHash;
    int mKernelHash;
    int mObjectToProjectionMatrixHash;
    int mTexture0Hash;
    int mTexture1Hash;
    int mTexture2Hash;    
    int mProjectionMatrixHash;
    int mInverseProjectionMatrixHash;
    int mRandomTextureHash;
    int mNormalTextureHash;
    int mDepthTextureHash;
    int mMainTextureHash;    
    int mSSAOTextureHash;
};

#endif