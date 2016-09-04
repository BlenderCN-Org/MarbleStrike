#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11depthstencilstate.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3D11_COMPARISON_FUNC gCompareFunctionTranslation[] =
{	
	D3D11_COMPARISON_NEVER,			//CompareFunc_Never
	D3D11_COMPARISON_LESS,			//CompareFunc_Less
	D3D11_COMPARISON_EQUAL,			//CompareFunc_Equal
	D3D11_COMPARISON_LESS_EQUAL,	//CompareFunc_Less_Equal
	D3D11_COMPARISON_GREATER,		//CompareFunc_Greater
	D3D11_COMPARISON_NOT_EQUAL,		//CompareFunc_Not_Equal
	D3D11_COMPARISON_GREATER_EQUAL,	//CompareFunc_Greater_Equal
	D3D11_COMPARISON_ALWAYS,		//CompareFunc_Always
};

static D3D11_STENCIL_OP gStencilOpTranslation[] =
{
	D3D11_STENCIL_OP_KEEP,		//StencilOp_Keep
	D3D11_STENCIL_OP_ZERO,		//StencilOp_Zero
	D3D11_STENCIL_OP_REPLACE,	//StencilOp_Replace
	D3D11_STENCIL_OP_INCR_SAT,	//StencilOp_Increase_Saturate
	D3D11_STENCIL_OP_DECR_SAT,	//StencilOp_Decrease_Saturate
	D3D11_STENCIL_OP_INVERT,	//StencilOp_Invert
	D3D11_STENCIL_OP_INCR,		//StencilOp_Increase
	D3D11_STENCIL_OP_DECR,		//StencilOp_Decrease
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11DepthStencilState::D3D11DepthStencilState() 
{   
	mDepthStencilState = NULL;
}

//============================================================================

D3D11DepthStencilState::~D3D11DepthStencilState()
{
	RELEASE_D3D_OBJECT( mDepthStencilState );
}

//============================================================================

void D3D11DepthStencilState::Dispatch( 
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

	const DepthStencilState* currentState = renderStateShadowing->GetCurrentDepthStencilState();

	if ( currentState == this )
	{
		return;
	}

	if ( ( currentState != this || forceUpload ) && mDepthStencilState != NULL )
	{
		deviceContext->OMSetDepthStencilState( mDepthStencilState, mData.mStencilRef );
	}

	renderStateShadowing->SetCurrentDepthStencilState( this );
}

//============================================================================

void D3D11DepthStencilState::Create()
{
	if ( mDepthStencilState == NULL )
	{
		mDepthStencilDesc.DepthEnable = mData.mDepthEnable;

		D3D11_DEPTH_WRITE_MASK depthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		if ( mData.mDepthWriteMask == true )
		{
			depthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		}
		mDepthStencilDesc.DepthWriteMask = depthWriteMask;
		mDepthStencilDesc.DepthFunc = gCompareFunctionTranslation[mData.mDepthFunc];

		mDepthStencilDesc.StencilEnable = mData.mStencilEnable;
		mDepthStencilDesc.StencilReadMask = static_cast<UINT8>( mData.mStencilReadMask );
		mDepthStencilDesc.StencilWriteMask = static_cast<UINT8>( mData.mStencilWriteMask );

		mDepthStencilDesc.FrontFace.StencilFailOp = gStencilOpTranslation[mData.mFrontFace.mStencilFailOp];
		mDepthStencilDesc.FrontFace.StencilDepthFailOp = gStencilOpTranslation[mData.mFrontFace.mStencilDepthFailOp];
		mDepthStencilDesc.FrontFace.StencilPassOp = gStencilOpTranslation[mData.mFrontFace.mStencilPassOp];
		mDepthStencilDesc.FrontFace.StencilFunc = gCompareFunctionTranslation[mData.mFrontFace.mStencilFunc];

		mDepthStencilDesc.BackFace.StencilFailOp = gStencilOpTranslation[mData.mBackFace.mStencilFailOp];
		mDepthStencilDesc.BackFace.StencilDepthFailOp = gStencilOpTranslation[mData.mBackFace.mStencilDepthFailOp];
		mDepthStencilDesc.BackFace.StencilPassOp = gStencilOpTranslation[mData.mBackFace.mStencilPassOp];
		mDepthStencilDesc.BackFace.StencilFunc = gCompareFunctionTranslation[mData.mBackFace.mStencilFunc];

		D3D11Renderer::GetDevice()->CreateDepthStencilState( &mDepthStencilDesc, &mDepthStencilState );
	}
}

#endif