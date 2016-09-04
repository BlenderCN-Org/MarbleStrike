//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderstateshadowing.hpp"
#include "common/engine/render/renderer.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RenderStateShadowing::RenderStateShadowing()
{
	int numTextureUnits = Renderer::Get()->GetNumTextureUnits();
	mCurrentSamplerState = NEW_PTR( "Sampler States" ) const SamplerState*[numTextureUnits];
	Clear();
}

//===========================================================================

RenderStateShadowing::~RenderStateShadowing()
{	
	DELETE_PTR_ARRAY( mCurrentSamplerState );
}

//===========================================================================

void RenderStateShadowing::Clear()
{
	mCurrentDepthStencilState = NULL;	
	mCurrentBlendState = NULL;	
	mCurrentRasterizerState = NULL;
    mShaderProgram = -1;
	int numTextureUnits = Renderer::Get()->GetNumTextureUnits();
	for ( int i = 0; i < numTextureUnits; ++i )
	{
		mCurrentSamplerState[i] = NULL;
	}
    mCurrentRenderTarget = NULL;
    mCurrentRenderTargetType = RenderStateShadowing::RENDER_TARGET_TYPE_NONE;
	mCurrentEffectPass = NULL;

	memset( &mVectorParameterTable[0][0], -99999, sizeof ( Math::Vector4 ) * MAX_SHADER_TYPE * MAX_NUM_SHADERS * MAX_CONSTANT_VECTOR_HANDLES );
    memset( &mMatrixParameterTable[0][0], -99999, sizeof ( Math::Matrix44 ) * MAX_SHADER_TYPE * MAX_NUM_SHADERS * MAX_CONSTANT_MATRIX_HANDLES );
}

//===========================================================================

const DepthStencilState* RenderStateShadowing::GetCurrentDepthStencilState() const
{
	return mCurrentDepthStencilState;
}

//===========================================================================

void RenderStateShadowing::SetCurrentDepthStencilState( const DepthStencilState* state )
{
	mCurrentDepthStencilState = state;
}

//===========================================================================

const BlendState* RenderStateShadowing::GetCurrentBlendState() const
{	
	return mCurrentBlendState;
}

//===========================================================================

void RenderStateShadowing::SetCurrentBlendState( const BlendState* data )
{
	mCurrentBlendState = data;
}

//===========================================================================

const RasterizerState* RenderStateShadowing::GetCurrentRasterizerState() const
{	
	return mCurrentRasterizerState;
}

//===========================================================================

void RenderStateShadowing::SetCurrentRasterizerState( const RasterizerState* data )
{
	mCurrentRasterizerState = data;
}

//===========================================================================

const SamplerState* RenderStateShadowing::GetCurrentSamplerState( int index ) const
{	
	return mCurrentSamplerState[index];
}

//===========================================================================

void RenderStateShadowing::SetCurrentSamplerState( int index, const SamplerState* data )
{
	mCurrentSamplerState[index] = data;
}

//===========================================================================

const RenderTarget* RenderStateShadowing::GetCurrentRenderTarget() const
{	
	return mCurrentRenderTarget;
}

//===========================================================================

void RenderStateShadowing::SetCurrentRenderTarget( RenderTarget* renderTarget )
{
	mCurrentRenderTarget = renderTarget;
}

//===========================================================================

RenderStateShadowing::RenderTargetType RenderStateShadowing::GetCurrentRenderTargetType() const
{
    return mCurrentRenderTargetType;
}

//===========================================================================

void RenderStateShadowing::SetCurrentRenderTargetType( RenderStateShadowing::RenderTargetType type )
{
    mCurrentRenderTargetType = type;
}

//===========================================================================

const EffectPass* RenderStateShadowing::GetCurrentEffectPass() const
{
	return mCurrentEffectPass;
}

//===========================================================================

void RenderStateShadowing::SetCurrentEffectPass( const EffectPass* pass )
{
	mCurrentEffectPass = pass;
}

//===========================================================================

void RenderStateShadowing::SetCurrentShaderParameterVector( int shaderType, int shaderHandle, int constantHandle, Math::Vector4 data )
{
	Assert( shaderHandle < MAX_NUM_SHADERS, "" );
	Assert( constantHandle < MAX_CONSTANT_VECTOR_HANDLES, "" );
    Assert( shaderType < MAX_SHADER_TYPE, "" );
	mVectorParameterTable[shaderType][shaderHandle][constantHandle] = data;
}

//===========================================================================

Math::Vector4 RenderStateShadowing::GetCurrentShaderParameterVector( int shaderType, int shaderHandle, int constantHandle )
{
    Assert( shaderHandle < MAX_NUM_SHADERS, "" );
	Assert( constantHandle < MAX_CONSTANT_VECTOR_HANDLES, "" );
    Assert( shaderType < MAX_SHADER_TYPE, "" );
	return mVectorParameterTable[shaderType][shaderHandle][constantHandle];
}

//===========================================================================

void RenderStateShadowing::SetCurrentShaderParameterMatrix( int shaderType, int shaderHandle, int constantHandle, Math::Matrix44 data )
{
    Assert( shaderHandle < MAX_NUM_SHADERS, "" );
	Assert( constantHandle < MAX_CONSTANT_VECTOR_HANDLES, "" );
    Assert( shaderType < MAX_SHADER_TYPE, "" );
	mMatrixParameterTable[shaderType][shaderHandle][constantHandle] = data;
}

//===========================================================================

Math::Matrix44 RenderStateShadowing::GetCurrentShaderParameterMatrix( int shaderType, int shaderHandle, int constantHandle )
{
    Assert( shaderHandle < MAX_NUM_SHADERS, "" );
	Assert( constantHandle < MAX_CONSTANT_MATRIX_HANDLES, "" );
    Assert( shaderType < MAX_SHADER_TYPE, "" );
	return mMatrixParameterTable[shaderType][shaderHandle][constantHandle];
}

//===========================================================================

void RenderStateShadowing::SetShaderProgram( unsigned int id)
{
    mShaderProgram = id;
}

//===========================================================================

unsigned int RenderStateShadowing::GetShaderProgram()
{
    return mShaderProgram;
}