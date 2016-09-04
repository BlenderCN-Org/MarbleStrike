#ifndef RENDER_STATE_MANAGER_HPP
#define RENDER_STATE_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/samplerstate.hpp"
#include "common/engine/system/hashtable.hpp"
#include "common/engine/system/resourcehandle.hpp"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class DepthStencilState;
class BlendState;
class RasterizerState;
class SamplerState;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RenderStateManager
{
public:
    
    RenderStateManager();
	~RenderStateManager();

	ResourceHandle<DepthStencilState> GetDepthStencilState( const char* name );
	ResourceHandle<BlendState> GetBlendState( const char* name );
	ResourceHandle<RasterizerState> GetRasterizerState( const char* name );
	ResourceHandle<SamplerState> GetSamplerState( const char* name );

	DepthStencilState* GetDepthStencilStateByHandle( ResourceHandle<DepthStencilState> handle );
	BlendState* GetBlendStateByHandle( ResourceHandle<BlendState> handle );
	RasterizerState* GetRasterizerStateByHandle( ResourceHandle<RasterizerState> handle );
	SamplerState* GetSamplerStateByHandle( ResourceHandle<SamplerState> handle );
    
private:

	HashTable< DepthStencilState* > mDepthStencilStates;
	HashTable< BlendState* > mBlendStates;
	HashTable< RasterizerState* > mRasterizerStates;
	HashTable< SamplerState* > mSamplerStates;
};

#endif
