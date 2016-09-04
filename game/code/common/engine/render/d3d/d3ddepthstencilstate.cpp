#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d/d3ddepthstencilstate.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include <d3d9.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3DCMPFUNC gCompareFunctionTranslation[] =
{	
	D3DCMP_NEVER,			//CompareFunc_Never
	D3DCMP_LESS,			//CompareFunc_Less
	D3DCMP_EQUAL,			//CompareFunc_Equal
	D3DCMP_LESSEQUAL,		//CompareFunc_Less_Equal
	D3DCMP_GREATER,			//CompareFunc_Greater
	D3DCMP_NOTEQUAL,		//CompareFunc_Not_Equal
	D3DCMP_GREATEREQUAL,	//CompareFunc_Greater_Equal
	D3DCMP_ALWAYS,			//CompareFunc_Always
};

static D3DSTENCILOP gStencilOpTranslation[] =
{
	D3DSTENCILOP_KEEP,      //StencilOp_Keep
	D3DSTENCILOP_ZERO,      //StencilOp_Zero
	D3DSTENCILOP_REPLACE,   //StencilOp_Replace
	D3DSTENCILOP_INCRSAT,   //StencilOp_Increase_Saturate
	D3DSTENCILOP_DECRSAT,	//StencilOp_Decrease_Saturate
	D3DSTENCILOP_INVERT,    //StencilOp_Invert
	D3DSTENCILOP_INCR,      //StencilOp_Increase
	D3DSTENCILOP_DECR,      //StencilOp_Decrease	
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DDepthStencilState::D3DDepthStencilState() 
{    
}

//============================================================================

D3DDepthStencilState::~D3DDepthStencilState()
{
}

//============================================================================

void D3DDepthStencilState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context, 
	bool forceUpload 
	)
{	
	UNUSED_ALWAYS( context );

	const DepthStencilState* currentState = renderStateShadowing->GetCurrentDepthStencilState();
	const DepthStencilStateStruct* currentStateData = NULL;

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

	if ( forceUpload == true || mData.mDepthEnable != currentStateData->mDepthEnable )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_ZENABLE, mData.mDepthEnable );
	}

	if ( forceUpload == true || mData.mDepthWriteMask != currentStateData->mDepthWriteMask )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_ZWRITEENABLE, mData.mDepthWriteMask );
	}

	if ( forceUpload == true || mData.mDepthFunc != currentStateData->mDepthFunc )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_ZFUNC, gCompareFunctionTranslation[mData.mDepthFunc] );
	}

	if ( forceUpload == true || mData.mStencilEnable != currentStateData->mStencilEnable )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILENABLE, mData.mStencilEnable );
	}

	if ( forceUpload == true || mData.mFrontFace.mStencilFailOp != currentStateData->mFrontFace.mStencilFailOp )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILFAIL, gStencilOpTranslation[mData.mFrontFace.mStencilFailOp] );
	}

	if ( forceUpload == true || mData.mFrontFace.mStencilDepthFailOp != currentStateData->mFrontFace.mStencilDepthFailOp )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILZFAIL, gStencilOpTranslation[mData.mFrontFace.mStencilDepthFailOp] );
	}

	if ( forceUpload == true || mData.mFrontFace.mStencilPassOp != currentStateData->mFrontFace.mStencilPassOp )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILPASS, gStencilOpTranslation[mData.mFrontFace.mStencilPassOp] );
	}

	if ( forceUpload == true || mData.mFrontFace.mStencilFunc != currentStateData->mFrontFace.mStencilFunc )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILFUNC, gCompareFunctionTranslation[mData.mFrontFace.mStencilFunc] );
	}

	if ( forceUpload == true || mData.mStencilRef != currentStateData->mStencilRef )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILREF, mData.mStencilRef );
	}

	if ( forceUpload == true || mData.mStencilReadMask != currentStateData->mStencilReadMask )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILMASK, mData.mStencilReadMask );
	}

	if ( forceUpload == true || mData.mStencilWriteMask != currentStateData->mStencilWriteMask )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_STENCILWRITEMASK, mData.mStencilWriteMask );
	}

	renderStateShadowing->SetCurrentDepthStencilState( this );
}

#endif