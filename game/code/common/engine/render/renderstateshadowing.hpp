#ifndef RENDER_STATE_SHADOWING_HPP
#define RENDER_STATE_SHADOWING_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/samplerstate.hpp"
#include "common/engine/system/hashtable.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/math/matrix44.hpp"

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_NUM_SHADERS = 30;
const int MAX_CONSTANT_VECTOR_HANDLES = 20;
const int MAX_CONSTANT_MATRIX_HANDLES = 20;
const int MAX_SHADER_TYPE = 2;

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class DepthStencilState;
class BlendState;
class RasterizerState;
class SamplerState;
class RenderTarget;
class EffectPass;
class ShaderParameter;
class Texture;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RenderStateShadowing
{
public:
    
    enum RenderTargetType
    {
        RENDER_TARGET_TYPE_MAIN,
        RENDER_TARGET_TYPE_OTHER,
        RENDER_TARGET_TYPE_NONE
    };
    
    RenderStateShadowing();
	~RenderStateShadowing();

	void Clear();

	const DepthStencilState* GetCurrentDepthStencilState() const;
	void SetCurrentDepthStencilState( const DepthStencilState* state );
	
	const BlendState* GetCurrentBlendState() const;
	void SetCurrentBlendState( const BlendState* data );
	
	const RasterizerState* GetCurrentRasterizerState() const;
	void SetCurrentRasterizerState( const RasterizerState* data );
	
	const SamplerState* GetCurrentSamplerState( int index ) const;
	void SetCurrentSamplerState( int index, const SamplerState* data );
    
    const RenderTarget* GetCurrentRenderTarget() const;
    void SetCurrentRenderTarget( RenderTarget* renderTarget );
    
    RenderTargetType GetCurrentRenderTargetType() const;
    void SetCurrentRenderTargetType( RenderTargetType type );

	const EffectPass* GetCurrentEffectPass() const;
	void SetCurrentEffectPass( const EffectPass* pass );

	void SetCurrentShaderParameterVector( int shaderType, int shaderHandle, int constantHandle, Math::Vector4 data );
    Math::Vector4 GetCurrentShaderParameterVector( int shaderType, int shaderHandle, int constantHandle );
    
    void SetCurrentShaderParameterMatrix( int shaderType, int shaderHandle, int constantHandle, Math::Matrix44 data );
    Math::Matrix44 GetCurrentShaderParameterMatrix( int shaderType, int shaderHandle, int constantHandle );
    
    void SetShaderProgram( unsigned int id );
    unsigned int GetShaderProgram();

private:

	const DepthStencilState* mCurrentDepthStencilState;
	const BlendState* mCurrentBlendState;
	const RasterizerState* mCurrentRasterizerState;
	const SamplerState** mCurrentSamplerState;
    const RenderTarget* mCurrentRenderTarget;
    RenderTargetType mCurrentRenderTargetType;
	const EffectPass* mCurrentEffectPass;
    
    int mShaderProgram;

    Math::Vector4 mVectorParameterTable[MAX_SHADER_TYPE][MAX_NUM_SHADERS][MAX_CONSTANT_VECTOR_HANDLES];
    Math::Matrix44 mMatrixParameterTable[MAX_SHADER_TYPE][MAX_NUM_SHADERS][MAX_CONSTANT_MATRIX_HANDLES];
};

#endif
