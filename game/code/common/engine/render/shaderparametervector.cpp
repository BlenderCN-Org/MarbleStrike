//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/shaderparametervector.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/effectpass.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ShaderParameterVector::ShaderParameterVector( int id )
	:	ShaderParameter( id )
{
}

//============================================================================

ShaderParameterVector::~ShaderParameterVector()
{

}

//============================================================================

void ShaderParameterVector::SetParameter( 
	RenderStateShadowing* /*renderStateShadowing*/, 
	EffectPass* /*effectPass*/,
	void* /*context*/
	)
{
	//ShaderProgram* shaderProgram = effectPass->GetShaderProgram();
	//shaderProgram->SetParamVector( GetHandle(), GetData(), GetNumItems() );
}

//============================================================================

int ShaderParameterVector::UpdateParameter( ResourceHandle<ShaderProgram> /*shaderProgram*/ )
{
	//int handle = shaderProgram->GetParamHandle( GetName() );
	//SetHandle( handle );

	//return handle;	
	return 0;
}

//============================================================================

bool ShaderParameterVector::operator != ( const ShaderParameter &other )
{
	const ShaderParameterVector* otherData = reinterpret_cast< const ShaderParameterVector*>( &other );
	if ( *mData != *otherData->mData )
	{
		return true;
	}
	else
	{
		return false;
	}
}