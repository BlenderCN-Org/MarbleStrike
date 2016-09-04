//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/constantbuffer.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/shaderparametermatrix.hpp"
#include "common/engine/render/shaderparametertexture.hpp"
#include "common/engine/render/shaderparametervector.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ConstantBuffer::ConstantBuffer( ConstantBufferType type )
{
	mConstantBufferType = type;
	mConstantBufferData = NULL;
	mConstantBufferDataSize = 0;
	mUniqueVectorID = 0;
    mUniqueMatrixID = 0;
    mUniqueTextureID = 0;
}

//============================================================================

ConstantBuffer::~ConstantBuffer()
{
	ShaderParameterVectorMap::iterator iterVector = mShaderParameterVectorMap.begin();
	while( iterVector != mShaderParameterVectorMap.end() )
	{
		DELETE_PTR( iterVector->second );
		iterVector++;
	}
	mShaderParameterVectorMap.clear();	

	ShaderParameterMatrixMap::iterator iterMatrix = mShaderParameterMatrixMap.begin();
	while( iterMatrix != mShaderParameterMatrixMap.end() )
	{
		DELETE_PTR( iterMatrix->second );
		iterMatrix++;
	}
	mShaderParameterMatrixMap.clear();	

	ShaderParameterTextureMap::iterator iterTexture = mShaderParameterTextureMap.begin();
	while( iterTexture != mShaderParameterTextureMap.end() )
	{
		DELETE_PTR( iterTexture->second );
		iterTexture++;
	}
	mShaderParameterTextureMap.clear();	
}

//============================================================================

void ConstantBuffer::SetConstantBufferData( void* data, int size )
{
	mConstantBufferData = data;
	mConstantBufferDataSize = size;
}

//============================================================================

void ConstantBuffer::SetVectorParameter(
	int nameHash, 
	Math::Vector4 *data, 
	int numItems
	)
{    
	ShaderParameterVector* findParam = GetParameterVector( nameHash );
	if ( findParam == NULL )
	{
		ShaderParameterVector* newParam = NEW_PTR( "Shader Parameter Vector" ) ShaderParameterVector( mUniqueVectorID );
		mUniqueVectorID++;
		//newParam->SetName( name );
		newParam->SetData( data, numItems );

		mShaderParameterVectorMap[nameHash] = newParam;
	}
	else
	{
		Assert( false, "Error: Trying to add an already set parameter." );
	}
}

//============================================================================

void ConstantBuffer::SetMatrixParameter( 
	int nameHash, 
	Math::Matrix44* data, 
	int /*index*/
	)
{       
	ShaderParameterMatrix* findParam = GetParameterMatrix( nameHash );
	if ( findParam == NULL )
	{
		ShaderParameterMatrix* newParam = NEW_PTR( "Shader Parameter Matrix" ) ShaderParameterMatrix( mUniqueMatrixID );
		mUniqueMatrixID++;
		//newParam->SetName( name );
		newParam->SetData( data );

		mShaderParameterMatrixMap[nameHash] = newParam;
	}
	else
	{
		Assert( false, "Error: Trying to add an already set parameter." );
	}
}

//============================================================================

void ConstantBuffer::SetTextureParameter( 
	int nameHash, 
	Texture** data
	)
{    
	ShaderParameterTexture* findParam = GetParameterTexture( nameHash );
	if ( findParam == NULL )
	{
		ShaderParameterTexture* newParam = NEW_PTR( "Shader Parameter Texture" ) ShaderParameterTexture( mUniqueTextureID );
		mUniqueTextureID++;
		//newParam->SetName( name );
		newParam->SetData( data );

		mShaderParameterTextureMap[nameHash] = newParam;
	}
	else
	{
		Assert( false, "Error: Trying to add an already set parameter." );
	}
}

//===========================================================================

ShaderParameterVector* ConstantBuffer::GetParameterVector( int nameHash )
{
	ShaderParameterVectorMap::iterator iter = mShaderParameterVectorMap.find( nameHash );

	if ( iter != mShaderParameterVectorMap.end() )
	{
		ShaderParameterVector* param = iter->second;
		return param;
	}

	return NULL;
}

//===========================================================================

ShaderParameterMatrix* ConstantBuffer::GetParameterMatrix( int nameHash )
{    
	ShaderParameterMatrixMap::iterator iter = mShaderParameterMatrixMap.find( nameHash );

	if ( iter != mShaderParameterMatrixMap.end() )
	{
		ShaderParameterMatrix* param = iter->second;
		return param;
	}

	return NULL;
}

//===========================================================================

