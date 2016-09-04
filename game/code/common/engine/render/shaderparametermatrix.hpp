#ifndef SHADER_PARAMETER_MATRIX_HPP
#define SHADER_PARAMETER_MATRIX_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/render/shaderparameter.hpp"
#include "common/engine/system/assert.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

static const int MAX_MATRIX_ARRAY = 16;

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class ShaderProgram;
class EffectPass;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ShaderParameterMatrix : public ShaderParameter
{
public:
	ShaderParameterMatrix( int id, int size = 1 );
	~ShaderParameterMatrix();

	void SetData( Math::Matrix44 *data, int index = 0 )
	{
		Assert( index < MAX_MATRIX_ARRAY, "" );
		mData[index] = data;
	}

	Math::Matrix44* GetData( int index = 0 ) const
	{
		Assert( index < MAX_MATRIX_ARRAY, "" );
		return mData[index];
	}

	void SetHandle( int handle )
	{
		mHandle = handle;
	}

	const int GetHandle() const
	{
		return mHandle;
	}

	void SetParameter( 
		RenderStateShadowing* renderStateShadowing, 
		EffectPass* effectPass,
		void* context
		);
	int UpdateParameter( ResourceHandle<ShaderProgram> shaderProgram );
	bool operator != ( const ShaderParameter &other );

private:

	Math::Matrix44* mData[MAX_MATRIX_ARRAY];
	int mSize;
	int mHandle;
};
#endif
