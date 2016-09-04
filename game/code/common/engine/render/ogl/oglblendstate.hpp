#if defined( RENDER_PLAT_OPENGL )

#ifndef OGL_BLEND_STATE_HPP
#define OGL_BLEND_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/blendstate.hpp"

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

class OGLBlendState : public BlendState
{
public:

	OGLBlendState();
	~OGLBlendState();

	void Dispatch( 
		RenderStateShadowing* renderStateShadowing,
		void* context, 
		bool forceUpload = false 
		);
		
private:

};

#endif

#endif