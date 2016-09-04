#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_OGL_RENDERFACTORY_HPP
#define FIXED_OGL_RENDERFACTORY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rendererfactory.hpp"
#include "iphone/render/ogles1.1/fixedoglshaderprogram.hpp"
#include "iphone/render/ogles1.1/fixedogltexture.hpp"
#include "iphone/render/ogles1.1/fixedoglrendertarget.hpp"
#include "iphone/render/ogles1.1/fixedoglvertexbuffer.hpp"
#include "iphone/render/ogles1.1/fixedoglvertexdeclaration.hpp"
#include "iphone/render/ogles1.1/fixedogldepthstencilstate.hpp"
#include "iphone/render/ogles1.1/fixedoglblendstate.hpp"
#include "iphone/render/ogles1.1/fixedoglrasterizerstate.hpp"
#include "iphone/render/ogles1.1/fixedoglsamplerstate.hpp"
#include "iphone/render/ogles1.1/fixedoglvertexshader.hpp"
#include "iphone/render/ogles1.1/fixedoglpixelshader.hpp"
#include "iphone/render/ogles1.1/fixedoglconstantbuffer.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class FixedOGLRenderFactory : public RenderFactory
{
public:

    Texture* CreateTexture()
	{
		Texture* texture = NEW_PTR( "Fixed OGL Texture" ) FixedOGLTexture;		

		return texture;		
	}

    RenderTarget* CreateRenderTarget()
	{
		return NEW_PTR( "Fixed OGL Render Target" ) FixedOGLRenderTarget;		
	}

	VertexBuffer* CreateVertexBuffer()
	{
		return NEW_PTR( "Fixed OGL Vertex Buffer" ) FixedOGLVertexBuffer;		
	}

	VertexDeclaration* CreateVertexDeclaration()
	{
		return NEW_PTR( "Fixed OGL Vertex Declaration" ) FixedOGLVertexDeclaration;		
	}

	DepthStencilState* CreateDepthStencilState()
	{
		return NEW_PTR( "Fixed OGL Depth Stencil State" ) FixedOGLDepthStencilState;		
	}

	BlendState* CreateBlendState()
	{
		return NEW_PTR( "Fixed OGL Blend State" ) FixedOGLBlendState;		
	}

	RasterizerState* CreateRasterizerState()
	{
		return NEW_PTR( "Fixed OGL Rasterizer State" ) FixedOGLRasterizerState;		
	}

	SamplerState* CreateSamplerState()
	{
		return NEW_PTR( "Fixed OGL Sampler State" ) FixedOGLSamplerState;		
	}

	VertexShader* CreateVertexShader()
	{
		return NEW_PTR( "Fixed OGL Vertex Shader" ) FixedOGLVertexShader;		
	}

	PixelShader* CreatePixelShader()
	{
		return NEW_PTR( "Fixed OGL Pixel Shader" ) FixedOGLPixelShader;		
	}

	ShaderProgram* CreateShaderProgram( int id )
	{
		return NEW_PTR( "Fixed GLSL Program" ) FixedOGLShaderProgram( id );		
	}

	ConstantBuffer* CreateConstantBuffer( ConstantBuffer::ConstantBufferType type )
	{
		return NEW_PTR( "Fixed Constant Buffer" ) FixedOGLConstantBuffer( type );
	}

private:

};

#endif

#endif