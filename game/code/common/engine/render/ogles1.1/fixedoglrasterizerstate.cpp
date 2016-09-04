#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglrasterizerstate.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static int gCullFaceTranslation[] =
{   
	GL_FRONT_AND_BACK,	// CullMode_None
	GL_FRONT,			// CullMode_Front
	GL_BACK				// CullMode_Back
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLRasterizerState::FixedOGLRasterizerState() 
{    
}

//============================================================================

FixedOGLRasterizerState::~FixedOGLRasterizerState()
{
}

//============================================================================

void FixedOGLRasterizerState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context,
	bool forceUpload 
	)
{	
	UNUSED_ALWAYS( context );

	const RasterizerState* currentState = renderStateShadowing->GetCurrentRasterizerState();
	const RasterizerStateStruct* currentStateData = NULL;


	if ( currentState == this )
	{
		return;
	}

	if ( currentState != NULL )
	{
		currentStateData = currentState->GetData();
	}
	else
	{
		forceUpload = true;
	}

	if ( forceUpload == true || mData.mCullMode != currentStateData->mCullMode )
	{
		if ( mData.mCullMode == CullMode_None )
		{
			FixedOGL::oglDisable( GL_CULL_FACE );
		}
		else
		{
			FixedOGL::oglEnable( GL_CULL_FACE );
			FixedOGL::oglFrontFace( GL_CCW );
			FixedOGL::oglCullFace( gCullFaceTranslation[mData.mCullMode] );
		}
	}

	renderStateShadowing->SetCurrentRasterizerState( this );
}

#endif