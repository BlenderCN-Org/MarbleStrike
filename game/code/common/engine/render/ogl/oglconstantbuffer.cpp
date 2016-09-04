#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ogl/oglconstantbuffer.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLConstantBuffer::OGLConstantBuffer( ConstantBuffer::ConstantBufferType type )
	:	ConstantBuffer( type )
{
}

//============================================================================

OGLConstantBuffer::~OGLConstantBuffer()
{
	
}
//===========================================================================

void OGLConstantBuffer::Dispatch( 
	RenderStateShadowing* renderStateShadowing, 
	EffectPass* effectPass,
	void* context 
	)
{
	ConstantBuffer::DispatchInternal( renderStateShadowing, effectPass, context );
}

#endif