#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_RENDERERFACTORY_HPP
#define D3D11_RENDERERFACTORY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d11/d3d11blendstate.hpp"
#include "common/engine/render/d3d11/d3d11constantbuffer.hpp"
#include "common/engine/render/d3d11/d3d11depthstencilstate.hpp"
#include "common/engine/render/d3d11/d3d11pixelshader.hpp"
#include "common/engine/render/d3d11/d3d11rasterizerstate.hpp"
#include "common/engine/render/d3d11/d3d11renderqueue.hpp"
#include "common/engine/render/d3d11/d3d11rendertarget.hpp"
#include "common/engine/render/d3d11/d3d11samplerstate.hpp"
#include "common/engine/render/d3d11/d3d11shaderprogram.hpp"
#include "common/engine/render/d3d11/d3d11texture.hpp"
#include "common/engine/render/d3d11/d3d11vertexbuffer.hpp"
#include "common/engine/render/d3d11/d3d11vertexdeclaration.hpp"
#include "common/engine/render/d3d11/d3d11vertexshader.hpp"
#include "common/engine/render/rendererfactory.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11RenderFactory : public RenderFactory
{
public:

	Texture* CreateTexture()
	{
		return NEW_PTR( "D3D11 Texture" ) D3D11Texture;
	}

	RenderTarget* CreateRenderTarget()
	{
		return NEW_PTR( "D3D11 Render Target" ) D3D11RenderTarget;
	}

	VertexBuffer* CreateVertexBuffer()
	{
		return NEW_PTR( "D3D11 Vertex Buffer" ) D3D11VertexBuffer;
	}

	VertexDeclaration* CreateVertexDeclaration()
	{
		return NEW_PTR( "D3D11 Vertex Declaration" ) D3D11VertexDeclaration;
	}

	DepthStencilState* CreateDepthStencilState()
	{
		return NEW_PTR( "D3D11 Depth Stencil State" ) D3D11DepthStencilState;
	}

	BlendState* CreateBlendState()
	{
		return NEW_PTR( "D3D11 Blend State" ) D3D11BlendState;
	}

	RasterizerState* CreateRasterizerState()
	{
		return NEW_PTR( "D3D11 Rasterizer State" ) D3D11RasterizerState;
	}

	SamplerState* CreateSamplerState()
	{
		return NEW_PTR( "D3D11 Sampler State" ) D3D11SamplerState;
	}

	VertexShader* CreateVertexShader()
	{
		return NEW_PTR( "D3D11 Vertex Shader" ) D3D11VertexShader;
	}

	PixelShader* CreatePixelShader()
	{
		return NEW_PTR( "D3D11 Pixel Shader" ) D3D11PixelShader;
	}

	ShaderProgram* CreateShaderProgram( int id )
	{
		return NEW_PTR( "D3D11 Shader Program" ) D3D11ShaderProgram( id );
	}

	ConstantBuffer* CreateConstantBuffer( ConstantBuffer::ConstantBufferType type )
	{
		return NEW_PTR( "D3D11 Constant Buffer" ) D3D11ConstantBuffer( type );
	}

	RenderQueue* CreateRenderQueue()
	{
		return NEW_PTR( "D3D11 Render Queue" ) D3D11RenderQueue;
	}

private:

};

#endif

#endif