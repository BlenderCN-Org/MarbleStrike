#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11pixelshader.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11shaderprogram.hpp"
#include "common/engine/render/d3d11/d3d11texture.hpp"
#include "common/engine/render/d3d11/d3d11vertexshader.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/samplerstate.hpp"
#include "common/engine/render/shadermanager.hpp"
#include "common/engine/render/texture.hpp"
#include "common/engine/render/textureparameter.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/utilities.hpp"
#include <stdio.h>

D3D11VertexShader* D3D11ShaderProgram::mCurrentVertexShader = NULL;
D3D11PixelShader* D3D11ShaderProgram::mCurrentPixelShader = NULL;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11ShaderProgram::D3D11ShaderProgram( int id )
	:	ShaderProgram( id )
{   
	mVertexBufferSize = 0;
	mPixelBufferSize = 0;
}

//===========================================================================

D3D11ShaderProgram::~D3D11ShaderProgram()
{   
	for ( int i = 0; i < mNumThreads; ++i )
	{
		DELETE_PTR_ARRAY( mVertexData[i] );
	}
	DELETE_PTR_ARRAY( mVertexData );

	for ( int i = 0; i < mNumThreads; ++i )
	{
		DELETE_PTR_ARRAY( mPixelData[i] );
	}
	DELETE_PTR_ARRAY( mPixelData );
}

//===========================================================================

void D3D11ShaderProgram::Create()
{	
	D3D11Renderer* renderer = reinterpret_cast<D3D11Renderer*>( Renderer::Get() );
	mNumThreads = renderer->GetNumThreads();

	VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	
    mVertexConstantList = reinterpret_cast<D3D11VertexShader*>( vertexShader )->GetConstantList();
	mVertexBufferSize = reinterpret_cast<D3D11VertexShader*>( vertexShader )->GetBufferSize();

	mVertexData = NEW_PTR( "Vertex Data" ) unsigned char*[mNumThreads];
	for ( int i = 0; i < mNumThreads; ++i )
	{
		mVertexData[i] = NEW_PTR( "Vertex Data" ) unsigned char[mVertexBufferSize];
	}

	mD3D11VertexBuffer = NEW_PTR( "VertexBuffer" ) ID3D11Buffer*[mNumThreads];
	for ( int i = 0; i < mNumThreads; ++i )
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = mVertexBufferSize;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		HRESULT hr = D3D11Renderer::GetDevice()->CreateBuffer( &bufferDesc, NULL, &mD3D11VertexBuffer[i] );
		if ( hr != S_OK )
		{
			Assert( false, "Error creating buffer.")
		}
	}

	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	mPixelConstantList = reinterpret_cast<D3D11PixelShader*>( pixelShader )->GetConstantList();
    mPixelBufferSize = reinterpret_cast<D3D11PixelShader*>( pixelShader )->GetBufferSize();
    mTextureList = reinterpret_cast<D3D11PixelShader*>( pixelShader )->GetTextureList();
    
	mPixelData = NEW_PTR( "Pixel Data" ) unsigned char*[mNumThreads];
	for ( int i = 0; i < mNumThreads; ++i )
	{
		mPixelData[i] = NEW_PTR( "Vertex Data" ) unsigned char[mPixelBufferSize];
	}

	mD3D11PixelBuffer = NEW_PTR( "PixelBuffer" ) ID3D11Buffer*[mNumThreads];
	for ( int i = 0; i < mNumThreads; ++i )
	{
		if ( mPixelBufferSize > 0 )
		{
			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.ByteWidth = mPixelBufferSize;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			HRESULT hr = D3D11Renderer::GetDevice()->CreateBuffer( &bufferDesc, NULL, &mD3D11PixelBuffer[i] );
			if ( hr != S_OK )
			{
				Assert( false, "Error creating buffer.")
			}
		}
		else
		{
			mD3D11PixelBuffer[i] = NULL;
		}
	}
}

//===========================================================================

void D3D11ShaderProgram::Destroy()
{    
	for ( int i = 0; i < mNumThreads; ++i )
	{
		if ( mD3D11VertexBuffer[i] )
		{
			mD3D11VertexBuffer[i]->Release();
			mD3D11VertexBuffer[i] = NULL;
		}
	}
	DELETE_PTR_ARRAY( mD3D11VertexBuffer );

	for ( int i = 0; i < mNumThreads; ++i )
	{
		if ( mD3D11PixelBuffer[i] )
		{
			mD3D11PixelBuffer[i]->Release();
			mD3D11PixelBuffer[i] = NULL;
		}
	}
	DELETE_PTR_ARRAY( mD3D11PixelBuffer );
}

//===========================================================================

void D3D11ShaderProgram::Reload()
{
	VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );
	vertexShader->Reload();
	pixelShader->Reload();
}

//===========================================================================

void D3D11ShaderProgram::DisableTextureSamplers( void* context )
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	ID3D11ShaderResourceView* nullViews[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	deviceContext->PSSetShaderResources( 0, 8, nullViews );
}

