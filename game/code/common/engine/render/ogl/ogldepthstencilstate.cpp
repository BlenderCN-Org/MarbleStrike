#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogl/ogldepthstencilstate.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static GLenum gCompareFunctionTranslation[] =
{	
	GL_NEVER,		//CompareFunc_Never
	GL_LESS,		//CompareFunc_Less
	GL_EQUAL,		//CompareFunc_Equal
	GL_LEQUAL,		//CompareFunc_Less_Equal
	GL_GREATER,		//CompareFunc_Greater
	GL_NOTEQUAL,	//CompareFunc_Not_Equal
	GL_GEQUAL,		//CompareFunc_Greater_Equal
	GL_ALWAYS		//CompareFunc_Always
};

static GLenum gStencilOpTranslation[] =
{
	GL_KEEP,		 //StencilOp_Keep
	GL_ZERO,		 //StencilOp_Zero
	GL_REPLACE,		 //StencilOp_Replace
	GL_INCR_WRAP,	 //StencilOp_Increase_Saturate
	GL_DECR_WRAP,	 //StencilOp_Decrease_Saturate
	GL_INVERT,		 //StencilOp_Invert
	GL_INCR,		 //StencilOp_Increase
	GL_DECR,		 //StencilOp_Decrease
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OGLDepthStencilState::OGLDepthStencilState() 
{    
}

//============================================================================

OGLDepthStencilState::~OGLDepthStencilState()
{
}

//============================================================================

void OGLDepthStencilState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context, 
	bool forceUpload 
	)
{	
	UNUSED_ALWAYS( context );

	const DepthStencilState* currentState = renderStateShadowing->GetCurrentDepthStencilState();
	const DepthStencilStateStruct* currentStateData = NULL;

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

	if ( 
		forceUpload == true ||
		mData.mDepthEnable != currentStateData->mDepthEnable ||
		mData.mDepthFunc != currentStateData->mDepthFunc		
		)
	{
		if ( mData.mDepthEnable )
		{
			OGL::oglEnable( GL_DEPTH_TEST );
			OGL::oglDepthFunc( gCompareFunctionTranslation[mData.mDepthFunc] );
		}
		else
		{
			OGL::oglDisable( GL_DEPTH_TEST );
		}
	}

	if ( forceUpload == true || mData.mDepthWriteMask != currentStateData->mDepthWriteMask )
	{
		OGL::oglDepthMask( mData.mDepthWriteMask );
	}	

	if ( 
		forceUpload == true || 
		mData.mStencilEnable != currentStateData->mStencilEnable || 
		mData.mStencilRef != currentStateData->mStencilRef || 
		mData.mStencilReadMask != currentStateData->mStencilReadMask || 
		mData.mFrontFace.mStencilFailOp != currentStateData->mFrontFace.mStencilFailOp || 
		mData.mFrontFace.mStencilDepthFailOp != currentStateData->mFrontFace.mStencilDepthFailOp || 
		mData.mFrontFace.mStencilPassOp != currentStateData->mFrontFace.mStencilPassOp || 
		mData.mBackFace.mStencilFailOp != currentStateData->mBackFace.mStencilFailOp ||
		mData.mBackFace.mStencilDepthFailOp != currentStateData->mBackFace.mStencilDepthFailOp ||
		mData.mBackFace.mStencilFailOp != currentStateData->mBackFace.mStencilFailOp
		)
	{
		if ( mData.mStencilEnable )
		{
			OGL::oglEnable( GL_STENCIL_TEST );
			OGL::oglStencilFuncSeparate( 
				GL_FRONT, 
				gCompareFunctionTranslation[mData.mFrontFace.mStencilFunc], 
				mData.mStencilRef,
				mData.mStencilReadMask
				);
			OGL::oglStencilOpSeparate(
				GL_FRONT,
				gStencilOpTranslation[mData.mFrontFace.mStencilFailOp],
				gStencilOpTranslation[mData.mFrontFace.mStencilDepthFailOp],
				gStencilOpTranslation[mData.mFrontFace.mStencilPassOp]
			);
			OGL::oglStencilFuncSeparate( 
				GL_BACK, 
				gCompareFunctionTranslation[mData.mBackFace.mStencilFunc], 
				mData.mStencilRef,
				mData.mStencilReadMask
				);
			OGL::oglStencilOpSeparate(
				GL_BACK,
				gStencilOpTranslation[mData.mBackFace.mStencilFailOp],
				gStencilOpTranslation[mData.mBackFace.mStencilDepthFailOp],
				gStencilOpTranslation[mData.mBackFace.mStencilPassOp]
			);

		}
		else
		{
			OGL::oglDisable( GL_STENCIL_TEST );
		}
	}

	renderStateShadowing->SetCurrentDepthStencilState( this );
}

#endif