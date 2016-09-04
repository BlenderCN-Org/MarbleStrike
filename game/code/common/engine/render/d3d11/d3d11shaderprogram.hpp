#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_SHADER_PROGRAM_HPP
#define D3D11_SHADER_PROGRAM_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/vector4.hpp"
#include "common/engine/render/shaderprogram.hpp"
#include "common/engine/system/array.hpp"
#include "common/engine/system/resourcehandle.hpp"
#include "common/engine/system/utilities.hpp"
#include <d3d11.h>
#include <list>

//////////////////////////////////////////////////////
// FOWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderStateShadowing;
class D3D11VertexShader;
class D3D11PixelShader;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum ConstantType
{
    CONSTANT_TYPE_VERTEX,
    CONSTANT_TYPE_PIXEL
};

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct ConstantData
{
	//char mName[64];
    int mNameHash;
	int mSize;
	int mOffset;
	void* mData;
};

struct TextureData
{
	//char mName[64];
    int mNameHash;
	int mSamplerIndex;
};

typedef std::list<ConstantData> ConstantList;
typedef std::list<TextureData> TextureList;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11ShaderProgram : public ShaderProgram
{
public:

    D3D11ShaderProgram( int id );
    virtual ~D3D11ShaderProgram();

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

private:

	void SetParamTexture( 		
		const Texture* texture, 
		int samplerIndex,
		RenderStateShadowing* renderStateShadowing,		
		ResourceHandle<SamplerState> samplerStateHandle, 
		void* context
		);

	ConstantList* mVertexConstantList;
	ConstantList* mPixelConstantList;
	TextureList* mTextureList;

	unsigned char** mVertexData;
	int mVertexBufferSize;
	unsigned char** mPixelData;
	int mPixelBufferSize;

	ID3D11Buffer** mD3D11VertexBuffer;
	ID3D11Buffer** mD3D11PixelBuffer;

    static D3D11VertexShader* mCurrentVertexShader;
    static D3D11PixelShader* mCurrentPixelShader;

	int mNumThreads;
};

#endif

#endif