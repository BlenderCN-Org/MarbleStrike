#if defined( RENDER_PLAT_D3D9 )

#ifndef D3D_RENDERERFACTORY_HPP
#define D3D_RENDERERFACTORY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/d3d/d3dblendstate.hpp"
#include "common/engine/render/d3d/d3dconstantbuffer.hpp"
#include "common/engine/render/d3d/d3ddepthstencilstate.hpp"
#include "common/engine/render/d3d/d3dpixelshader.hpp"
#include "common/engine/render/d3d/d3drasterizerstate.hpp"
#include "common/engine/render/d3d/d3drendertarget.hpp"
#include "common/engine/render/d3d/d3dsamplerstate.hpp"
#include "common/engine/render/d3d/d3dshaderprogram.hpp"
#include "common/engine/render/d3d/d3dtexture.hpp"
#include "common/engine/render/d3d/d3dvertexbuffer.hpp"
#include "common/engine/render/d3d/d3dvertexdeclaration.hpp"
#include "common/engine/render/d3d/d3dvertexshader.hpp"
#include "common/engine/render/rendererfactory.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3DRenderFactory : public RenderFactory
{
public:

	Texture* CreateTexture()
	{
		Texture* texture = NEW_PTR( "D3D Texture" ) D3DTexture;		

		return texture;
	}

	RenderTarget* CreateRenderTarget()
	{
		return NEW_PTR( "D3D Render Target" ) D3DRenderTarget;
	}

	VertexBuffer* CreateVertexBuffer()
	{
		return NEW_PTR( "D3D Vertex Buffer" ) D3DVertexBuffer;
	}

	VertexDeclaration* CreateVertexDeclaration()
	{
		return NEW_PTR( "D3D Vertex Declaration" ) D3DVertexDeclaration;
	}

	DepthStencilState* CreateDepthStencilState()
	{
		return NEW_PTR( "D3D Depth Stencil State" ) D3DDepthStencilState;
	}

	BlendState* CreateBlendState()
	{
		return NEW_PTR( "D3D Blend State" ) D3DBlendState;
	}

	RasterizerState* CreateRasterizerState()
	{
		return NEW_PTR( "D3D Rasterizer State" ) D3DRasterizerState;
	}

	SamplerState* CreateSamplerState()
	{
		return NEW_PTR( "D3D Sampler State" ) D3DSamplerState;
	}

	VertexShader* CreateVertexShader()
	{
		return NEW_PTR( "D3D Vertex Shader" ) D3DVertexShader;
	}

	PixelShader* CreatePixelShader()
	{
		return NEW_PTR( "D3D Pixel Shader" ) D3DPixelShader;
	}

	ShaderProgram* CreateShaderProgram( int id )
	{
		return NEW_PTR( "D3D Shader Program" ) D3DShaderProgram( id );
	}

	ConstantBuffer* CreateConstantBuffer( ConstantBuffer::ConstantBufferType type )
	{
		return NEW_PTR( "D3D Constant Buffer" ) D3DConstantBuffer( type );
	}

private:

};

#endif

#endif