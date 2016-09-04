//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/shaderparametermatrix.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ShaderParameterMatrix::ShaderParameterMatrix( int id, int size )
	:	ShaderParameter( id )
{	
	mSize = size;
}

//============================================================================

ShaderParameterMatrix::~ShaderParameterMatrix()
{
}

//============================================================================

void ShaderParameterMatrix::SetParameter( 
	RenderStateShadowing* /*renderStateShadowing*/, 
	EffectPass* /*effectPass*/,
	void* /*context*/
	)
{
	//ShaderProgram* shaderProgram = effectPass->GetShaderProgram();

	//if ( mSize > 1 )
	//{
	//	shaderProgram->SetParamMatrixArray( GetHandle(), mSize, mData[0] );
	//}
	//else
	//{
	//	shaderProgram->SetParamMatrix( GetHandle(), *GetData() );
	//}
}

//============================================================================

int ShaderParameterMatrix::UpdateParameter( ResourceHandle<ShaderProgram> /*shaderProgram*/ )
{
	//int handle = shaderProgram->GetParamHandle( GetName() );
	//SetHandle( handle );

	//return handle;	
	return 0;
}

//============================================================================

bool ShaderParameterMatrix::operator != ( const ShaderParameter &other )
{	
	const ShaderParameterMatrix* otherData = reinterpret_cast< const ShaderParameterMatrix*>( &other );
	if ( mSize != otherData->mSize )
	{	
		return true;
	}
	else
	{
		for ( int i = 0 ; i < mSize; ++i )
		{
			if ( *mData[i] != *otherData->mData[i] )
			{
				return true;
			}
		}

		return false;
	}
}