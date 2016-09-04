#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogles2/oglesconstantbuffer.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLESConstantBuffer::OGLESConstantBuffer( ConstantBuffer::ConstantBufferType type )
	: ConstantBuffer( type )
{
}

//============================================================================

OGLESConstantBuffer::~OGLESConstantBuffer()
{
	
}
//===========================================================================

void OGLESConstantBuffer::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	EffectPass* effectPass, 
	void* context 
	)
{
	ConstantBuffer::DispatchInternal( renderStateShadowing, effectPass, context );
}

#endif