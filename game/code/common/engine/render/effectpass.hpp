#ifndef EFFECT_PASS_HPP
#define EFFECT_PASS_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/assert.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include <map>
#include <string>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class VertexShader;
class PixelShader;
class DepthStencilState;
class BlendState;
class RasterizerState;
class ShaderProgram;
class SamplerState;
class RenderStateShadowing;

///////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class EffectPass
{
public:

	EffectPass();
	~EffectPass();

	void Dispatch( 
		RenderStateShadowing* renderStateShadowing,
		const std::map<int, const void*> &constantMap,
		int threadID,
		void* context
		);
	bool IsTranslucent() const;

	void SetShaderProgram( ResourceHandle<ShaderProgram> program )
	{
		mShaderProgramHandle = program;
	}

	void SetDepthStencilState( ResourceHandle<DepthStencilState> state )
	{
		mDepthStencilState = state;
	}

	void SetBlendState( ResourceHandle<BlendState> state )
	{
		mBlendState = state;
	}

	ResourceHandle<BlendState> GetBlendState()
	{
		return mBlendState;
	}

	void SetRasterizerState( ResourceHandle<RasterizerState> state )
	{
		mRasterizerState = state;
	}	

	void SetSamplerState( int nameHash, ResourceHandle<SamplerState> state )
	{
		mSamplerStateMap[nameHash] = state;
	}

	const char* GetName()
	{
		return mName;
	}

	void SetName( const char* name );
	
	//const char* GetVertexShaderName()
	//{
	//	return mVertexShaderName;
	//}

	void SetVertexShaderName( const char* name );

	//const char* GetPixelShaderName()
	//{
	//	return mPixelShaderName;
	//}

	void SetPixelShaderName( const char* name );

	ResourceHandle<ShaderProgram> GetShaderProgram()
	{
		return mShaderProgramHandle;
	}

	ResourceHandle<SamplerState> GetSamplerState( int nameHash )
	{
		return mSamplerStateMap[nameHash];
	}

	void AddSamplerStatePair( int nameHash, ResourceHandle<SamplerState> state );

	void Reload();

private:

	char mName[32];	
	char mPixelShaderName[64];
	char mVertexShaderName[64];

	std::map< int, ResourceHandle<SamplerState> > mSamplerStateMap;	
	ResourceHandle<ShaderProgram> mShaderProgramHandle;
	ResourceHandle<DepthStencilState> mDepthStencilState;
	ResourceHandle<BlendState> mBlendState;
	ResourceHandle<RasterizerState> mRasterizerState;


};

#endif
