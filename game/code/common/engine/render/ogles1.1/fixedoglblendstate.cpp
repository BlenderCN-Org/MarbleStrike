#if defined( RENDER_PLAT_OGLES1_1)

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "iphone/render/ogles1.1/fixedoglblendstate.hpp"
#include "iphone/render/ogles1.1/fixedoglwrapper.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/renderstatemanager.hpp"
#include "common/engine/render/renderstateshadowing.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

static int gBlendTranslation[] =
{   
	GL_ZERO,				// Blend_Zero
	GL_ONE,					// Blend_One
	GL_SRC_COLOR,			// Blend_Src_Color
	GL_ONE_MINUS_SRC_COLOR,	// Blend_Inv_Src_Color
	GL_SRC_ALPHA,			// Blend_Src_Alpha
	GL_ONE_MINUS_SRC_ALPHA,	// Blend_Inv_Src_Alpha
	GL_DST_ALPHA,			// Blend_Dest_Alpha
	GL_ONE_MINUS_DST_ALPHA,	// Blend_Inv_Dest_Alpha
	GL_DST_COLOR,			// Blend_Dest_Color
	GL_ONE_MINUS_DST_COLOR,	// Blend_Inv_Dest_Color
	GL_SRC_ALPHA_SATURATE,	// Blend_Src_Alpha_Sat
	GL_ZERO,				// Blend_Blend_Factor
	GL_ZERO,				// Blend_Inv_Blend_Factor
	GL_ZERO,				// Blend_Src1_Color
	GL_ZERO,				// Blend_Inv_Src1_Color
	GL_ZERO,				// Blend_Src1_Alpha
	GL_ZERO,				// Blend_Inv_Src1_Alpha
};

static int gAlphaFuncComparisonTranslation[] =
{
	GL_NEVER,		// AlphaFuncCompare_Never
	GL_LESS,		// AlphaFuncCompare_Less
	GL_EQUAL,		// AlphaFuncCompare_Equal
	GL_LEQUAL,		// AlphaFuncCompare_Less_Equal
	GL_GREATER,		// AlphaFuncCompare_Greater
	GL_NOTEQUAL,	// AlphaFuncCompare_Not_Equal
	GL_GEQUAL,		// AlphaFuncCompare_Greater_Equal
	GL_ALWAYS		// AlphaFuncCompare_Always
};

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

FixedOGLBlendState::FixedOGLBlendState() 
{    
}

//============================================================================

FixedOGLBlendState::~FixedOGLBlendState()
{
}

//============================================================================

void FixedOGLBlendState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* /*context*/,
	bool forceUpload 
	)
{		
	const BlendState* currentState = renderStateShadowing->GetCurrentBlendState();
	const BlendStateStruct* currentStateData = NULL;
	
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
		mData.mBlendEnable[0] != currentStateData->mBlendEnable[0] ||
		mData.mSrcBlend != currentStateData->mSrcBlend ||
		mData.mDestBlend != currentStateData->mDestBlend		
		)
	{
		if ( mData.mBlendEnable[0] == true )
		{
			FixedOGL::oglEnable( GL_BLEND );
			FixedOGL::oglBlendFunc( gBlendTranslation[mData.mSrcBlend], gBlendTranslation[mData.mDestBlend] );
		}
		else
		{
			FixedOGL::oglDisable( GL_BLEND );
		}
	}

	if ( forceUpload == true ||
		mData.mAlphaTestEnable != currentStateData->mAlphaTestEnable ||
		mData.mAlphaRef != currentStateData->mAlphaRef ||
		mData.mAlphaFunc != currentStateData->mAlphaFunc
		)
	{
		if ( mData.mAlphaTestEnable )
		{
			FixedOGL::oglEnable( GL_ALPHA_TEST );
			float alphaRef = static_cast<float>( mData.mAlphaRef ) / 255.0f;
			FixedOGL::oglAlphaFunc( gAlphaFuncComparisonTranslation[mData.mAlphaFunc], alphaRef );
		}
		else
		{
			FixedOGL::oglDisable( GL_ALPHA_TEST );
		}
	}

	renderStateShadowing->SetCurrentBlendState( this );
}

#endif