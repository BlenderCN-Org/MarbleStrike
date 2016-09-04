#if defined( RENDER_PLAT_OPENGL )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/ogl/oglblendstate.hpp"
#include "common/engine/render/ogl/oglwrapper.hpp"
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

static int gBlendOpTranslation[] =
{
	GL_FUNC_ADD_EXT,				// BlendOp_Add
	GL_FUNC_SUBTRACT_EXT,			// BlendOp_Subtract
	GL_FUNC_REVERSE_SUBTRACT_EXT,	// BlendOp_Rev_Subtract
	GL_MIN_EXT,						// BlendOp_Min
	GL_MAX_EXT,						// BlendOp_Max
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

OGLBlendState::OGLBlendState() 
{    
}

//============================================================================

OGLBlendState::~OGLBlendState()
{
}

//============================================================================

void OGLBlendState::Dispatch( 
	RenderStateShadowing* renderStateShadowing,
	void* context, 
	bool forceUpload 
	) 
{		
	UNUSED_ALWAYS( context );

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
		mData.mDestBlend != currentStateData->mDestBlend ||
		mData.mSrcBlendAlpha != currentStateData->mSrcBlendAlpha ||
		mData.mDestBlendAlpha != currentStateData->mDestBlendAlpha ||
		mData.mBlendOp != currentStateData->mBlendOp ||
		mData.mBlendOpAlpha != currentStateData->mBlendOpAlpha		
		)
	{
		if ( mData.mBlendEnable[0] == true )
		{
			OGL::oglEnable( GL_BLEND );
			OGL::oglBlendFuncSeparate( 
				gBlendTranslation[mData.mSrcBlend], 
				gBlendTranslation[mData.mDestBlend],
				gBlendTranslation[mData.mSrcBlendAlpha],
				gBlendTranslation[mData.mDestBlendAlpha]
			);

			OGL::oglBlendEquationSeparate( 
				gBlendOpTranslation[mData.mBlendOp],
				gBlendOpTranslation[mData.mBlendOpAlpha]
			);
		}
		else
		{
			OGL::oglDisable( GL_BLEND );
		}
	}

	if ( 
		forceUpload == true ||
		mData.mAlphaTestEnable != currentStateData->mAlphaTestEnable ||
		mData.mAlphaRef != currentStateData->mAlphaRef ||
		mData.mAlphaFunc != currentStateData->mAlphaFunc		
		)
	{
		if ( mData.mAlphaTestEnable )
		{
			OGL::oglEnable( GL_ALPHA_TEST );
			float alphaRef = static_cast<float>( mData.mAlphaRef ) / 255.0f;
			OGL::oglAlphaFunc( gAlphaFuncComparisonTranslation[mData.mAlphaFunc], alphaRef );
		}
		else
		{
			OGL::oglDisable( GL_ALPHA_TEST );
		}
	}

	renderStateShadowing->SetCurrentBlendState( this );
}

#endif