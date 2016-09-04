#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11rasterizerstate.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3D11_CULL_MODE gCullModeTranslation[] =
{
	D3D11_CULL_NONE,	// CullMode_None
	D3D11_CULL_FRONT,	// CullMode_Front
	D3D11_CULL_BACK		// CullMode_Back	
};

static D3D11_FILL_MODE gFillModeTranslation[] =
{
	D3D11_FILL_WIREFRAME,	// FillMode_Wireframe
	D3D11_FILL_SOLID		// FillMode_Solid
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11RasterizerState::D3D11RasterizerState() 
{    
	mRasterizerState = NULL;
}

//============================================================================

D3D11RasterizerState::~D3D11RasterizerState()
{
	RELEASE_D3D_OBJECT( mRasterizerState );
}

//============================================================================

void D3D11RasterizerState::Dispatch( 
	RenderStateShadowing* renderStateShadowing, 
	void* context,
	bool forceUpload 
	) 
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	const RasterizerState* currentState = renderStateShadowing->GetCurrentRasterizerState();
	
	if ( currentState == this )
	{
		return;
	}

	if ( ( currentState != this || forceUpload ) && mRasterizerState != NULL )
	{
		deviceContext->RSSetState( mRasterizerState );
	}

	renderStateShadowing->SetCurrentRasterizerState( this );
}

//============================================================================

void D3D11RasterizerState::Create()
{
	if ( mRasterizerState == NULL )
	{
		mRasterizerDesc.FillMode = gFillModeTranslation[mData.mFillMode];
		mRasterizerDesc.CullMode = gCullModeTranslation[mData.mCullMode];
		mRasterizerDesc.FrontCounterClockwise = mData.mFrontCounterClockwise;
		mRasterizerDesc.DepthBias = mData.mDepthBias;
		mRasterizerDesc.DepthBiasClamp = mData.mDepthBiasClamp;
		mRasterizerDesc.SlopeScaledDepthBias = mData.mSlopeScaledDepthBias;
		mRasterizerDesc.DepthClipEnable = TRUE;
		mRasterizerDesc.ScissorEnable = mData.mScissorEnable;
		mRasterizerDesc.MultisampleEnable = mData.mMultisampleEnable;
		mRasterizerDesc.AntialiasedLineEnable = mData.mAntialiasedLineEnable;

		D3D11Renderer::GetDevice()->CreateRasterizerState( &mRasterizerDesc, &mRasterizerState );
	}
}

#endif