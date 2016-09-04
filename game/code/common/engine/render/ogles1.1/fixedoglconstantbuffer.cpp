#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/effectpass.hpp"
#include "iphone/render/ogles1.1/fixedoglrenderer.hpp"
#include "iphone/render/ogles1.1/fixedoglshaderprogram.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "iphone/render/ogles1.1/fixedoglshaderprogram.hpp"
#include "iphone/render/ogles1.1/fixedoglconstantbuffer.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLConstantBuffer::FixedOGLConstantBuffer( ConstantBuffer::ConstantBufferType type )
	: ConstantBuffer( type )
{
}

//============================================================================

FixedOGLConstantBuffer::~FixedOGLConstantBuffer()
{
	
}
//===========================================================================

void FixedOGLConstantBuffer::Dispatch( 
	RenderStateShadowing* /*renderStateShadowing*/,
	EffectPass* /*effectPass*/, 
	void* /*context*/ 
	)
{
	//ConstantBuffer::DispatchInternal( renderStateShadowing, effectPass, context );

	//FixedOGLShaderProgram* shaderProgramfixed = reinterpret_cast<FixedOGLShaderProgram*>( effectPass->GetShaderProgram() );
	//shaderProgramfixed->SetConstants( renderStateShadowing, context );
}

#endif