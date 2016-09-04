#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11samplerstate.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static D3D11_TEXTURE_ADDRESS_MODE gTextureAddressTranslation[] =
{	
	D3D11_TEXTURE_ADDRESS_WRAP,			// TextureAddressMode_Wrap,
	D3D11_TEXTURE_ADDRESS_MIRROR,		// TextureAddressMode_Mirror,
	D3D11_TEXTURE_ADDRESS_CLAMP,		// TextureAddressMode_Clamp,
	D3D11_TEXTURE_ADDRESS_BORDER,		// TextureAddressMode_Border,
	D3D11_TEXTURE_ADDRESS_MIRROR_ONCE,	// TextureAddressMode_Mirror_once
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11SamplerState::D3D11SamplerState() 
{    
	mSamplerState = NULL;
}

//============================================================================

D3D11SamplerState::~D3D11SamplerState()
{
	RELEASE_D3D_OBJECT( mSamplerState );
}

//============================================================================

void D3D11SamplerState::Dispatch( 
	RenderStateShadowing* renderStateShadowing, 
	void* context,
	int samplerIndex, 
	bool forceUpload 
	)
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	const SamplerState* currentState = renderStateShadowing->GetCurrentSamplerState( samplerIndex );

	if ( currentState == this )
	{
		return;
	}

	if ( ( currentState != this || forceUpload ) && mSamplerState != NULL )
	{
		deviceContext->PSSetSamplers( samplerIndex, 1, &mSamplerState );
	}

	renderStateShadowing->SetCurrentSamplerState( samplerIndex, this );
}


//============================================================================

void D3D11SamplerState::Create()
{
	if ( mSamplerState == NULL )
	{
		memset( &mSamplerDesc, 0, sizeof( D3D11_SAMPLER_DESC ) );
		mSamplerDesc.Filter = GetFilterMode( mData.mMinFilter, mData.mMagFilter, mData.mMipFilter );
		mSamplerDesc.AddressU = gTextureAddressTranslation[mData.mAddressU];
		mSamplerDesc.AddressV = gTextureAddressTranslation[mData.mAddressV];
		mSamplerDesc.AddressW = gTextureAddressTranslation[mData.mAddressW];
		mSamplerDesc.MipLODBias = mData.mMipLODBias;
		mSamplerDesc.MaxAnisotropy = mData.mMaxAnisotropy;
		mSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		for ( int i = 0; i < 4; ++i )
		{
			mSamplerDesc.BorderColor[i] = mData.mBorderColor[i];
		}
		mSamplerDesc.MinLOD = 0;
		mSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		D3D11Renderer::GetDevice()->CreateSamplerState( &mSamplerDesc, &mSamplerState );
	}	
}

//============================================================================

D3D11_FILTER D3D11SamplerState::GetFilterMode( FilterMode minFilter, FilterMode magFilter, FilterMode mipFilter )
{
	if ( minFilter == Filter_Point && magFilter == Filter_Point && ( mipFilter == Filter_Point || mipFilter == Filter_None ) )
	{
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
	else if ( minFilter == Filter_Point && magFilter == Filter_Point && mipFilter == Filter_Linear )
	{
		return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	}
	else if ( minFilter == Filter_Point && magFilter == Filter_Linear && ( mipFilter == Filter_Point || mipFilter == Filter_None ) )
	{
		return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	}
	else if ( minFilter == Filter_Point && magFilter == Filter_Linear && mipFilter == Filter_Linear )
	{
		return D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	}
	else if ( minFilter == Filter_Linear && magFilter == Filter_Point && ( mipFilter == Filter_Point || mipFilter == Filter_None ) )
	{
		return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	}
	else if ( minFilter == Filter_Linear && magFilter == Filter_Point && mipFilter == Filter_Linear )
	{
		return D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	}
	else if ( minFilter == Filter_Linear && magFilter == Filter_Linear && ( mipFilter == Filter_Point || mipFilter == Filter_None ) )
	{
		return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}
	else if ( minFilter == Filter_Linear && magFilter == Filter_Linear && mipFilter == Filter_Linear )
	{
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else if ( minFilter == Filter_Anisotropic && magFilter == Filter_Anisotropic && mipFilter == Filter_Anisotropic )
	{
		return D3D11_FILTER_ANISOTROPIC;
	}
	else
	{
		Assert( false, "Unknown Filter Combination" );
	}

	return D3D11_FILTER_MIN_MAG_MIP_POINT;
}

#endif