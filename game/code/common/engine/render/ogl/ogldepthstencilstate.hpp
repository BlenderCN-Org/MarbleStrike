#if defined( RENDER_PLAT_OPENGL )

#ifndef OGL_DEPTH_STENCIL_STATE_HPP
#define OGL_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/depthstencilstate.hpp"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OGLDepthStencilState : public DepthStencilState
{
public:

	OGLDepthStencilState();
	~OGLDepthStencilState();

	void Dispatch( 
		RenderStateShadowing* renderStateShadowing,
		void* context, 
		bool forceUpload = false 
		);
		
private:

};

#endif

#endif