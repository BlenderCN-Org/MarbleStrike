//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/resource.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/blenderasset.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/render/image.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Resource::Resource( ResourceType type, const char* filename )
{
    System::StringCopy( mName, 64, filename );
    mResourceType = type;
    mData = NULL;
}

//===========================================================================

Resource::~Resource()
{
    switch ( mResourceType )
    {
    case RESOURCE_BLENDER_ASSET:
        {			
            BlenderAsset* data = reinterpret_cast<BlenderAsset*>( mData );
            DELETE_PTR( data );
        }
        break;
    case RESOURCE_IMAGE:
        {
            Image* data = reinterpret_cast<Image*>( mData );
            DELETE_PTR( data );
        }
        break;
    default:
        Assert( false, "" );
        break;
    }

}

//===========================================================================

void Resource::Load()
{
    switch ( mResourceType )
    {
    case RESOURCE_BLENDER_ASSET:
        {
            BlenderAsset* blenderAsset = NEW_PTR( "Blender Asset" ) BlenderAsset;
            blenderAsset->Load( mName );
            mData = reinterpret_cast<void*>( blenderAsset );
        }
        break;
    case RESOURCE_IMAGE:
        {
            Image* image = NEW_PTR( "Image" ) Image;
            image->Load( mName );
            mData = reinterpret_cast<void*>( image );			
        }
        break;
    default:
        Assert( false, "" );
        break;
    }
}

//===========================================================================

void Resource::Update()
{

}

//===========================================================================

void* Resource::GetData()
{
    return mData;
}