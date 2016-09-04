//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/resourcemanager.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/render/renderer.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

ResourceManager::ResourceManager()
{
    mResourceManagerState = RESOURCE_MANAGER_STATE_READY;
}

//===========================================================================

ResourceManager::~ResourceManager()
{
    ClearResources();
}

//===========================================================================

void ResourceManager::ClearResources()
{
    std::lock_guard<std::mutex> lock(mLoadThreadMutex);

    for ( std::list<Resource*>::iterator iter = mResourceList.begin(); iter != mResourceList.end(); ++iter )
    {
        DELETE_PTR( *iter );
    }
    
    mResourceList.clear();
}

//===========================================================================

Resource* ResourceManager::CreateResource( ResourceType type, const char* name )
{
    Resource* newResource = NEW_PTR( "Resource" ) Resource( type, name );
    mResourceList.push_back( newResource );    
    return newResource;
}

//===========================================================================

void ResourceManager::LoadResources()
{
#if defined( RENDER_PLAT_D3D11 ) || defined( PLAT_IOS ) || defined( RENDER_PLAT_OPENGL ) || defined( RENDER_PLAT_D3D9 )
    mLoadThread = std::thread( LoadEntry, this );
    mLoadThread.detach();
    mResourceManagerState = RESOURCE_MANAGER_STATE_LOADING;
#else
	LoadEntry( this );
#endif
}

//===========================================================================

ResourceManagerState ResourceManager::GetResourceManagerState()
{
    return mResourceManagerState;
}

//===========================================================================

void ResourceManager::LoadEntry( ResourceManager* resourceManager )
{
    std::lock_guard<std::mutex> lock( resourceManager->mLoadThreadMutex );

#if !defined( PLAT_ANDROID )
    Renderer::Get()->SetThreadContext();
#endif

    for ( std::list<Resource*>::iterator iter = resourceManager->mResourceList.begin(); iter != resourceManager->mResourceList.end(); ++iter )
    {
        ( *iter )->Load();
    }

    Renderer::Get()->FlushContext();

    resourceManager->mResourceManagerState = RESOURCE_MANAGER_STATE_READY;
}