#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d/d3drasterizerstate.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include <d3d9.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3DCULL gCullModeTranslation[] =
{
	D3DCULL_NONE,	// CullMode_None
	D3DCULL_CCW,	// CullMode_Front
	D3DCULL_CW		// CullMode_Back	
};

static D3DFILLMODE gFillModeTranslation[] =
{
	D3DFILL_WIREFRAME,	// FillMode_Wireframe
	D3DFILL_SOLID		// FillMode_Solid
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DRasterizerState::D3DRasterizerState() 
{    
}

//============================================================================

D3DRasterizerState::~D3DRasterizerState()
{
}

//============================================================================

void D3DRasterizerState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context, 
	bool forceUpload 
	) 
{
	UNUSED_ALWAYS( context );

	const RasterizerState* currentState = renderStateShadowing->GetCurrentRasterizerState();
	const RasterizerStateStruct* currentStateData = NULL;

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

	if ( forceUpload == true || mData.mCullMode != currentStateData->mCullMode )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_CULLMODE , gCullModeTranslation[mData.mCullMode] );	
	}

	if ( forceUpload == true || mData.mDepthBias != currentStateData->mDepthBias )
	{
		float fBias = static_cast<float>( mData.mDepthBias ) / 16777215.f;
		DWORD dwBias = *(DWORD*)&fBias;
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_DEPTHBIAS, dwBias );	
	}

	if ( forceUpload == true || mData.mSlopeScaledDepthBias != currentStateData->mSlopeScaledDepthBias )
	{
		float fBias = mData.mSlopeScaledDepthBias;
		DWORD dwBias = *(DWORD*)&fBias;
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_SLOPESCALEDEPTHBIAS, dwBias );	
	}

	if ( forceUpload == true || mData.mFillMode != currentStateData->mFillMode )
	{
		D3DRenderer::GetDevice()->SetRenderState( D3DRS_FILLMODE , gFillModeTranslation[mData.mFillMode] );	
	}	

	renderStateShadowing->SetCurrentRasterizerState( this );
}

#endif