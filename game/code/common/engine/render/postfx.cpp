//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/constantbuffer.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/postfx.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendererfactory.hpp"
#include "common/engine/render/rendertarget.hpp"
#include "common/engine/render/shaderparametermatrix.hpp"
#include "common/engine/render/shaderparametertexture.hpp"
#include "common/engine/render/shaderparametervector.hpp"
#include "common/engine/render/simpledraw.hpp"
#include "common/engine/system/debug/debugvariable.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/render/rendernode.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

PostFX::PostFX()
{	
}

//===========================================================================

PostFX::~PostFX()
{
	DELETE_PTR( mQuad );
}

//===========================================================================

void PostFX::Initialize()
{
    //mBlurDirectionHash = System::StringHash( "gBlurDirection" );
    //mBlurOffsetsHash = System::StringHash( "gBlurOffsets" );
    //mBlurWeightsHash = System::StringHash( "gBlurWeights" );
    mOffsetHash = System::StringHash( "gOffset" );    
    //mDownSampleOffsetsHash = System::StringHash( "gDownSampleOffsets" );
    //mPixelSizeHash = System::StringHash( "gPixelSize" );
    //mDoFParamsHash = System::StringHash( "gDoFParams" );
    //mDoFParams2Hash = System::StringHash( "gDoFParams2" );
    //mSampleOffsetsHash = System::StringHash( "gSampleOffsets" );
    //mSamplesHash = System::StringHash( "gSamples" );
    //mKernelHash = System::StringHash( "gKernel" );
    mObjectToProjectionMatrixHash = System::StringHash( "gObjectToProjectionMatrix" );
    //mTexture0Hash = System::StringHash( "gTexture0" );
    //mTexture1Hash = System::StringHash( "gTexture1" );
    //mTexture2Hash = System::StringHash( "gTexture2" );    
    //mProjectionMatrixHash = System::StringHash( "gProjectionMatrix" );
    //mInverseProjectionMatrixHash = System::StringHash( "gInverseProjectionMatrix" );
    //mRandomTextureHash = System::StringHash( "gRandomTexture" );
    //mNormalTextureHash = System::StringHash( "gNormalTexture" );
    //mDepthTextureHash = System::StringHash( "gDepthTexture" );
    mMainTextureHash = System::StringHash( "gMainTexture" );   
    //mSSAOTextureHash = System::StringHash( "gSSAOTexture" );

	mQuad = NEW_PTR( "Quad" ) Mesh;
	mQuad->SetPrimitiveType( PRIMITIVE_TRIANGLES );
	Math::Vector3 positions[4];
	positions[0].Set( 0, 0, 0 );
	positions[1].Set( 1, 0, 0 );
	positions[2].Set( 1, 1, 0 );
	positions[3].Set( 0, 1, 0 );	
	Math::Vector2 texCoords[4];
	texCoords[0].Set( 0, 0 );
	texCoords[1].Set( 1, 0 );
	texCoords[2].Set( 1, 1 );
	texCoords[3].Set( 0, 1 );
	unsigned int indices[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;
	mQuad->SetNumVertices( 4 );	
	mQuad->SetPositions( positions, 4 );
	mQuad->SetTexCoords( texCoords, 4 );
	mQuad->SetFaceIndices( indices, 2, 3 );

	//mLuminanceEffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_luminance" );
	//mDownSample44Effect = GameApp::Get()->GetEffectManager()->GetEffect( "post_downsample" );	
	//mBlurEffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_blur" );	
	//mBlur44Effect = GameApp::Get()->GetEffectManager()->GetEffect( "post_blur44" );	
	//mBrightPassEffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_brightpass" );
	//mDOFEffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_dof" );
	//mHDREffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_hdr" );
	//mSSAOEffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_ssao" );
	mPostPassEffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_pass" );	
    //mEdgeOutlineEffect = GameApp::Get()->GetEffectManager()->GetEffect( "post_edge_outline" );	
}

//===========================================================================

void PostFX::Update( float /*elapsedTime*/ )
{
}

//===========================================================================

void PostFX::Luminance(
	RenderTarget* /*inputRT*/,
	RenderTarget* outputRT,
	Math::Vector4[4] /*luminanceOffsets[4]*/
)
{
	Renderer::Get()->SetRenderTarget( outputRT, NULL );	
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.0f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );

	//mLuminanceVertexData.mObjectToProjectionMatrix = quadTransform;
	//mLuminanceVertexData.mOffset.Set( 1.0f / static_cast<float>( outputRT->GetWidth() ), 1.0f / static_cast<float>( outputRT->GetHeight() ), 0, 0 );
	//for ( int i = 0; i < 4; ++i )
	//{
	//	mLuminancePixelData.mDownSampleOffsets[i] = luminanceOffsets[i];
	//}
	//mLuminanceTextureInput = inputRT->GetColorTexture();

	//EffectPass* effectPass = mLuminanceEffect->GetTechnique( 0 )->GetPass( 0 );
	//effectPass->Dispatch( &mRenderStateShadowing, NULL );
	//mLuminanceConstantBufferVertex->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//mLuminanceConstantBufferPixel->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//Renderer::Get()->RenderBuffers( 
	//	mQuad, 
	//	mLuminanceEffect->GetVertexDeclaration(), 
	//	effectPass,
	//	NULL
	//	);

	//////RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
	//////renderNode->SetDescription( "Luminance" );
	//////renderNode->SetTechnique( 0 );
	//////renderNode->SetPass( 0 );
	//////renderNode->SetRenderTarget( outputRT );
	//////renderNode->SetEffect( mLuminanceEffect );
	//////renderNode->SetVertexConstantBuffer( mLuminanceConstantBufferVertex );
	//////renderNode->SetPixelConstantBuffer(mLuminanceConstantBufferPixel );
	//////renderNode->SetMesh( mQuad );
	//////GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::LuminanceDownSample33(
	RenderTarget* /*inputRT*/,
	RenderTarget* outputRT,
	Math::Vector4[9] /*downSampleOffsets[9]*/ 
)
{
	Renderer::Get()->SetRenderTarget( outputRT, NULL );	
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.0f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );

	//mLuminanceDownSample33VertexData.mObjectToProjectionMatrix = quadTransform;
	//mLuminanceDownSample33VertexData.mOffset.Set( 1.0f / static_cast<float>( outputRT->GetWidth() ), 1.0f / static_cast<float>( outputRT->GetHeight() ), 0, 0 );
	//for ( int i = 0; i < 9; ++i )
	//{
	//	mLuminanceDownSample33PixelData.mDownSampleOffsets[i] = downSampleOffsets[i];
	//}
	//mLuminanceDownSample33TextureInput = inputRT->GetColorTexture();

	//EffectPass* effectPass = mLuminanceEffect->GetTechnique( 1 )->GetPass( 0 );
	//effectPass->Dispatch( &mRenderStateShadowing, NULL );
	//mLuminanceDownSample33ConstantBufferVertex->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//mLuminanceDownSample33ConstantBufferPixel->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//Renderer::Get()->RenderBuffers( 
	//	mQuad, 
	//	mLuminanceEffect->GetVertexDeclaration(), 
	//	effectPass,
	//	NULL
	//	);

	//////RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
	//////renderNode->SetDescription( "LuminanceDownSample33" );
	//////renderNode->SetTechnique( 1 );
	//////renderNode->SetPass( 0 );
	//////renderNode->SetRenderTarget( outputRT );
	//////renderNode->SetEffect( mLuminanceEffect );
	//////renderNode->SetVertexConstantBuffer( mLuminanceDownSample33ConstantBufferVertex );
	//////renderNode->SetPixelConstantBuffer(mLuminanceDownSample33ConstantBufferPixel );
	//////renderNode->SetMesh( mQuad );
	//////GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::DownSample44(
	RenderTarget* inputRT,
	RenderTarget* outputRT,
	Math::Vector4 downSampleOffsets[16]
)
{
	Renderer::Get()->SetRenderTarget( outputRT, NULL );		
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	
		
	Math::Vector4 offset( 1.0f / static_cast<float>( outputRT->GetWidth() ), 1.0f / static_cast<float>( outputRT->GetHeight() ), 0, 0 );
	
	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "DownSample44" );	
