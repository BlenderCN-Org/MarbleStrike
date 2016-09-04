#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/shaderparameter.hpp"
#include "common/engine/system/resourcehandle.hpp"
#include <map>

//////////////////////////////////////////////////////
// FOWARD DECLARATIONS
//////////////////////////////////////////////////////

class Texture;
class VertexShader;
class PixelShader;
class TextureParameter;
class SamplerState;
class RenderStateShadowing;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum VariableType
{
    VAR_TYPE_VECTOR,
    VAR_TYPE_MATRIX,
	VAR_TYPE_TEXTURE
};

const int SHADER_HANDLE_ERROR = -1;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ShaderProgram
{
public:

    ShaderProgram( int id );
    virtual ~ShaderProgram();

	virtual void Create() = 0;
    virtual void Destroy() = 0;
	virtual void Reload() = 0;

	virtual void DisableTextureSamplers( void* context ) = 0;    

	virtual void SetProgram(
		EffectPass* effectPass,
		RenderStateShadowing* renderStateShadowing,
		const std::map<int, const void*> &constantMap,
		int threadID,
		void* context 
		) = 0;
	
	void SetVertexShader( ResourceHandle<VertexShader> shader )
	{
		mVertexShaderHandle = shader;
	}

	void SetPixelShader( ResourceHandle<PixelShader> shader )
	{
		mPixelShaderHandle = shader;
	}

	ResourceHandle<VertexShader> GetVertexShader()
	{
		return mVertexShaderHandle;
	}

	ResourceHandle<PixelShader> GetPixelShader()
	{
		return mPixelShaderHandle;
	}

	int GetID()
	{
		return mID;
	}

protected:

	ResourceHandle<VertexShader> mVertexShaderHandle;
	ResourceHandle<PixelShader> mPixelShaderHandle;

	int mID;
};

#endif
