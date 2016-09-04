#if defined( RENDER_PLAT_OGLES2 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogles2/oglesrasterizerstate.hpp"
#include "common/engine/render/ogles2/ogleswrapper.hpp"
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

//static int gFillModeTranslation[] =
//{   
//	GL_LINE,	// FillMode_Wireframe
//	GL_FILL		// FillMode_Solid
//};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLESRasterizerState::OGLESRasterizerState() 
{    
}

//============================================================================

OGLESRasterizerState::~OGLESRasterizerState()
{
}

//============================================================================

void OGLESRasterizerState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* /*context*/,
	bool forceUpload 
	)
{	
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
		//OGL::oglPolygonMode( GL_FRONT_AND_BACK, gFillModeTranslation[mData.mFillMode] );
	}
    
    if ( forceUpload == true || 
		mData.mDepthBias != currentStateData->mDepthBias ||
		mData.mSlopeScaledDepthBias != currentStateData->mSlopeScaledDepthBias 
		)
	{
		OGLES::oglEnable(GL_POLYGON_OFFSET_FILL);
		OGLES::oglPolygonOffset( mData.mSlopeScaledDepthBias, static_cast<float>( mData.mDepthBias ) );
	}

	if ( forceUpload == true || mData.mCullMode != currentStateData->mCullMode )
	{
		if ( mData.mCullMode == CullMode_None )
		{
			OGLES::oglDisable( GL_CULL_FACE );
		}
		else
		{
			OGLES::oglEnable( GL_CULL_FACE );
			OGLES::oglFrontFace( GL_CCW );
			OGLES::oglCullFace( gCullFaceTranslation[mData.mCullMode] );
		}
	}

	renderStateShadowing->SetCurrentRasterizerState( this );
}

#endif