//===========================================================================

void D3D11ShaderProgram::SetProgram(
	EffectPass* effectPass,
	RenderStateShadowing* renderStateShadowing,
	const std::map<int, const void*> &constantMap, 
	int threadID,
	void* context 
	)
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	VertexShader* vertexShader = GameApp::Get()->GetShaderManager()->GetVertexShaderByHandle( mVertexShaderHandle );
	PixelShader* pixelShader = GameApp::Get()->GetShaderManager()->GetPixelShaderByHandle( mPixelShaderHandle );

	D3D11VertexShader* d3d11VertexShader = reinterpret_cast<D3D11VertexShader*>( vertexShader );
	D3D11PixelShader* d3d11PixelShader = reinterpret_cast<D3D11PixelShader*>( pixelShader );

    if ( mCurrentVertexShader != vertexShader )
    {
        mCurrentVertexShader = d3d11VertexShader;
        d3d11VertexShader->Dispatch( deviceContext );
    }

    if ( mCurrentPixelShader != pixelShader )
    {
        mCurrentPixelShader = d3d11PixelShader;
	    d3d11PixelShader->Dispatch( deviceContext );	
    }

	ConstantList::iterator vertexConstantListIter = mVertexConstantList->begin();
	for ( vertexConstantListIter; vertexConstantListIter != mVertexConstantList->end(); ++vertexConstantListIter )
	{
		ConstantData data = *vertexConstantListIter;
		std::map<int, const void*>::const_iterator iter = constantMap.find( data.mNameHash );
		Assert( iter != constantMap.end(), "Parameter not set." );

		memcpy( mVertexData[threadID] + data.mOffset, iter->second, data.mSize );
	}

	D3D11_MAPPED_SUBRESOURCE vertexMappedResource;
	deviceContext->Map( mD3D11VertexBuffer[threadID], 0, D3D11_MAP_WRITE_DISCARD, 0, &vertexMappedResource );	
	memcpy( vertexMappedResource.pData, mVertexData[threadID], mVertexBufferSize );
	deviceContext->Unmap( mD3D11VertexBuffer[threadID], 0 );
	deviceContext->VSSetConstantBuffers( 0, 1, &mD3D11VertexBuffer[threadID] );

	ConstantList::iterator pixelConstantListIter = mPixelConstantList->begin();
	for ( pixelConstantListIter; pixelConstantListIter != mPixelConstantList->end(); ++pixelConstantListIter )
	{
		ConstantData data = *pixelConstantListIter;
		std::map<int, const void*>::const_iterator iter = constantMap.find( data.mNameHash );
		Assert( iter != constantMap.end(), "Parameter not set." );

		memcpy( mPixelData[threadID] + data.mOffset, iter->second, data.mSize );
	}
	
	if ( mPixelBufferSize > 0 )
	{
		D3D11_MAPPED_SUBRESOURCE pixelMappedResource;
		deviceContext->Map( mD3D11PixelBuffer[threadID], 0, D3D11_MAP_WRITE_DISCARD, 0, &pixelMappedResource );	
		memcpy( pixelMappedResource.pData, mPixelData[threadID], mPixelBufferSize );
		deviceContext->Unmap( mD3D11PixelBuffer[threadID], 0 );
		deviceContext->PSSetConstantBuffers( 0, 1, &mD3D11PixelBuffer[threadID] );
	}
	
	TextureList::iterator textureListIter = mTextureList->begin();
	for ( textureListIter; textureListIter != mTextureList->end(); ++textureListIter )
	{
		TextureData data = *textureListIter;
		std::map<int, const void*>::const_iterator iter = constantMap.find( data.mNameHash );
		Assert( iter != constantMap.end(), "Parameter not set." );

		const Texture* texture = reinterpret_cast<const Texture*>( iter->second );
		Assert( texture != NULL, "" );
		ResourceHandle<SamplerState> state = effectPass->GetSamplerState( data.mNameHash );
		SetParamTexture( texture, data.mSamplerIndex, renderStateShadowing, state, context );		
	}
}

//===========================================================================

void D3D11ShaderProgram::SetParamTexture( 
	const Texture* texture, 	
	int samplerIndex,
	RenderStateShadowing* renderStateShadowing,
	ResourceHandle<SamplerState> samplerStateHandle,
	void* context	
	)
{
	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	const D3D11Texture* d3d11Texture = reinterpret_cast<const D3D11Texture*>( texture );
	ID3D11ShaderResourceView* textureResourceView = d3d11Texture->GetHandle();

	deviceContext->PSSetShaderResources( samplerIndex, 1, &textureResourceView );

	SamplerState* samplerState = GameApp::Get()->GetRenderStateManager()->GetSamplerStateByHandle( samplerStateHandle );
	samplerState->Dispatch( renderStateShadowing, context, samplerIndex );
}

#endif