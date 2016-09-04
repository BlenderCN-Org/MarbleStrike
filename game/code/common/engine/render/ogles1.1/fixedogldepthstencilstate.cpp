#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedogldepthstencilstate.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static int gCompareFunctionTranslation[] =
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
	GL_ZERO,		 //StencilOp_Increase_Saturate
	GL_ZERO,		 //StencilOp_Decrease_Saturate
	GL_INVERT,		 //StencilOp_Invert
	GL_INCR,		 //StencilOp_Increase
	GL_DECR,		 //StencilOp_Decrease
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLDepthStencilState::FixedOGLDepthStencilState() 
{    
}

//============================================================================

FixedOGLDepthStencilState::~FixedOGLDepthStencilState()
{
}

//============================================================================

void FixedOGLDepthStencilState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* /*context*/,
	bool forceUpload 
	)
{	
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
			FixedOGL::oglEnable( GL_DEPTH_TEST );
			FixedOGL::oglDepthFunc( gCompareFunctionTranslation[mData.mDepthFunc] );
		}
		else
		{
			FixedOGL::oglDisable( GL_DEPTH_TEST );
		}
	}

	if ( forceUpload == true || mData.mDepthWriteMask != currentStateData->mDepthWriteMask )
	{
		FixedOGL::oglDepthMask( mData.mDepthWriteMask );
	}

	if ( 
		forceUpload == true || 
		mData.mStencilEnable != currentStateData->mStencilEnable || 
		mData.mStencilRef != currentStateData->mStencilRef || 
		mData.mStencilReadMask != currentStateData->mStencilReadMask || 
		mData.mFrontFace.mStencilFailOp != currentStateData->mFrontFace.mStencilFailOp || 
		mData.mFrontFace.mStencilDepthFailOp != currentStateData->mFrontFace.mStencilDepthFailOp || 
		mData.mFrontFace.mStencilPassOp != currentStateData->mFrontFace.mStencilPassOp
		)
	{
		if ( mData.mStencilEnable )
		{
			FixedOGL::oglEnable( GL_STENCIL_TEST );
			FixedOGL::oglStencilFunc( 
				gCompareFunctionTranslation[mData.mFrontFace.mStencilFunc], 
				mData.mStencilRef,
				mData.mStencilReadMask
				);
			FixedOGL::oglStencilOp(
				gStencilOpTranslation[mData.mFrontFace.mStencilFailOp],
				gStencilOpTranslation[mData.mFrontFace.mStencilDepthFailOp],
				gStencilOpTranslation[mData.mFrontFace.mStencilPassOp]
			);
		}
		else
		{
			FixedOGL::oglDisable( GL_STENCIL_TEST );
		}
	}

	renderStateShadowing->SetCurrentDepthStencilState( this );
}

#endif