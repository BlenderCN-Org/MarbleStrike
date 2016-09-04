#ifndef BLENDER_ASSET_MANAGER_HPP
#define BLENDER_ASSET_MANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////


#include "common/engine/render/blenderasset.hpp"
#include <string>
#include <map>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BlenderAsset;

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BlenderAssetManager
{
public:

	BlenderAssetManager();
	~BlenderAssetManager();

	BlenderAsset* GetBlenderAsset( const char* blenderAssetName, bool returnNULL = false );
	
private:

	typedef std::map< std::string, BlenderAsset* > BlenderAssetMap;
	BlenderAssetMap mBlenderAssets;
	
};

#endif
