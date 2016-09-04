#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLES_SHADER_PROGRAM_HPP
#define OGLES_SHADER_PROGRAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogles2/ogleswrapper.hpp"
#include "common/engine/render/shaderprogram.hpp"

//////////////////////////////////////////////////////
// FOWARD DECLARATIONS
//////////////////////////////////////////////////////

class OGLESPixelShader;
class OGLESVertexShader;

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

class OGLESShaderProgram : public ShaderProgram
{
public:

	enum VertexAttribStream
	{
		VERTEX_ARRAY, 	
		TEXCOORD_ARRAY, 	
		COLOR_ARRAY,
		NORMAL_ARRAY, 
		TANGENT_ARRAY
	};
    
    OGLESShaderProgram( int id );
    virtual ~OGLESShaderProgram();

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

	int GetInTexCoord()
	{
		return mInTexCoord;
	}

	int GetInColor()
	{
		return mInColor;
	}

	int GetInNormal()
	{
		return mInNormal;
	}

protected:

	
	void LinkShaders();	
	void UnLinkShaders() const;	
	int GetAttribParamHandle( const char* paramName ) const;
	void SetParamVector( int handle, const Math::Vector4* value, int numItems ); 	
	void SetParamMatrix( int handle, const Math::Matrix44* matrix, int numItems );
	void SetParamTexture( 
		int handle, 
		const Texture* texture, 
		int samplerIndex, 
		ResourceHandle<SamplerState> samplerStateHandle
		);
    
    unsigned int mShaderProgram;
	int mInVertex;
	int mInTexCoord;
	int mInColor;
	int mInNormal;

	typedef std::list<ConstantData> ConstantList;
	ConstantList mConstantList;
};

#endif

#endif