//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/blenderasset.hpp"
#include "common/engine/render/blenderassetmanager.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// GLOBAL
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BlenderAssetManager::BlenderAssetManager()
{
}

//============================================================================

BlenderAssetManager::~BlenderAssetManager()
{
	for( BlenderAssetMap::iterator iter = mBlenderAssets.begin(); iter != mBlenderAssets.end(); ++iter )
	{
		BlenderAsset* blenderAsset = ( *iter ).second;
		DELETE_PTR( blenderAsset );
	}
}

//===========================================================================

BlenderAsset* BlenderAssetManager::GetBlenderAsset( const char* blenderAssetName, bool returnNULL )
{
	BlenderAssetMap::iterator findIter = mBlenderAssets.find( blenderAssetName );
	BlenderAsset* item = NULL;

	if ( returnNULL && findIter == mBlenderAssets.end() )
	{
		return NULL;
	}

	if ( findIter == mBlenderAssets.end() )
	{
		item = NEW_PTR( "BlenderAsset" ) BlenderAsset;
		item->Load( blenderAssetName );
		mBlenderAssets[blenderAssetName] = item;
	}	
	else
	{
		item = findIter->second;
	}

	return item;	
}