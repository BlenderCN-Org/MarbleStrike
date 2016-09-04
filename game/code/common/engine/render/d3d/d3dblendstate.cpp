#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d/d3dblendstate.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include <d3d9.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3DBLEND gBlendTranslation[BLEND_FUNC_MAX] =
{   
	D3DBLEND_ZERO,			// Blend_Zero
	D3DBLEND_ONE,			// Blend_One
	D3DBLEND_SRCCOLOR,		// Blend_Src_Color
	D3DBLEND_INVSRCCOLOR,	// Blend_Inv_Src_Color
	D3DBLEND_SRCALPHA,		// Blend_Src_Alpha
	D3DBLEND_INVSRCALPHA,	// Blend_Inv_Src_Alpha
	D3DBLEND_DESTALPHA,		// Blend_Dest_Alpha
	D3DBLEND_INVDESTALPHA,	// Blend_Inv_Dest_Alpha
	D3DBLEND_DESTCOLOR,		// Blend_Dest_Color
	D3DBLEND_INVDESTCOLOR,	// Blend_Inv_Dest_Color
	D3DBLEND_SRCALPHASAT,	// Blend_Src_Alpha_Sat
	D3DBLEND_ZERO,			// Blend_Blend_Factor
	D3DBLEND_ZERO,			// Blend_Inv_Blend_Factor
	D3DBLEND_ZERO,			// Blend_Src1_Color
	D3DBLEND_ZERO,			// Blend_Inv_Src1_Color
	D3DBLEND_ZERO,			// Blend_Src1_Alpha
	D3DBLEND_ZERO,			// Blend_Inv_Src1_Alpha
};

static D3DBLENDOP gBlendOpTranslation[BLENDOP_FUNC_MAX] =
{
	D3DBLENDOP_ADD,			// BlendOp_Add
	D3DBLENDOP_SUBTRACT,	// BlendOp_Subtract
	D3DBLENDOP_REVSUBTRACT,	// BlendOp_Rev_Subtract
	D3DBLENDOP_MIN,			// BlendOp_Min
	D3DBLENDOP_MAX,			// BlendOp_Max
};

static D3DCMPFUNC gCompareFunctionTranslation[] =
{	
	D3DCMP_NEVER,			// AlphaFuncCompare_Never
	D3DCMP_LESS,			// AlphaFuncCompare_Less
	D3DCMP_EQUAL,			// AlphaFuncCompare_Equal
	D3DCMP_LESSEQUAL,		// AlphaFuncCompare_Less_Equal
	D3DCMP_GREATER,			// AlphaFuncCompare_Greater
	D3DCMP_NOTEQUAL,		// AlphaFuncCompare_Not_Equal
	D3DCMP_GREATEREQUAL,	// AlphaFuncCompare_Greater_Equal
	D3DCMP_ALWAYS,			// AlphaFuncCompare_Always
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DBlendState::D3DBlendState() 
{    
}

//============================================================================

D3DBlendState::~D3DBlendState()
{
}

//============================================================================

void D3DBlendState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context, 
	bool forceUpload 
	)
{		
	UNUSED_ALWAYS( context );

	const BlendState* currentState = renderStateShadowing->GetCurrentBlendState();
	const BlendStateStruct* currentStateData = NULL;

	if ( currentState == this )
	{
		return;
	}

	if ( currentState != NULL )
	{
		currentStateData = currentState->GetData();
	}
	else
	{
		forceUpload = true;
	}

	if ( forceUpload == true || mData.mBlendEnable[0] != currentStateData->mBlendEnable[0] )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, mData.mBlendEnable[0] );
	}

	if ( forceUpload == true || mData.mSrcBlend != currentStateData->mSrcBlend )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_SRCBLEND, gBlendTranslation[mData.mSrcBlend] );
	}

	if ( forceUpload == true || mData.mDestBlend != currentStateData->mDestBlend )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_DESTBLEND, gBlendTranslation[mData.mDestBlend] );
	}

	if ( forceUpload == true || mData.mBlendOp != currentStateData->mBlendOp )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_BLENDOP, gBlendOpTranslation[mData.mBlendOp] );
	}

	if ( forceUpload == true || mData.mSrcBlendAlpha != currentStateData->mSrcBlendAlpha )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_SRCBLENDALPHA, gBlendTranslation[mData.mSrcBlendAlpha] );
	}

	if ( forceUpload == true || mData.mDestBlendAlpha != currentStateData->mDestBlendAlpha )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_DESTBLENDALPHA, gBlendTranslation[mData.mDestBlendAlpha] );
	}

	if ( forceUpload == true || mData.mDestBlendAlpha != currentStateData->mDestBlendAlpha )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_BLENDOPALPHA, gBlendOpTranslation[mData.mDestBlendAlpha] );
	}

	if ( forceUpload == true || mData.mAlphaTestEnable != currentStateData->mAlphaTestEnable )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_ALPHATESTENABLE, mData.mAlphaTestEnable );
	}

	if ( forceUpload == true || mData.mAlphaFunc != currentStateData->mAlphaFunc )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_ALPHAFUNC, gCompareFunctionTranslation[mData.mAlphaFunc] );
	}

	if ( forceUpload == true || mData.mAlphaRef != currentStateData->mAlphaRef )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_ALPHAREF, mData.mAlphaRef );
	}

	renderStateShadowing->SetCurrentBlendState( this );
}

#endif