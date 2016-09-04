#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d/d3dconstantbuffer.hpp"
#include "common/engine/render/shaderparameter.hpp"
#include "common/engine/render/shaderparametermatrix.hpp"
#include "common/engine/render/shaderparametertexture.hpp"
#include "common/engine/render/shaderparametervector.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DConstantBuffer::D3DConstantBuffer( ConstantBuffer::ConstantBufferType type )
	:	ConstantBuffer( type )
{
}

//============================================================================

D3DConstantBuffer::~D3DConstantBuffer()
{
	
}
//===========================================================================

void D3DConstantBuffer::Dispatch(
	RenderStateShadowing* renderStateShadowing, 
	EffectPass* effectPass,
	void* context
	)
{
	ConstantBuffer::DispatchInternal( renderStateShadowing, effectPass, context );
}

//===========================================================================

void D3DConstantBuffer::UploadParameters( 
	RenderStateShadowing* renderStateShadowing, 
	EffectPass* effectPass,
	void* context
	)
{
	ShaderParameterVectorMap::iterator iterVector = mShaderParameterVectorMap.begin();
	while( iterVector != mShaderParameterVectorMap.end() )
	{
		ShaderParameter* shaderParam = iterVector->second;
		shaderParam->SetParameter( renderStateShadowing, effectPass, context );
		iterVector++;
	}

	ShaderParameterMatrixMap::iterator iterMatrix = mShaderParameterMatrixMap.begin();
	while( iterMatrix != mShaderParameterMatrixMap.end() )
	{
		ShaderParameter* shaderParam = iterMatrix->second;
		shaderParam->SetParameter( renderStateShadowing, effectPass, context );
		iterMatrix++;
	}

	ShaderParameterTextureMap::iterator iterTexture = mShaderParameterTextureMap.begin();
	while( iterTexture != mShaderParameterTextureMap.end() )
	{
		ShaderParameter* shaderParam = iterTexture->second;        
		shaderParam->SetParameter( renderStateShadowing, effectPass, context );		
		iterTexture++;
	}
}

#endif