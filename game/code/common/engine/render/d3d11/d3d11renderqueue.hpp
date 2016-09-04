#if defined( RENDER_PLAT_D3D11 )

#ifndef D3D11_RENDERQUEUE_HPP
#define D3D11_RENDERQUEUE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/renderstateshadowing.hpp"
#include <d3d11.h>
#include <list>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderNode;

namespace System
{
	class Task;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct RenderNodeThreadData
{
	std::list<RenderNode*> mRenderNodes;
	ID3D11DeviceContext** mDeferredContext;
	ID3D11CommandList** mCommandList;
	RenderStateShadowing mRenderStateShadowing;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class D3D11RenderQueue : public RenderQueue
{
public:

	D3D11RenderQueue();
	~D3D11RenderQueue();

	void Render();
	int GetNumThreads();

private:

	ID3D11DeviceContext** mDeferredContext;
	ID3D11CommandList** mCommandList;
	int mNumThreads;

	RenderNodeThreadData* mRenderNodeThreadData;
	System::Task* mTasks;
};

#endif

#endif