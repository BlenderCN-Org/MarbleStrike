#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogl/oglrasterizerstate.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
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

static int gFillModeTranslation[] =
{   
	GL_LINE,	// FillMode_Wireframe
	GL_FILL		// FillMode_Solid
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLRasterizerState::OGLRasterizerState() 
{    
}

//============================================================================

OGLRasterizerState::~OGLRasterizerState()
{
}

//============================================================================

void OGLRasterizerState::Dispatch( 
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

	if ( forceUpload == true || mData.mFillMode != currentStateData->mFillMode )
	{
		OGL::oglPolygonMode( GL_FRONT_AND_BACK, gFillModeTranslation[mData.mFillMode] );
	}

	if ( forceUpload == true || 
		mData.mDepthBias != currentStateData->mDepthBias ||
		mData.mSlopeScaledDepthBias != currentStateData->mSlopeScaledDepthBias 
		)
	{
		OGL::oglEnable(GL_POLYGON_OFFSET_FILL);
		OGL::oglPolygonOffset( mData.mSlopeScaledDepthBias, static_cast<float>( mData.mDepthBias ) );
	}

	if ( forceUpload == true || mData.mScissorEnable != currentStateData->mScissorEnable )
	{
		if ( mData.mScissorEnable == false )
		{
			OGL::oglDisable( GL_SCISSOR_TEST );
		}
		else
		{
			OGL::oglEnable( GL_SCISSOR_TEST );						
		}
	}

	if ( forceUpload == true || mData.mCullMode != currentStateData->mCullMode )
	{
		if ( mData.mCullMode == CullMode_None )
		{
			OGL::oglDisable( GL_CULL_FACE );
		}
		else
		{
			OGL::oglEnable( GL_CULL_FACE );			
			OGL::oglCullFace( gCullFaceTranslation[mData.mCullMode] );
		}
	}

	renderStateShadowing->SetCurrentRasterizerState( this );
}

#endif