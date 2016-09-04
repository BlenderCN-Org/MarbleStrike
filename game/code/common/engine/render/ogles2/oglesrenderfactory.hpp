#if defined( RENDER_PLAT_OGLES2 )

#ifndef OGLES_RENDERFACTORY_HPP
#define OGLES_RENDERFACTORY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogles2/oglesblendstate.hpp"
#include "common/engine/render/ogles2/oglesconstantbuffer.hpp"
#include "common/engine/render/ogles2/oglesdepthstencilstate.hpp"
#include "common/engine/render/ogles2/oglespixelshader.hpp"
#include "common/engine/render/ogles2/oglesrasterizerstate.hpp"
#include "common/engine/render/ogles2/oglesrendertarget.hpp"
#include "common/engine/render/ogles2/oglessamplerstate.hpp"
#include "common/engine/render/ogles2/oglesshaderprogram.hpp"
#include "common/engine/render/ogles2/oglestexture.hpp"
#include "common/engine/render/ogles2/oglesvertexbuffer.hpp"
#include "common/engine/render/ogles2/oglesvertexdeclaration.hpp"
#include "common/engine/render/ogles2/oglesvertexshader.hpp"
#include "common/engine/render/rendererfactory.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLESRenderFactory : public RenderFactory
{
public:

    Texture* CreateTexture()
	{
		return NEW_PTR( "OGLES Texture" ) OGLESTexture;
	}

    RenderTarget* CreateRenderTarget()
	{
		return NEW_PTR( "OGLES Render Target" ) OGLESRenderTarget;
	}

	VertexBuffer* CreateVertexBuffer()
	{
		return NEW_PTR( "OGLES Vertex Buffer" ) OGLESVertexBuffer;
	}

	VertexDeclaration* CreateVertexDeclaration()
	{
		return NEW_PTR( "OGLES Vertex Declaration" ) OGLESVertexDeclaration;
	}

	DepthStencilState* CreateDepthStencilState()
	{
		return NEW_PTR( "OGLES Depth Stencil State" ) OGLESDepthStencilState;
	}

	BlendState* CreateBlendState()
	{
		return NEW_PTR( "OGLES Blend State" ) OGLESBlendState;
	}

	RasterizerState* CreateRasterizerState()
	{
		return NEW_PTR( "OGLES Rasterizer State" ) OGLESRasterizerState;
	}

	SamplerState* CreateSamplerState()
	{
		return NEW_PTR( "OGLES Sampler State" ) OGLESSamplerState;
	}

	VertexShader* CreateVertexShader()
	{
		return NEW_PTR( "OGLES Vertex Shader" ) OGLESVertexShader;
	}

	PixelShader* CreatePixelShader()
	{
		return NEW_PTR( "OGLES Pixel Shader" ) OGLESPixelShader;
	}

	ShaderProgram* CreateShaderProgram( int id )
	{
		return NEW_PTR( "OGLES Program" ) OGLESShaderProgram( id );
	}

	ConstantBuffer* CreateConstantBuffer( ConstantBuffer::ConstantBufferType type )
	{
		return NEW_PTR( "OGLES Constant Buffer" ) OGLESConstantBuffer( type );
	}

private:
    
};

#endif

#endif