#endif

	renderNode->mRenderTarget = outputRT;
	renderNode->mEffect = mDownSample44Effect;	
	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
	renderNode->SetVector( mOffsetHash, &offset );
	renderNode->SetVector( mDownSampleOffsetsHash, downSampleOffsets, 16 );
	renderNode->SetTexture( mTexture0Hash, inputRT->GetColorTexture() );
	renderNode->mMesh = mQuad;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::BrightPass(
	RenderTarget* /*inputRT*/,
	RenderTarget* outputRT,
	float /*brightnessThreshold*/,
	Math::Vector4[4] /*downSampleOffsets[4]*/
)
{
	Renderer::Get()->SetRenderTarget( outputRT, NULL );	
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.0f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );

	//mBrightPassVertexData.mObjectToProjectionMatrix = quadTransform;
	//mBrightPassVertexData.mOffset.Set( 1.0f / static_cast<float>( outputRT->GetWidth() ), 1.0f / static_cast<float>( outputRT->GetHeight() ), 0, 0 );
	//mBrightPassPixelData.mBrightPassThreshold.Set( brightnessThreshold, 0, 0, 0 );
	//for ( int i = 0; i < 4; ++i )
	//{
	//	mBrightPassPixelData.mDownSampleOffsets[i] = downSampleOffsets[i];
	//}
	//mBrightPassTextureInput = inputRT->GetColorTexture();

	//EffectPass* effectPass = mBrightPassEffect->GetTechnique( 0 )->GetPass( 0 );
	//effectPass->Dispatch( &mRenderStateShadowing, NULL );
	//mBrightPassConstantBufferVertex->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//mBrightPassConstantBufferPixel->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//Renderer::Get()->RenderBuffers( 
	//	mQuad, 
	//	mBrightPassEffect->GetVertexDeclaration(), 
	//	effectPass,
	//	NULL
	//	);

	//////RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
	//////renderNode->SetDescription( "Bright Pass" );
	//////renderNode->SetTechnique( 0 );
	//////renderNode->SetPass( 0 );
	//////renderNode->SetRenderTarget( outputRT );
	//////renderNode->SetEffect( mBrightPassEffect );
	//////renderNode->SetVertexConstantBuffer( mBrightPassConstantBufferVertex );
	//////renderNode->SetPixelConstantBuffer(mBrightPassConstantBufferPixel );
	//////renderNode->SetMesh( mQuad );
	//////GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::BlurHorizontal(
	RenderTarget* inputRT,
	RenderTarget* outputRT,
	float offsets[9],
	float weights[9]
)
{
	Renderer::Get()->SetRenderTarget( outputRT, NULL );		
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "Blur Horizontal" );	
#endif
	renderNode->mRenderTarget = outputRT;
	renderNode->mEffect = mBlurEffect;

	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
	renderNode->SetVector( mBlurDirectionHash, Math::Vector4( 1.0, 0.0, 0.0, 0.0 ) );	

	Math::Vector4 blurOffsets[3];	
	blurOffsets[0].Set( offsets[0], offsets[1], offsets[2], offsets[3] );
	blurOffsets[1].Set( offsets[4], offsets[5], offsets[6], offsets[7] );
	blurOffsets[2].Set( offsets[8], 0, 0, 0 );
	renderNode->SetVector( mBlurOffsetsHash, blurOffsets, 3 );

	Math::Vector4 blurWeights[3];	
	blurWeights[0].Set( weights[0], weights[1], weights[2], weights[3] );
	blurWeights[1].Set( weights[4], weights[5], weights[6], weights[7] );
	blurWeights[2].Set( weights[8], 0, 0, 0 );
	renderNode->SetVector( mBlurWeightsHash, blurWeights, 3 );

	Math::Vector4 offset( 1.0f / static_cast<float>( outputRT->GetWidth() ), 1.0f / static_cast<float>( outputRT->GetHeight() ), 0, 0 );
	renderNode->SetVector( mOffsetHash, &offset );
	renderNode->SetTexture( mTexture0Hash, inputRT->GetColorTexture() );

	renderNode->mMesh = mQuad;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::BlurVertical(
	RenderTarget* inputRT,
	RenderTarget* outputRT,
	float offsets[9],
	float weights[9]
)
{
	Renderer::Get()->SetRenderTarget( outputRT, NULL );		
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "Blur Vertical" );	
#endif
	renderNode->mRenderTarget = outputRT;
	renderNode->mEffect = mBlurEffect;

    renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );	
	renderNode->SetVector( mBlurDirectionHash, Math::Vector4( 0.0, 1.0, 0.0, 0.0 ) );	
	
	Math::Vector4 blurOffsets[3];	
	blurOffsets[0].Set( offsets[0], offsets[1], offsets[2], offsets[3] );
	blurOffsets[1].Set( offsets[4], offsets[5], offsets[6], offsets[7] );
	blurOffsets[2].Set( offsets[8], 0, 0, 0 );
	renderNode->SetVector( mBlurOffsetsHash, blurOffsets, 3 );

	Math::Vector4 blurWeights[3];	
	blurWeights[0].Set( weights[0], weights[1], weights[2], weights[3] );
	blurWeights[1].Set( weights[4], weights[5], weights[6], weights[7] );
	blurWeights[2].Set( weights[8], 0, 0, 0 );
	renderNode->SetVector( mBlurWeightsHash, blurWeights, 3 );

	Math::Vector4 offset( 1.0f / static_cast<float>( outputRT->GetWidth() ), 1.0f / static_cast<float>( outputRT->GetHeight() ), 0, 0 );
	renderNode->SetVector( mOffsetHash, &offset );
	renderNode->SetTexture( mTexture0Hash, inputRT->GetColorTexture() );

	renderNode->mMesh = mQuad;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::Blur44(
	RenderTarget* inputRT,
	RenderTarget* outputRT
	)
{
	Renderer::Get()->SetRenderTarget( outputRT, NULL );		
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "Blur44" );	
#endif
	renderNode->mRenderTarget = outputRT;
	renderNode->mEffect = mBlur44Effect;

	float width = static_cast<float>( inputRT->GetWidth() );
	float height = static_cast<float>( inputRT->GetHeight() );
	Math::Vector4 pixelSize( 1.0f / width, 1.0f / height, 0, 0 );

	renderNode->SetVector( mPixelSizeHash, &pixelSize );
	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
	renderNode->SetTexture( mTexture0Hash, inputRT->GetColorTexture() );

	renderNode->mMesh = mQuad;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::DepthOfField(
	RenderTarget* mainSourceRT,
	RenderTarget* blurRT,
	const Math::Vector4 &dofParams,
	const Math::Vector4 &dofParams2,
	Math::Vector4 sampleOffsets[12]
	)
{
	Renderer::Get()->SetRenderTarget( NULL, NULL );
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	float screenWidth = static_cast<float>( Database::Get()->GetBackBufferWidth() );
	float screenHeight = static_cast<float>( Database::Get()->GetBackBufferHeight() );
	Math::Vector4 offset( 1.0f / screenWidth, 1.0f / screenHeight, 0, 0 );

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "Depth of Field" );	
#endif

	renderNode->mRenderTarget = NULL;
	renderNode->mEffect = mDOFEffect;	
	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
	renderNode->SetTexture( mTexture0Hash, mainSourceRT->GetColorTexture() );
	renderNode->SetTexture( mTexture1Hash, blurRT->GetColorTexture() );
	renderNode->SetTexture( mTexture2Hash, mainSourceRT->GetDepthTexture() );
	renderNode->SetVector( mDoFParamsHash, &dofParams );
	renderNode->SetVector( mDoFParams2Hash, &dofParams2 );
	renderNode->SetVector( mSampleOffsetsHash, sampleOffsets, 12 );	
	renderNode->SetVector( mOffsetHash, &offset );

	renderNode->mMesh = mQuad;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::HDRComposite(
	RenderTarget* /*mainSourceRT*/,
	RenderTarget* /*luminanceRT*/,
	RenderTarget* /*bloomRT*/
	)
{
	Renderer::Get()->SetRenderTarget( NULL, NULL );
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	//mHDRVertexData.mObjectToProjectionMatrix = quadTransform;
	//float screenWidth = static_cast<float>( Database::Get()->GetScreenWidth() );
	//float screenHeight = static_cast<float>( Database::Get()->GetScreenHeight() );
	//mHDRVertexData.mOffset.Set( 1.0f / screenWidth, 1.0f / screenHeight, 0, 0 );
	//float exposure = 0.50f;
	//float gaussMultiplier = 0.4f;
	//mHDRPixelData.mHDRParameters.Set( 
	//	exposure, 
	//	gaussMultiplier, 
	//	1.0f / static_cast<float>( bloomRT->GetWidth() ), 
	//	1.0f / static_cast<float>( bloomRT->GetHeight() )
	//	);
	//mHDRMainTextureInput  = mainSourceRT->GetColorTexture();
	//mHDRLuminanceTextureInput = luminanceRT->GetColorTexture();
	//mHDRBloomTextureInput = bloomRT->GetColorTexture();

	//EffectPass* effectPass = mHDREffect->GetTechnique( 0 )->GetPass( 0 );
	//effectPass->Dispatch( &mRenderStateShadowing, NULL );
	//mHDRConstantBufferVertex->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//mHDRConstantBufferPixel->Dispatch( &mRenderStateShadowing, effectPass, NULL );
	//Renderer::Get()->RenderBuffers( 
	//	mQuad, 
	//	mHDREffect->GetVertexDeclaration(), 
	//	effectPass,
	//	NULL
	//	);

	//////RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
	//////renderNode->SetDescription( "HDR Composite" );
	//////renderNode->SetTechnique( 0 );
	//////renderNode->SetPass( 0 );
	//////renderNode->SetRenderTarget( NULL );
	//////renderNode->SetEffect( mHDREffect );
	//////renderNode->SetVertexConstantBuffer( mHDRConstantBufferVertex );
	//////renderNode->SetPixelConstantBuffer(mHDRConstantBufferPixel );
	//////renderNode->SetMesh( mQuad );
	//////GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::SSAO( 
	RenderTarget* inputRT,
	RenderTarget* outputRT,
	Texture* randomTexture, 
	Texture* normalTexture,
	Texture* depthTexture,
	Math::Matrix44 projectionMatrix, 
	Math::Vector4 samples[16],
	Math::Vector4 hemiSphereKernel[16]
)
{
	Assert( inputRT != NULL, "" );
    UNUSED_ALWAYS( inputRT );

	Renderer::Get()->SetRenderTarget( outputRT, NULL );
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );

	Math::Matrix44 inverseProjectionMatrix;
	inverseProjectionMatrix = projectionMatrix;
	inverseProjectionMatrix.Inverse();

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "SSAO" );
#endif

    renderNode->mEffect = mSSAOEffect;	
	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
	renderNode->SetMatrix( mProjectionMatrixHash, &projectionMatrix );
	renderNode->SetMatrix( mInverseProjectionMatrixHash, &inverseProjectionMatrix );
	renderNode->SetVector( mSamplesHash, samples, 16 );
	renderNode->SetVector( mKernelHash, hemiSphereKernel, 16 );
	renderNode->SetTexture( mRandomTextureHash, randomTexture );
	renderNode->SetTexture( mNormalTextureHash, normalTexture );
	renderNode->SetTexture( mDepthTextureHash, depthTexture );
	renderNode->mRenderTarget = outputRT;
	renderNode->mMesh = mQuad;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::EdgeOutline( 
    RenderTarget* inputRT, 
    const Math::Vector4 &dofParams,
    Math::Vector4 samples[9] 
)
{
	int technique = mEdgeOutlineEffect->GetTechniqueIndex( "post_edge_outline" );
	
	Assert( inputRT != NULL, "" );

	Renderer::Get()->SetRenderTarget( NULL, NULL );
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	float screenWidth = static_cast<float>( Database::Get()->GetBackBufferWidth() );
	float screenHeight = static_cast<float>( Database::Get()->GetBackBufferHeight() );
	Math::Vector4 offset( 1.0f / screenWidth, 1.0f / screenHeight, 0, 0 );

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "Post Edge Outline" );
#endif

	renderNode->mEffect = mEdgeOutlineEffect;	
	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
    renderNode->SetVector( mOffsetHash, &offset );
	renderNode->SetVector( mSampleOffsetsHash, samples, 9 );
    renderNode->SetVector( mDoFParamsHash, &dofParams );
	renderNode->SetTexture( mMainTextureHash, inputRT->GetColorTexture() );
    renderNode->SetTexture( mDepthTextureHash, inputRT->GetDepthTexture() );
	renderNode->mMesh = mQuad;
	renderNode->mTechnique = technique;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::PostPass( RenderTarget* inputRT )
{
	int technique = mPostPassEffect->GetTechniqueIndex( "post_pass" );
	
	Assert( inputRT != NULL, "" );

	Renderer::Get()->SetRenderTarget( NULL, NULL );
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	float screenWidth = static_cast<float>( Database::Get()->GetBackBufferWidth() );
	float screenHeight = static_cast<float>( Database::Get()->GetBackBufferHeight() );
	Math::Vector4 offset( 1.0f / screenWidth, 1.0f / screenHeight, 0, 0 );

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "Post Pass" );
#endif

	renderNode->mEffect = mPostPassEffect;	
	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
	renderNode->SetVector( mOffsetHash, &offset );
	renderNode->SetTexture( mMainTextureHash, inputRT->GetColorTexture() );
	renderNode->mMesh = mQuad;
	renderNode->mTechnique = technique;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::PostPassSSAO( RenderTarget* inputRT, RenderTarget* ssaoRT )
{
	int technique = mPostPassEffect->GetTechniqueIndex( "post_pass_ssao" );

	Assert( inputRT != NULL, "" );

	Renderer::Get()->SetRenderTarget( NULL, NULL );
	Renderer::Get()->SetBackgroundColor( 0.0f, 0.0f, 0.7f );
	Renderer::Get()->ClearRenderer( NULL );

	Math::Matrix44 quadTransform;
	quadTransform.Translate( -1.0f, -1.0f, 0.0f );
	quadTransform.Scale( 2.0f, 2.0f, 1.0f );	

	float screenWidth = static_cast<float>( Database::Get()->GetBackBufferWidth() );
	float screenHeight = static_cast<float>( Database::Get()->GetBackBufferHeight() );
	Math::Vector4 offset( 1.0f / screenWidth, 1.0f / screenHeight, 0, 0 );

	RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
#if defined ( _DEBUG )
	renderNode->SetDescription( "Post Pass" );
#endif

	renderNode->mEffect = mPostPassEffect;	
	renderNode->SetMatrix( mObjectToProjectionMatrixHash, &quadTransform );
	renderNode->SetVector( mOffsetHash, &offset );
	renderNode->SetTexture( mMainTextureHash, inputRT->GetColorTexture() );
	renderNode->SetTexture( mSSAOTextureHash, ssaoRT->GetColorTexture() );
	renderNode->mMesh = mQuad;
	renderNode->mTechnique = technique;
	GameApp::Get()->GetRenderQueue()->Render();
}

