#ifndef RENDERFACTORY_HPP
#define RENDERFACTORY_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/constantbuffer.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/render/renderqueue.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

class ShaderProgram;
class Texture;
class RenderTarget;
class VertexBuffer;
class VertexDeclaration;
class DepthStencilState;
class BlendState;
class RasterizerState;
class SamplerState;
class VertexShader;
class PixelShader;
class ShaderProgram;
class ConstantBuffer;
class RenderQueue;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RenderFactory
{
public:

	virtual ~RenderFactory() {}

    virtual Texture* CreateTexture() = 0;
    virtual RenderTarget* CreateRenderTarget() = 0;
	virtual VertexBuffer* CreateVertexBuffer() = 0;
	virtual VertexDeclaration* CreateVertexDeclaration() = 0;
	virtual DepthStencilState* CreateDepthStencilState() = 0;
	virtual BlendState* CreateBlendState() = 0;
	virtual RasterizerState* CreateRasterizerState() = 0;
	virtual SamplerState* CreateSamplerState() = 0;
	virtual VertexShader* CreateVertexShader() = 0;
	virtual PixelShader* CreatePixelShader() = 0;
	virtual ShaderProgram* CreateShaderProgram( int id ) = 0;
	virtual ConstantBuffer* CreateConstantBuffer( ConstantBuffer::ConstantBufferType type ) = 0;
	virtual RenderQueue* CreateRenderQueue()
	{
		return NEW_PTR( "Render Queue" ) RenderQueue;
	}
    
protected:

private:

};

#endif
