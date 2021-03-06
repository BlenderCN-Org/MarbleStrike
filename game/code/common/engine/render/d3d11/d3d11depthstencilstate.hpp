#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_DEPTH_STENCIL_STATE_HPP
#define D3D11_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/depthstencilstate.hpp"
#include <d3d11.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderStateShadowing;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11DepthStencilState : public DepthStencilState
{
public:

	D3D11DepthStencilState();
	~D3D11DepthStencilState();

	void Dispatch( 
		RenderStateShadowing* renderStateShadowing, 
		void* context,
		bool forceUpload = false 
		);
		
private:

	void Create();

	D3D11_DEPTH_STENCIL_DESC  mDepthStencilDesc;
	ID3D11DepthStencilState* mDepthStencilState;

};

#endif

#endif