//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/rendernode.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effectpass.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/constantbuffer.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/render/rendertarget.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include "common/engine/system/profiler.hpp"
#include "common/engine/render/vertexdeclaration.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

const int MAX_NUM_FLOATS = 100 * 4;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RenderNode::RenderNode()
{    
	mEffect = NULL;
	mTechnique = 0;
	mSimpleMesh = NULL;
	mMesh = NULL;
	mRenderTarget = NULL;
	mScissorRectangle.SetPosition( 0, 0 );
	mScissorRectangle.SetHeight( 0 );
	mScissorRectangle.SetWidth( 0 );
#if defined ( _DEBUG )
	System::StringCopy( mDescription, 32, "" );
#endif

	mFloatData = NEW_PTR( "Float Data" ) float[MAX_NUM_FLOATS];
	mCurrentNumFloats= 0;
}

//============================================================================

RenderNode::~RenderNode()
{
	DELETE_PTR_ARRAY( mFloatData );
}

//============================================================================

void RenderNode::Clear()
{

#if defined ( _DEBUG )
	mDescription[0] = '\0';
#endif

	mRenderTarget = NULL;
	mEffect = NULL;
	mTechnique = 0;
	mSimpleMesh = NULL;
	mMesh = NULL;	
	mScissorRectangle.SetPosition( 0, 0 );
	mScissorRectangle.SetHeight( 0 );
	mScissorRectangle.SetWidth( 0 );

	mCurrentNumFloats = 0;	
}

//============================================================================

#if defined ( _DEBUG )
void RenderNode::SetDescription( const char* text )
{
	System::StringCopy( mDescription, 64, text );
}
#endif

//============================================================================

void RenderNode::Render( 
	RenderStateShadowing* renderStateShadowing, 
	int threadID,
	void* context 
	)
{
#if defined ( _DEBUG )
	Renderer::Get()->CaptureMarkBegin( mDescription );
#endif

    Renderer::Get()->SetMainContext();

	if ( mRenderTarget )
	{
		if ( mRenderTarget != renderStateShadowing->GetCurrentRenderTarget() )
		{        
			Renderer::Get()->SetRenderTarget( mRenderTarget, context );
			renderStateShadowing->SetCurrentRenderTarget( mRenderTarget );
			renderStateShadowing->SetCurrentRenderTargetType( RenderStateShadowing::RENDER_TARGET_TYPE_OTHER );
		}
	}
	else
	{
		if ( RenderStateShadowing::RENDER_TARGET_TYPE_MAIN != renderStateShadowing->GetCurrentRenderTargetType() )
		{  
			Renderer::Get()->SetRenderTarget( NULL, context );
			renderStateShadowing->SetCurrentRenderTarget( NULL );
			renderStateShadowing->SetCurrentRenderTargetType( RenderStateShadowing::RENDER_TARGET_TYPE_MAIN );
		}
	}    

	Renderer::Get()->SetScissorRectangle(
		mScissorRectangle.GetX(),
		mScissorRectangle.GetY(),
		mScissorRectangle.GetWidth(),
		mScissorRectangle.GetHeight(),
		mRenderTarget,
		context
		);

	EffectPass* effectPass = mEffect->GetTechnique( mTechnique )->GetPass( 0 );
	effectPass->Dispatch( renderStateShadowing, mConstantMap, threadID, context );	
    mEffect->GetVertexDeclaration()->CreateVertexDeclaration( effectPass );
    
	if ( mSimpleMesh )
	{
		Renderer::Get()->Render( mSimpleMesh, mEffect->GetVertexDeclaration(), effectPass, threadID, context );
	}
	
	if ( mMesh )
	{
		Renderer::Get()->RenderBuffers( mMesh, mEffect->GetVertexDeclaration(), effectPass, context );
	}

#if defined ( _DEBUG )
	Renderer::Get()->CaptureMarkEnd();
#endif
}

//============================================================================

void RenderNode::SetVector( int nameHash, const Math::Vector4* data, int num )
{    
	int numFloats = 4 * num;
	memcpy( &mFloatData[mCurrentNumFloats], data, sizeof( float ) * 4 * num );
	mConstantMap[nameHash] = &mFloatData[mCurrentNumFloats];
	mCurrentNumFloats += numFloats;
	Assert( mCurrentNumFloats < MAX_NUM_FLOATS, "" );
}

//=====================s=======================================================

void RenderNode::SetVector( int nameHash, const Math::Vector4 &data )
{    
	int numFloats = 4;
	memcpy( &mFloatData[mCurrentNumFloats], &data, sizeof( float ) * 4 );
	mConstantMap[nameHash] = &mFloatData[mCurrentNumFloats];
	mCurrentNumFloats += numFloats;
	Assert( mCurrentNumFloats < MAX_NUM_FLOATS, "" );
}

//============================================================================

void RenderNode::SetMatrix( int nameHash, const Math::Matrix44* data, int num )
{
 	int numFloats = 16 * num;
	memcpy( &mFloatData[mCurrentNumFloats], data, sizeof( float ) * numFloats );
	mConstantMap[nameHash] = &mFloatData[mCurrentNumFloats];
	mCurrentNumFloats += numFloats;
	Assert( mCurrentNumFloats < MAX_NUM_FLOATS, "" );
}

//============================================================================

void RenderNode::SetTexture( int nameHash, const Texture* texture )
{
	mConstantMap[nameHash] = texture;
}