#ifndef SHADER_PARAMETER_FLOAT_HPP
#define SHADER_PARAMETER_FLOAT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector4.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/render/shaderparameter.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class ShaderProgram;
class EffectPass;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ShaderParameterVector : public ShaderParameter
{
public:
	ShaderParameterVector( int id );
	~ShaderParameterVector();
	
	void SetData( Math::Vector4* data, int numItems )
	{
		mData = data;
		mNumItems = numItems;
	}

	Math::Vector4* GetData()
	{
		return mData;
	}

	void SetHandle( int handle )
	{
		mHandle = handle;
	}

	int GetHandle()
	{
		return mHandle;
	}

	int GetNumItems()
	{
		return mNumItems;
	}

	void SetParameter( 
		RenderStateShadowing* renderStateShadowing, 
		EffectPass* effectPass,
		void* context
		);
	int UpdateParameter( ResourceHandle<ShaderProgram> shaderProgram );
	bool operator != ( const ShaderParameter &other );

private:

	Math::Vector4* mData;
	int mHandle;
	int mNumItems;
};
#endif
