#if defined( RENDER_PLAT_D3D9 )

#ifndef EFFECT_PROGRAM_HPP
#define EFFECT_PROGRAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/array.hpp"
#include <d3d9.h>
#include <d3dx9.h>
#include <list>

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

const int MAX_D3D_PARAMETERS = 50;

//////////////////////////////////////////////////////
// FOWARD DECLARATIONS
//////////////////////////////////////////////////////

enum ConstantType
{
    CONSTANT_TYPE_VERTEX,
    CONSTANT_TYPE_PIXEL
};

struct ConstantData
{
	//char mName[64];
    int mNameHash;
	int mSize;
	VariableType mVariableType;
	ConstantType mConstantType;
	D3DXHANDLE mHandle;
};

struct TextureData
{
	//char mName[64];
    int mNameHash;
	VariableType mVariableType;
	ConstantType mConstantType;
	int mSamplerIndex;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3DShaderProgram : public ShaderProgram
{
public:

    D3DShaderProgram( int id );
    virtual ~D3DShaderProgram();
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

protected:

	void SetParamVector( 
		D3DXHANDLE handle, 
		ConstantType constantType, 
		const Math::Vector4* value, 
		int numItems 
		);

	void SetParamMatrix( 
		D3DXHANDLE handle, 
		ConstantType constantType, 
		const Math::Matrix44* matrix,
		int numItems
		);

	void SetParamTexture( 
		const Texture* texture, 	
		int samplerIndex,
		RenderStateShadowing* renderStateShadowing,
		SamplerState* state,
		void* context
		);

	typedef std::list<ConstantData*> ConstantList;
	ConstantList mConstantList;		
	typedef std::list<TextureData> TextureList;
	TextureList mTextureList;		
};

#endif

#endif