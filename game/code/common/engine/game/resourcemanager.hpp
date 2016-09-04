#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/resource.hpp"
#include <list>
#include <thread>
#include <mutex>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum ResourceManagerState
{    
    RESOURCE_MANAGER_STATE_LOADING,
    RESOURCE_MANAGER_STATE_READY
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class ResourceManager
{
public:

    ResourceManager();
    ~ResourceManager();

    void ClearResources();    
    Resource* CreateResource( ResourceType type, const char* name );    
    void LoadResources();
    ResourceManagerState GetResourceManagerState();

private:

    static void LoadEntry( ResourceManager* resourceManager );

    std::thread mLoadThread;
    std::mutex mLoadThreadMutex;

    volatile ResourceManagerState mResourceManagerState;
    std::list<Resource*> mResourceList;	
};

#endif