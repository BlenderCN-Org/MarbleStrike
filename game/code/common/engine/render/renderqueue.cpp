//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/rendernode.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/render/effect.hpp"
#include "common/engine/render/effecttechnique.hpp"
#include "common/engine/render/effectpass.hpp"
#include <vector>
#include <stdlib.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

const int MAX_ALPHA_OBJECTS = 300;

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RenderQueue::RenderQueue()
{
	mNumCurrentRenderNodes = 0;
	mRenderNodes = NEW_PTR( "Render Nodes" ) RenderNode[MAX_RENDER_NODES];
}

//============================================================================

RenderQueue::~RenderQueue()
{
	DELETE_PTR_ARRAY( mRenderNodes );
}

//============================================================================

void RenderQueue::ClearStates()
{
	mRenderStateShadowing.Clear();
}

//============================================================================

RenderNode* RenderQueue::CreateRenderNode()
{
	RenderNode* renderNode = &( mRenderNodes[mNumCurrentRenderNodes] );
	mNumCurrentRenderNodes++;
	Assert( mNumCurrentRenderNodes < MAX_RENDER_NODES, "" );
	return renderNode;
}

//============================================================================

void RenderQueue::Render()
{
	RenderNode* alphaObjects[MAX_ALPHA_OBJECTS];
	int numAlphaObjects = 0;

	for ( int i = 0; i < mNumCurrentRenderNodes; ++i )
	{
		RenderNode* currentNode = &mRenderNodes[i];
		Effect* currentEffect = currentNode->mEffect;
		EffectPass* currentEffectPass = currentEffect->GetTechnique( currentNode->mTechnique )->GetPass( 0 );
		if ( currentEffectPass->IsTranslucent() )
		{
			if ( numAlphaObjects < MAX_ALPHA_OBJECTS )
			{
				alphaObjects[numAlphaObjects++] = currentNode;
			}
			else
			{
				Assert( false, "Too many alpha objects." );
			}

		}
		else
		{
			mRenderNodes[i].Render( &mRenderStateShadowing, 0, NULL );
		}
	}

	for ( int i = 0; i < numAlphaObjects; ++i )
	{
		alphaObjects[i]->Render( &mRenderStateShadowing, 0, NULL );
	}

	for ( int i = 0; i < mNumCurrentRenderNodes; ++i )
	{
		mRenderNodes[i].Clear();
	}

	mNumCurrentRenderNodes = 0;	
}