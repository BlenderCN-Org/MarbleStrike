#if defined( RENDER_PLAT_OGLES1_1)

#ifndef FIXED_OGL_SAMPLER_STATE_HPP
#define FIXED_OGL_SAMPLER_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/samplerstate.hpp"

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

class FixedOGLSamplerState : public SamplerState
{
public:

	FixedOGLSamplerState();
	~FixedOGLSamplerState();

	void Dispatch(
		RenderStateShadowing* renderStateShadowing,
		void* context,
		int samplerIndex, 
		bool forceUpload = false 
		);	

	void SetTextureState();
		
private:

	static int MipMapFilterTranslation(  FilterMode minFilter, FilterMode mipFilter  );


};

#endif

#endif