ShaderParameterTexture* ConstantBuffer::GetParameterTexture( int nameHash )
{
	ShaderParameterTextureMap::iterator iter = mShaderParameterTextureMap.find( nameHash );

	if ( iter != mShaderParameterTextureMap.end() )
	{
		ShaderParameterTexture* param = iter->second;
		return param;
	}

	return NULL;
}

//===========================================================================

void ConstantBuffer::DispatchInternal( 
	RenderStateShadowing* renderStateShadowing, 
	EffectPass* effectPass,
	void* context
	)
{
	ResourceHandle<ShaderProgram> shaderProgramHandle = effectPass->GetShaderProgram();	
	
	if ( mShaderProgramHandle.IsNull() )
	{		
		mShaderProgramHandle = shaderProgramHandle;
		UpdateParametersVector( shaderProgramHandle );	
		UpdateParametersTexture( shaderProgramHandle );	
		UpdateParametersMatrix( shaderProgramHandle );
	}
	else if ( mShaderProgramHandle != shaderProgramHandle )
	{
		mShaderProgramHandle = shaderProgramHandle;		
		UpdateParametersVector( shaderProgramHandle );	
		UpdateParametersTexture( shaderProgramHandle );	
		UpdateParametersMatrix( shaderProgramHandle );
	}

	UploadParameters( renderStateShadowing, effectPass, context );
}

//===========================================================================

void ConstantBuffer::UpdateParametersVector( ResourceHandle<ShaderProgram> shaderProgram )
{
	ShaderParameterVectorMap::iterator floatIter = mShaderParameterVectorMap.begin();
	while( floatIter != mShaderParameterVectorMap.end() )
	{
		ShaderParameterVector* param = floatIter->second;
		param->UpdateParameter( shaderProgram );
		floatIter++;
	}

}

//===========================================================================

void ConstantBuffer::UpdateParametersTexture( ResourceHandle<ShaderProgram> shaderProgramHandle )
{
	ShaderParameterTextureMap::iterator textureIter = mShaderParameterTextureMap.begin();
	while( textureIter != mShaderParameterTextureMap.end() )
	{
		ShaderParameterTexture* param = textureIter->second;
		param->UpdateParameter( shaderProgramHandle );
		textureIter++;
	}
}

//===========================================================================

void ConstantBuffer::UpdateParametersMatrix( ResourceHandle<ShaderProgram> shaderProgramHandle )
{
	ShaderParameterMatrixMap::iterator matrixIter = mShaderParameterMatrixMap.begin();
	while( matrixIter != mShaderParameterMatrixMap.end() )
	{
		ShaderParameterMatrix* param = matrixIter->second;
		param->UpdateParameter( shaderProgramHandle );
		matrixIter++;
	}
}

//===========================================================================

void ConstantBuffer::UploadParameters( 
	RenderStateShadowing* renderStateShadowing, 
	EffectPass* effectPass,
	void* context
	)
{
	ResourceHandle<ShaderProgram> shaderProgramHandle = effectPass->GetShaderProgram();	
	ShaderProgram* shaderProgram = GameApp::Get()->GetShaderManager()->GetShaderProgramByHandle( shaderProgramHandle );

	int shaderID = shaderProgram->GetID();

	ShaderParameterVectorMap::iterator iterVector = mShaderParameterVectorMap.begin();
	while( iterVector != mShaderParameterVectorMap.end() )
	{
		ShaderParameter* shaderParam = iterVector->second;
        Math::Vector4 shaderParamData = *( reinterpret_cast<ShaderParameterVector*>(shaderParam) )->GetData();

		int paramID = shaderParam->GetID();
        Math::Vector4 currentParam = renderStateShadowing->GetCurrentShaderParameterVector( mConstantBufferType, shaderID, paramID );
		if ( currentParam != shaderParamData )
		{
			shaderParam->SetParameter( renderStateShadowing, effectPass, context );
			renderStateShadowing->SetCurrentShaderParameterVector( mConstantBufferType, shaderID, paramID, shaderParamData );
		}
		iterVector++;
	}

	ShaderParameterMatrixMap::iterator iterMatrix = mShaderParameterMatrixMap.begin();
	while( iterMatrix != mShaderParameterMatrixMap.end() )
	{
		ShaderParameter* shaderParam = iterMatrix->second;
        Math::Matrix44 shaderParamData = *( reinterpret_cast<ShaderParameterMatrix*>(shaderParam) )->GetData();
        
		int paramID = shaderParam->GetID();
        Math::Matrix44 currentParam = renderStateShadowing->GetCurrentShaderParameterMatrix( mConstantBufferType, shaderID, paramID );
		if ( currentParam != shaderParamData )
		{
			shaderParam->SetParameter( renderStateShadowing, effectPass, context );
			renderStateShadowing->SetCurrentShaderParameterMatrix( mConstantBufferType, shaderID, paramID, shaderParamData );
		}
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