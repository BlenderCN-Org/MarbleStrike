#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_GLSL_PROGRAM_HPP
#define FIXED_GLSL_PROGRAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/shaderprogram.hpp"

//////////////////////////////////////////////////////
// FOWARD DECLARATIONS
//////////////////////////////////////////////////////

class FixedGLSLPixelShader;
class FixedGLSLVertexShader;
class Texture;

//////////////////////////////////////////////////////
// ENUMS
//////////////////////////////////////////////////////

enum ShaderProgramType
{
	SHADER_PROGRAM_NO_LIGHT,
	SHADER_PROGRAM_LIGHT
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class FixedOGLShaderProgram : public ShaderProgram
{
public:

    FixedOGLShaderProgram( int id );
    virtual ~FixedOGLShaderProgram();

	void Create();
    void Destroy();
	void Reload()
	{

	}
    
	void DisableTextureSamplers( void* context );
    void SetProgram( 
		EffectPass* effectPass,
		RenderStateShadowing* renderStateShadowing,
		const std::map<int, const void*> &constantMap,
		int threadID,
		void* context 
		);

protected:

	void SetConstants(
		EffectPass* effectPass,
		RenderStateShadowing* renderStateShadowing, 
		const std::map<int, const void*> &constantMap,
		void* context 
		);	
	
	void SetConstantsNoLight(
		EffectPass* effectPass,
		RenderStateShadowing* renderStateShadowing, 
		const std::map<int, const void*> &constantMap,
		void* context 
		);	
	void SetConstantsLight(
		EffectPass* effectPass,
		RenderStateShadowing* renderStateShadowing, 
		const std::map<int, const void*> &constantMap,
		void* context 
		);	
    
	ShaderProgramType mShaderProgramType;
};

#endif

#endif