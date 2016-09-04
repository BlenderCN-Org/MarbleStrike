#ifndef BLENDER_ASSET_HPP
#define BLENDER_ASSET_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/fixedarray.hpp"
#include "tinyxml2.h"
#include <vector>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Camera;
class CameraVolume;
class BoundingVolume;
class Curve;
class Light;
class Material;
class Mesh;
class RenderObject;
class RenderObjectData;
class Trigger;

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct MaterialAsset
{
	Material* mMaterial;
	bool mIsProxy;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BlenderAsset
{
public:
   
    BlenderAsset();
    ~BlenderAsset();

	int GetNumMaterials();
	Material* GetMaterialByIndex( int index );
	Material* GetMaterialByName( const char* materialName );

	int GetNumRenderObjects();
	RenderObject* GetRenderObjectByIndex( int index );
	RenderObject* GetRenderObjectByName( const char* renderObjectName );

	int GetNumCameraVolumes();
	CameraVolume* GetCameraVolumeByIndex( int index );
	
	void Load( const char* assetFilename );

private:

	void LoadBlenderAsset( tinyxml2::XMLNode* node );
	void LoadObject( tinyxml2::XMLNode* node );
	
	std::vector<Camera*> mCameras;	
	std::vector<CameraVolume*> mCameraVolumes;
    std::vector<BoundingVolume*> mBoundingVolumes;
    std::vector<Trigger*> mTriggers;
	std::vector<Light*> mLights;
	std::vector<Mesh*> mMeshes;
	std::vector<Curve*> mCurves;
	std::vector<RenderObjectData*> mEmptyObjects;

	FixedArray<MaterialAsset> mMaterials;
	FixedArray<RenderObject*> mRenderObjects;
};

#endif
