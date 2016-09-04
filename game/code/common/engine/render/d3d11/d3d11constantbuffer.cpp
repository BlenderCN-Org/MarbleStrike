#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d11/d3d11constantbuffer.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11ConstantBuffer::D3D11ConstantBuffer( ConstantBuffer::ConstantBufferType type )
	:	ConstantBuffer( type )
{
	mD3D11Buffer = NULL;
}

//============================================================================

D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
	if ( mD3D11Buffer )
	{
		mD3D11Buffer->Release();
		mD3D11Buffer = NULL;
	}
}

//============================================================================

void D3D11ConstantBuffer::SetConstantBufferData( void* data, int size )
{
	ConstantBuffer::SetConstantBufferData( data, size );

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = D3D11Renderer::GetDevice()->CreateBuffer( &bufferDesc, NULL, &mD3D11Buffer );
	if ( hr != S_OK )
	{
		Assert( false, "Error creating buffer.")
	}
}

//============================================================================

void D3D11ConstantBuffer::SetVectorParameter( const char* name, Math::Vector4 *data, int index )
{
	UNUSED_ALWAYS( name );
	UNUSED_ALWAYS( data );
	UNUSED_ALWAYS( index );
}

//============================================================================

void D3D11ConstantBuffer::SetMatrixParameter( const char* name, Math::Matrix44* data, int index )
{
	UNUSED_ALWAYS( name );
	UNUSED_ALWAYS( data );
	UNUSED_ALWAYS( index );
}

//===========================================================================

void D3D11ConstantBuffer::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	EffectPass* effectPass,
	void* context 
	)
{
	UNUSED_ALWAYS( effectPass );

	ID3D11DeviceContext* deviceContext = D3D11Renderer::GetImmediateDeviceContext();
	if ( context )
	{
		deviceContext = reinterpret_cast<ID3D11DeviceContext*>( context );
	}

	ConstantBuffer::DispatchInternal( renderStateShadowing, effectPass, context );

	if ( mD3D11Buffer )
	{
		//deviceContext->UpdateSubresource( mD3D11Buffer, 0, NULL, mConstantBufferData, 0, 0 );
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		deviceContext->Map( mD3D11Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );	
		memcpy( mappedResource.pData, mConstantBufferData, mConstantBufferDataSize );
		deviceContext->Unmap( mD3D11Buffer, 0 );

		if ( mConstantBufferType == CONSTANT_BUFFER_VERTEX )
		{
			deviceContext->VSSetConstantBuffers( 0, 1, &mD3D11Buffer );
		}
		else if ( mConstantBufferType == CONSTANT_BUFFER_PIXEL )
		{
			deviceContext->PSSetConstantBuffers( 0, 1, &mD3D11Buffer );
		}
	}
}

#endif