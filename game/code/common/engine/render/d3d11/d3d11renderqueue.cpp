#if defined( RENDER_PLAT_D3D11 )

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/d3d11/d3d11renderer.hpp"
#include "common/engine/render/d3d11/d3d11renderqueue.hpp"
#include "common/engine/render/rendernode.hpp"
#include "common/engine/system/task.hpp"
#include "common/engine/system/taskmanager.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

D3D11RenderQueue::D3D11RenderQueue()
{
	D3D11Renderer* renderer = reinterpret_cast<D3D11Renderer*>( Renderer::Get() );
	mNumThreads = renderer->GetNumThreads();
	mDeferredContext = NEW_PTR( "Deferred Context" )ID3D11DeviceContext*[mNumThreads];
	mCommandList = NEW_PTR( "Command List" )ID3D11CommandList*[mNumThreads];
	for ( int i = 0; i < mNumThreads; ++i )
	{
		HRESULT result = D3D11Renderer::GetDevice()->CreateDeferredContext( 
			0 /*Reserved for future use*/, 
			&mDeferredContext[i]
		);
		Assert( result == S_OK, "" );
        UNUSED_ALWAYS( result );

		mCommandList[i] = NULL;
	}

	mRenderNodeThreadData = NEW_PTR( "Thread Data" ) RenderNodeThreadData[mNumThreads];
	mTasks = NEW_PTR( "Task" ) System::Task[mNumThreads];
}

//============================================================================

D3D11RenderQueue::~D3D11RenderQueue()
{
	DELETE_PTR_ARRAY( mCommandList );

	DELETE_PTR_ARRAY( mRenderNodeThreadData );
	DELETE_PTR_ARRAY( mTasks );

	for ( int i = 0; i < mNumThreads; ++i )
	{
		mDeferredContext[i]->Release();
	}
	DELETE_PTR_ARRAY( mDeferredContext );
}

void* RenderNodeThread( void* data, int threadID )
{
	data;threadID;
	RenderNodeThreadData* renderNodeThreadData = reinterpret_cast<RenderNodeThreadData*>( data );

	std::list<RenderNode*>::iterator iter = renderNodeThreadData->mRenderNodes.begin();
	for ( iter; iter != renderNodeThreadData->mRenderNodes.end(); ++iter )
	{
		RenderNode* node = *iter;
		node->Render( 
			&renderNodeThreadData->mRenderStateShadowing,
			threadID,
			(void*)renderNodeThreadData->mDeferredContext[threadID]		
		);
	}

	return NULL;
}

//============================================================================

void D3D11RenderQueue::Render()
{
#if defined( SINGLE_THREAD_RENDER )
	
	RenderQueue::Render();

#elif defined( MULTI_THREAD_RENDER )

	System::TaskManager* taskManager = GameApp::Get()->GetTaskManager();

	int numNodesPerThread = ( mNumCurrentRenderNodes + mNumThreads - 1 ) / mNumThreads;
	int nodeIndex = 0;

	for ( int i = 0; i < mNumThreads; ++i )
	{
		mTasks[i].SetEntryPoint( RenderNodeThread );

		mRenderNodeThreadData[i].mRenderStateShadowing.Clear();
		mRenderNodeThreadData[i].mRenderNodes.clear();
		
		for ( int nodeCount = 0; nodeCount < numNodesPerThread; ++nodeCount )
		{
			if ( nodeIndex < mNumCurrentRenderNodes )
			{
				mRenderNodeThreadData[i].mRenderNodes.push_back( &mRenderNodes[nodeIndex] );
				nodeIndex++;
			}
			else
			{
				break;
			}
		}		

		mRenderNodeThreadData[i].mDeferredContext = mDeferredContext;
		mRenderNodeThreadData[i].mCommandList = mCommandList;

		mTasks[i].SetArguments( (void*)&mRenderNodeThreadData[i] );
		taskManager->AddTask( &mTasks[i] );
	}

	for ( int i = 0; i < mNumThreads; ++i )
	{
		mTasks[i].WaitOn();
	}
	
	for ( int i = 0; i < mNumThreads; ++i )
	{
		mDeferredContext[i]->FinishCommandList( 
			false, 
			&mCommandList[i] 
		);

		if ( mCommandList[i] )
		{
			D3D11Renderer::GetImmediateDeviceContext()->ExecuteCommandList( 
				mCommandList[i], 
				false 
				);
			RELEASE_D3D_OBJECT( mCommandList[i] );
		}
	}

	for ( int i = 0; i < mNumCurrentRenderNodes; ++i )
	{
		mRenderNodes[i].Clear();
	}

	mNumCurrentRenderNodes = 0;
#endif
}

#endif