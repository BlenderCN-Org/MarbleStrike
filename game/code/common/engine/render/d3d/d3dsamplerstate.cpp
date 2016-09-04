#if defined( RENDER_PLAT_D3D9 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d/d3drenderer.hpp"
#include "common/engine/render/d3d/d3dsamplerstate.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include <d3d9.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3DTEXTUREFILTERTYPE gFilterModeTranslation[] =
{	
	D3DTEXF_NONE,			// Filter_None,
	D3DTEXF_POINT,			// Filter_Point,
	D3DTEXF_LINEAR,			// Filter_Linear,
	D3DTEXF_ANISOTROPIC,	// Filter_Anisotropic
};

static D3DTEXTUREADDRESS gTextureAddressTranslation[] =
{	
	D3DTADDRESS_WRAP,		// TextureAddressMode_Wrap,
	D3DTADDRESS_MIRROR,		// TextureAddressMode_Mirror,
	D3DTADDRESS_CLAMP,		// TextureAddressMode_Clamp,
	D3DTADDRESS_BORDER,		// TextureAddressMode_Border,
	D3DTADDRESS_MIRRORONCE,	// TextureAddressMode_Mirror_once
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3DSamplerState::D3DSamplerState() 
{    
}

//============================================================================

D3DSamplerState::~D3DSamplerState()
{
}

//============================================================================

void D3DSamplerState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context, 
	int samplerIndex, 
	bool forceUpload 
	)
{
	UNUSED_ALWAYS( context );

	const SamplerState* currentState = renderStateShadowing->GetCurrentSamplerState( samplerIndex );
	const SamplerStateStruct* currentStateData = NULL;

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

	if ( forceUpload == true || mData.mAddressU != currentStateData->mAddressU )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_ADDRESSU, gTextureAddressTranslation[mData.mAddressU] );
	}

	if ( forceUpload == true || mData.mAddressV != currentStateData->mAddressV )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_ADDRESSV, gTextureAddressTranslation[mData.mAddressV] );
	}

	if ( forceUpload == true || mData.mAddressW != currentStateData->mAddressW )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_ADDRESSW, gTextureAddressTranslation[mData.mAddressW] );
	}

	if ( 
		forceUpload == true ||
		mData.mBorderColor[0] != currentStateData->mBorderColor[0] ||
		mData.mBorderColor[1] != currentStateData->mBorderColor[1] ||
		mData.mBorderColor[2] != currentStateData->mBorderColor[2] ||
		mData.mBorderColor[3] != currentStateData->mBorderColor[3]
		)
	{
		D3DCOLOR color = D3DCOLOR_RGBA( 
			static_cast<int>( mData.mBorderColor[0] * 255.0f ), 
			static_cast<int>( mData.mBorderColor[1] * 255.0f ), 
			static_cast<int>( mData.mBorderColor[2] * 255.0f ), 
			static_cast<int>( mData.mBorderColor[3] * 255.0f ) );
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_BORDERCOLOR, color );
	}

	if ( forceUpload == true || mData.mMagFilter != currentStateData->mMagFilter )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_MAGFILTER, gFilterModeTranslation[mData.mMagFilter] );
	}

	if ( forceUpload == true || mData.mMinFilter != currentStateData->mMinFilter )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_MINFILTER, gFilterModeTranslation[mData.mMinFilter] );
	}

	if ( forceUpload == true || mData.mMipFilter != currentStateData->mMipFilter )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_MIPFILTER, gFilterModeTranslation[mData.mMipFilter] );
	}
	
	if ( forceUpload == true )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_MIPMAPLODBIAS, 0 );
	}

	if ( forceUpload == true )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_MAXMIPLEVEL, 0 );
	}

	if ( forceUpload == true || mData.mMaxAnisotropy != currentStateData->mMaxAnisotropy )
	{
		D3DRenderer::GetDevice()->SetSamplerState( samplerIndex, D3DSAMP_MAXANISOTROPY, mData.mMaxAnisotropy );
	}

	renderStateShadowing->SetCurrentSamplerState( samplerIndex, this );
}

#endif