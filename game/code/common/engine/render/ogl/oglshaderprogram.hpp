#if defined( RENDER_PLAT_OPENGL )

#ifndef GLSL_PROGRAM_HPP
#define GLSL_PROGRAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include <list>

//////////////////////////////////////////////////////
// FOWARD DECLARATIONS
//////////////////////////////////////////////////////

class GLSLPixelShader;
class GLSLVertexShader;

//////////////////////////////////////////////////////
// FOWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct ConstantData
{
	//char mName[64];
    int mNameHash;
	VariableType mType;
	int mSize;
	int mHandle;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLShaderProgram : public ShaderProgram
{
public:

	enum VertexAttribStream
	{
		VERTEX_ARRAY, 	
		TEXCOORD_ARRAY,
		TEXCOORD1_ARRAY,
		COLOR_ARRAY,
		NORMAL_ARRAY, 
		TANGENT_ARRAY,
		BINORMAL_ARRAY
	};

    OGLShaderProgram( int id );
    virtual ~OGLShaderProgram();

	void Create();
    void Destroy();
	void Reload();

	void DisableTextureSamplers( void* context );

    void SetProgram( 
		EffectPass* effectPass,
		RenderStateShadowing* renderStateShadowing,
		const std::map<int, const void*> &constantMap, 
		int threadID,
		void* context 
		);

	int GetInVertex()
	{
		return mInVertex;
	}

	int GetInTexCoord( int index )
	{
		return mInTexCoord[index];
	}

	int GetInTangent()
	{
		return mInTangent;
	}

	int GetInColor()
	{
		return mInColor;
	}

	int GetInNormal()
	{
		return mInNormal;
	}

	int GetInBinormal()
	{
		return mInBinormal;
	}

protected:

	void LinkShaders();	
	void UnLinkShaders() const;	
	int GetAttribParamHandle( const char* paramName ) const;
	void SetParamVector( int handle, const Math::Vector4* value, int numItems ); 
	void SetParamMatrix( int handle, const Math::Matrix44* matrix, int numItems );	
	void SetParamTexture( int handle, const Texture* texture, int samplerIndex, SamplerState* state );	
    
    unsigned int mShaderProgram;
	int mInVertex;
	int mInTexCoord[2];
	int mInTangent;
	int mInColor;
	int mInNormal;
	int mInBinormal;

	typedef std::list<ConstantData> ConstantList;
	ConstantList mConstantList;
};

#endif

#endif