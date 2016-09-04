#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11blendstate.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3D11_BLEND gBlendTranslation[BLEND_FUNC_MAX] =
{   
	D3D11_BLEND_ZERO,				// Blend_Zero
	D3D11_BLEND_ONE,				// Blend_One
	D3D11_BLEND_SRC_COLOR,			// Blend_Src_Color
	D3D11_BLEND_INV_SRC_COLOR,		// Blend_Inv_Src_Color
	D3D11_BLEND_SRC_ALPHA,			// Blend_Src_Alpha
	D3D11_BLEND_INV_SRC_ALPHA,		// Blend_Inv_Src_Alpha
	D3D11_BLEND_DEST_ALPHA,			// Blend_Dest_Alpha
	D3D11_BLEND_INV_DEST_ALPHA,		// Blend_Inv_Dest_Alpha
	D3D11_BLEND_DEST_COLOR,			// Blend_Dest_Color
	D3D11_BLEND_INV_DEST_COLOR,		// Blend_Inv_Dest_Color
	D3D11_BLEND_SRC_ALPHA_SAT,		// Blend_Src_Alpha_Sat
	D3D11_BLEND_BLEND_FACTOR,		// Blend_Blend_Factor
	D3D11_BLEND_INV_BLEND_FACTOR,	// Blend_Inv_Blend_Factor
	D3D11_BLEND_SRC1_COLOR,  		// Blend_Src1_Color
	D3D11_BLEND_INV_SRC1_COLOR,		// Blend_Inv_Src1_Color
	D3D11_BLEND_SRC1_ALPHA,			// Blend_Src1_Alpha
	D3D11_BLEND_INV_SRC1_ALPHA,		// Blend_Inv_Src1_Alpha
};

static D3D11_BLEND_OP gBlendOpTranslation[BLENDOP_FUNC_MAX] =
{
	D3D11_BLEND_OP_ADD,				// BlendOp_Add
	D3D11_BLEND_OP_SUBTRACT,		// BlendOp_Subtract
	D3D11_BLEND_OP_REV_SUBTRACT,	// BlendOp_Rev_Subtract
	D3D11_BLEND_OP_MIN,				// BlendOp_Min
	D3D11_BLEND_OP_MAX,				// BlendOp_Max
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11BlendState::D3D11BlendState() 
{    
	mBlendState = NULL;
}

//============================================================================

D3D11BlendState::~D3D11BlendState()
{
	RELEASE_D3D_OBJECT( mBlendState );
}

//============================================================================

void D3D11BlendState::Dispatch( 
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

	const BlendState* currentState = renderStateShadowing->GetCurrentBlendState();
	
	if ( currentState == this )
	{
		return;
	}

	if ( ( currentState != this || forceUpload ) && mBlendState != NULL )
	{
		UINT sampleMask = 0xffffffff;
		deviceContext->OMSetBlendState( mBlendState, 0, sampleMask );
	}

	renderStateShadowing->SetCurrentBlendState( this );
}

//============================================================================

void D3D11BlendState::Create()
{
	if ( mBlendState == NULL )
	{
		mBlendDesc.AlphaToCoverageEnable = mData.mAlphaToCoverageEnable;
		mBlendDesc.IndependentBlendEnable = false;

		for ( int i = 0; i < MAX_BLEND_RENDER_TARGET; ++i )
		{
			mBlendDesc.RenderTarget[i].BlendEnable = mData.mBlendEnable[i];
			mBlendDesc.RenderTarget[i].SrcBlend = gBlendTranslation[mData.mSrcBlend];
			mBlendDesc.RenderTarget[i].DestBlend = gBlendTranslation[mData.mDestBlend];
			mBlendDesc.RenderTarget[i].BlendOp = gBlendOpTranslation[mData.mBlendOp];
			mBlendDesc.RenderTarget[i].SrcBlendAlpha = gBlendTranslation[mData.mSrcBlendAlpha];
			mBlendDesc.RenderTarget[i].DestBlendAlpha = gBlendTranslation[mData.mDestBlendAlpha];
			mBlendDesc.RenderTarget[i].BlendOpAlpha = gBlendOpTranslation[mData.mBlendOpAlpha];			
			mBlendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//static_cast<UINT8>( mData.mRenderTargetWriteMask[i] );
		}		

		D3D11Renderer::GetDevice()->CreateBlendState( &mBlendDesc, &mBlendState );
	}
}

#endif