#if defined( RENDER_PLAT_OPENGL )

#ifndef OGL_RENDERFACTORY_HPP
#define OGL_RENDERFACTORY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogl/oglblendstate.hpp"
#include "common/engine/render/ogl/oglconstantbuffer.hpp"
#include "common/engine/render/ogl/ogldepthstencilstate.hpp"
#include "common/engine/render/ogl/oglpixelshader.hpp"
#include "common/engine/render/ogl/oglrasterizerstate.hpp"
#include "common/engine/render/ogl/oglrendertarget.hpp"
#include "common/engine/render/ogl/oglsamplerstate.hpp"
#include "common/engine/render/ogl/oglshaderprogram.hpp"
#include "common/engine/render/ogl/ogltexture.hpp"
#include "common/engine/render/ogl/oglvertexbuffer.hpp"
#include "common/engine/render/ogl/oglvertexdeclaration.hpp"
#include "common/engine/render/ogl/oglvertexshader.hpp"
#include "common/engine/render/rendererfactory.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLRenderFactory : public RenderFactory
{
public:

    Texture* CreateTexture()
	{
		Texture* texture = NEW_PTR( "OGL Texture" ) OGLTexture;
		
		return texture;
	}

    RenderTarget* CreateRenderTarget()
	{
		return NEW_PTR( "OGL Render Target" ) OGLRenderTarget;
	}

	VertexBuffer* CreateVertexBuffer()
	{
		return NEW_PTR( "OGL Vertex Buffer" ) OGLVertexBuffer;
	}

	VertexDeclaration* CreateVertexDeclaration()
	{
		return NEW_PTR( "OGL Vertex Declaration" ) OGLVertexDeclaration;
	}

	DepthStencilState* CreateDepthStencilState()
	{
		return NEW_PTR( "OGL Depth Stencil State" ) OGLDepthStencilState;
	}

	BlendState* CreateBlendState()
	{
		return NEW_PTR( "OGL Blend State" ) OGLBlendState;
	}

	RasterizerState* CreateRasterizerState()
	{
		return NEW_PTR( "OGL Rasterizer State" ) OGLRasterizerState;
	}

	SamplerState* CreateSamplerState()
	{
		return NEW_PTR( "OGL Sampler State" ) OGLSamplerState;
	}

	VertexShader* CreateVertexShader()
	{
		return NEW_PTR( "OGL Vertex Shader" ) OGLVertexShader;
	}

	PixelShader* CreatePixelShader()
	{
		return NEW_PTR( "OGL Pixel Shader" ) OGLPixelShader;
	}

	ShaderProgram* CreateShaderProgram( int id )
	{
		return NEW_PTR( "GLSL Program" ) OGLShaderProgram( id );
	}

	ConstantBuffer* CreateConstantBuffer( ConstantBuffer::ConstantBufferType type )
	{
		return NEW_PTR( "GLSL Constant Buffer" ) OGLConstantBuffer( type );
	}

private:
    
};

#endif

#endif