//===========================================================================

void PostFX::GenerateDownSample22Offsets( int width, int height, Math::Vector4 offsets[4] )
{
	float sU = ( 1.0f / static_cast< float >( width ) );
	float sV = ( 1.0f / static_cast< float >( height ) );
	offsets[0].Set( -0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
	offsets[1].Set( 0.5f * sU, 0.5f * sV, 0.0f, 0.0f );
	offsets[2].Set( -0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
	offsets[3].Set( 0.5f * sU, -0.5f * sV, 0.0f, 0.0f );
}

//===========================================================================

void PostFX::GenerateDownSample33Offsets( int width, int height, Math::Vector4 offsets[9] )
{
	int index = 0;
	for( int y = -1; y < 2; y++ )
	{
		for( int x = -1; x < 2; x++ )
		{
			offsets[index++].Set(
				static_cast< float >( x ) / static_cast< float >( width ),
				static_cast< float >( y ) / static_cast< float >( height ),
				0.0f,   //unused
				0.0f    //unused
				);
		}
	}
}

//===========================================================================

void PostFX::GenerateDownSample44Offsets( int width, int height, Math::Vector4 offsets[16] )
{
	int index = 0;
	for( int i = -2; i < 2; i++ )
	{
		for( int j = -2; j < 2; j++ )
		{
			offsets[index++].Set(
				( static_cast< float >( i ) + 0.5f ) * ( 1.0f / static_cast< float >( width ) ),
				( static_cast< float >( j ) + 0.5f ) * ( 1.0f / static_cast< float >( height ) ),
				0.0f, // unused 
				0.0f  // unused
				);
		}
	}
}

//===========================================================================

void PostFX::GenerateGaussianOffsetsWeights( int length, float offsets[9], float weights[9] )
{
	float g_GaussMultiplier = 1.0f;
	float g_GaussMean = 0.0f;
	float g_GaussStdDev = 0.8f;
	float total = 0;
	for( int i = 0; i < 9; i++ )
	{
		// Compute the offsets. We take 9 samples - 4 either side and one in the middle:
		//     i =  0,  1,  2,  3, 4,  5,  6,  7,  8
		//Offset = -4, -3, -2, -1, 0, +1, +2, +3, +4
		offsets[i] = ( static_cast< float >( i ) - 4.0f ) * ( 1.0f / static_cast< float >( length ) );

		// 'x' is just a simple alias to map the [0,8] range down to a [-1,+1]
		float x = ( static_cast< float >( i ) - 4.0f ) / 4.0f;

		// Use a Gaussian distribution. Changing the standard-deviation
		// (second parameter) as well as the amplitude (multiplier) gives
		// distinctly different results.
		weights[i] = g_GaussMultiplier * Math::GaussianWeights( x, g_GaussMean, g_GaussStdDev );
		total += weights[i];
	}

	if ( total >  0 )
	{
		for( int i = 0; i < 9; i++ )
		{
			weights[i] /= total;
		}
	}
}

//===========================================================================

void PostFX::GenerateCircleSamples( int width, int height, Math::Vector4 sampleOffsets[12] )
{
	float dx = 1.0f / static_cast<float>( width );
	float dy = 1.0f / static_cast<float>( height );

	sampleOffsets[0].Set( -0.326212f * dx, -0.40581f * dy, 0, 0 );
	sampleOffsets[1].Set( -0.840144f * dx, -0.07358f * dy, 0, 0 );
	sampleOffsets[2].Set( -0.695914f * dx, 0.457137f * dy, 0, 0 );
	sampleOffsets[3].Set( -0.203345f * dx, 0.620716f * dy, 0, 0 );
	sampleOffsets[4].Set( 0.96234f * dx, -0.194983f * dy, 0, 0 ); 
	sampleOffsets[5].Set( 0.473434f * dx, -0.480026f * dy, 0, 0 ); 
	sampleOffsets[6].Set( 0.519456f * dx, 0.767022f * dy, 0, 0 ); 
	sampleOffsets[7].Set( 0.185461f * dx, -0.893124f * dy, 0, 0 ); 
	sampleOffsets[8].Set( 0.507431f * dx, 0.064425f * dy, 0, 0 ); 
	sampleOffsets[9].Set( 0.89642f * dx, 0.412458f * dy, 0, 0 ); 
	sampleOffsets[10].Set( -0.32194f * dx, -0.932615f * dy, 0, 0 ); 
	sampleOffsets[11].Set( -0.791559f * dx, -0.59771f * dy, 0, 0 );
}

//===========================================================================

void PostFX::GenerateRandomSphereSamples( Math::Vector4 samples[16] )
{
	for ( int i = 0; i < 16; ++i )
	{
		float theta = Math::RandomFloat0() * Math::PI;
		float phi = Math::RandomFloat0() * Math::PIMUL2;
		float x = sinf( theta ) * cosf( phi );		
		float y = sin( theta ) * sinf( phi );		
		float z = cos( theta );

		samples[i].Set( x, y, z, 1 );
	}	
}

//===========================================================================

void PostFX::GenerateHemiSphereSampleKernel( Math::Vector4 samples[16] )
{
	const int kernalSize = 16;
	for ( int i = 0; i < kernalSize; ++i ) 
	{
		samples[i] = Math::Vector4(
			Math::RandomFloat0(),
			Math::RandomFloat0(),
			Math::RandomFloat1(),
			0
			);

		Math::Vector4::Normalize( samples[i] );
		samples[i] *= Math::RandomFloat1();

		float scale = static_cast<float>( i ) / static_cast<float>( kernalSize );
		float t = scale * scale;
		scale = 0.1f * ( 1 - t ) + 1.0f * t;		
		samples[i] *= scale;
	}
}