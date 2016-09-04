#ifndef RESOURCE_HPP
#define RESOURCE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum ResourceType
{
    RESOURCE_BLENDER_ASSET,	
    RESOURCE_IMAGE,
    RESOURCE_UNKNOWN
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Resource
{
public:

    static void* LoadEntry( void* data, int threadID );

    Resource( ResourceType type, const char* filename );
    ~Resource();

    const char* GetName()
    {
        return mName;
    }

    ResourceType GetResourceType()
    {
        return mResourceType;
    }

    void Load();
    void Update();
    bool IsReady();
    void* GetData();

private:

    char mName[64];
    ResourceType mResourceType;    
    void* mData;
};

#endif
