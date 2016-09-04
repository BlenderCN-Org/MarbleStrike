//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/blenderasset.hpp"
#include "common/engine/render/blenderassetmanager.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/cameravolume.hpp"
#include "common/engine/render/boundingvolume.hpp"
#include "common/engine/render/curve.hpp"
#include "common/engine/render/light.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/trigger.hpp"
#include "common/engine/system/fixedstring.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/pathutilities.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/render/followpathconstraint.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BlenderAsset::BlenderAsset()
{
}

//============================================================================

BlenderAsset::~BlenderAsset()
{
	int numMaterials = mMaterials.GetSize();
	for ( int i = 0; i < numMaterials; ++i )
	{
		if ( mMaterials[i].mIsProxy == false )
		{
			DELETE_PTR( mMaterials[i].mMaterial );
		}
	}

	size_t numCameras = mCameras.size();
	for ( int i = 0; i < numCameras; ++i )
	{
		DELETE_PTR( mCameras[i] );
	}

    size_t numBoundingVolumes = mBoundingVolumes.size();
	for ( int i = 0; i < numBoundingVolumes; ++i )
	{
		DELETE_PTR( mBoundingVolumes[i] );
	}

	size_t numCameraVolumes = mCameraVolumes.size();
	for ( int i = 0; i < numCameraVolumes; ++i )
	{
		DELETE_PTR( mCameraVolumes[i] );
	}

	size_t numLights = mLights.size();
	for ( int i = 0; i < numLights; ++i )
	{
		DELETE_PTR( mLights[i] );
	}

	size_t numMeshes = mMeshes.size();
	for ( int i = 0; i < numMeshes; ++i )
	{
		DELETE_PTR( mMeshes[i] );
	}

	size_t numCurves = mCurves.size();
	for ( int i = 0; i < numCurves; ++i )
	{
		DELETE_PTR( mCurves[i] );
	}

	size_t numEmptyObjects = mEmptyObjects.size();
	for ( int i = 0; i < numEmptyObjects; ++i )
	{
		DELETE_PTR( mEmptyObjects[i] );
	}

	int numObjects = mRenderObjects.GetSize();
	for ( int i = 0; i < numObjects; ++i )
	{
		DELETE_PTR( mRenderObjects[i] );
	}
}

//============================================================================

int BlenderAsset::GetNumMaterials()
{
	return mMaterials.GetSize();
}

//============================================================================

Material* BlenderAsset::GetMaterialByIndex( int index )
{
	return mMaterials[index].mMaterial;
}

//============================================================================

Material* BlenderAsset::GetMaterialByName( const char* materialName )
{
	int numMaterials = mMaterials.GetSize();
	for ( int i = 0; i < numMaterials; ++i )
	{
		Material* material = mMaterials[i].mMaterial;
        Assert( material != NULL, "" );
		if ( strcmp( materialName, material->GetMaterialName() ) == 0 )
		{
			return material;
		}
	}

	return NULL;
}

//============================================================================

int BlenderAsset::GetNumRenderObjects()
{
	return mRenderObjects.GetSize();
}

//============================================================================

RenderObject* BlenderAsset::GetRenderObjectByIndex( int index )
{
	return mRenderObjects[index];
}

//============================================================================

RenderObject* BlenderAsset::GetRenderObjectByName( const char* renderObjectName )
{
	int numRenderObjects = mRenderObjects.GetSize();
	for ( int i = 0; i < numRenderObjects; ++i )
	{
		RenderObject* renderObject = mRenderObjects[i];
		if ( strcmp( renderObject->GetObjectName(), renderObjectName ) == 0 )
		{
			return renderObject;
		}
	}

	return NULL;
}

//============================================================================

int BlenderAsset::GetNumCameraVolumes()
{
	return (int)mCameraVolumes.size();
}

//============================================================================

CameraVolume* BlenderAsset::GetCameraVolumeByIndex( int index )
{
	return mCameraVolumes[index];
}

//============================================================================

void BlenderAsset::Load( const char* assetFilename )
{
	char buffer[256];
	System::StringCopy( buffer, 256, assetFilename );	
	System::StringConcat( buffer, 256, ".xml" );
	System::FixedString<256> loadPath = System::Path::SystemSlash( buffer );

	char filenameFullPath[256];
	const char* resourcePath = Database::Get()->GetResourcePath();
	System::StringCopy( filenameFullPath, 256, resourcePath );
	System::StringConcat( filenameFullPath, 256, loadPath );

	FILE* file = System::OpenFile( filenameFullPath, "rb" );
	tinyxml2::XMLDocument doc;
    tinyxml2::XMLError loadOkay = doc.LoadFile( file );
	fclose( file );
	file = NULL;
    Assert( loadOkay == tinyxml2::XML_SUCCESS, "" );
	if ( loadOkay == tinyxml2::XML_SUCCESS )
	{
		tinyxml2::XMLNode* node = &doc;
		
		for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
		{
            tinyxml2::XMLElement* element = currentNode->ToElement();
			if ( element )
			{	
				const char* elementValue = element->Value();
				if ( strcmp( elementValue, "blenderasset" ) == 0 )
				{
					LoadBlenderAsset( currentNode );
				}
			}
		}
	}
}

//============================================================================

void BlenderAsset::LoadBlenderAsset( tinyxml2::XMLNode* node )
{
	tinyxml2::XMLElement* element = node->ToElement();
	
    int numObjects = element->IntAttribute( "ObjectCount" );
	mRenderObjects.SetSize( numObjects );

	int numMaterials = element->IntAttribute( "MaterialCount" );
	mMaterials.SetSize( numMaterials );

	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* currentElement = currentNode->ToElement();
		if ( currentElement )
		{	
			const char* currentElementValue = currentElement->Value();
			if ( strcmp( currentElementValue, "object" ) == 0 )
			{
				LoadObject( currentNode );
			}
			else if ( strcmp( currentElementValue, "material" ) == 0 )
			{
				const char* libFilePath = currentElement->Attribute( "lib_filepath" );
				const char* name = currentElement->Attribute( "name" );

				if ( libFilePath == NULL )
				{
					Material* newMaterial = NEW_PTR( "Material" ) Material;
					newMaterial->Load( currentNode );

					MaterialAsset newMaterialAsset;
					newMaterialAsset.mIsProxy = false;
					newMaterialAsset.mMaterial = newMaterial;
					mMaterials.PushBack( newMaterialAsset );
				}
				else
				{
					BlenderAsset* blenderAsset = GameApp::Get()->GetBlenderAssetManager()->GetBlenderAsset( libFilePath );
					Material* material = blenderAsset->GetMaterialByName( name );

					MaterialAsset newMaterialAsset;
					newMaterialAsset.mIsProxy = true;
					newMaterialAsset.mMaterial = material;
					mMaterials.PushBack( newMaterialAsset );					
				}
			}
		}		
	}

	int numRenderObjects = mRenderObjects.GetSize();
	for ( int i = 0; i < numRenderObjects; ++i )
	{
		FollowPathConstraint* followPathConstraint = mRenderObjects[i]->GetFollowPathConstraint();
		if ( followPathConstraint )
		{
			RenderObject* curveObject = GetRenderObjectByName( followPathConstraint->GetTargetName() );
			Assert( 
				curveObject->GetRenderObjectData() != NULL && 
				curveObject->GetRenderObjectData()->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_CURVE, "" 
				);
			followPathConstraint->SetTarget( curveObject );
		}

        const char* parentObject = mRenderObjects[i]->GetParentObjectName();
        if ( strcmp( parentObject, "" ) != 0 )
        {
            RenderObject* renderObject = GetRenderObjectByName( parentObject );
            if ( renderObject )
            {
                mRenderObjects[i]->SetParentObject( renderObject );
            }
        }
	}
}


//============================================================================

void BlenderAsset::LoadObject( tinyxml2::XMLNode* node )
{
	tinyxml2::XMLElement* element = node->ToElement();

	const char* custom_type = element->Attribute( "custom_type" );    
	const char* proxy = element->Attribute( "proxy" );

    Assert( custom_type != NULL, "" );

	RenderObjectDataType renderObjectDataType = RenderObjectData::GetRenderObjectDataType( custom_type );
	RenderObjectData* newRenderObjectData = NULL;
	
	RenderObject* renderObject = NEW_PTR( "RenderObject" ) RenderObject;	
	mRenderObjects.PushBack( renderObject );

	bool isProxy = true;
	if ( proxy == NULL || strcmp( proxy, "False" ) == 0 )
	{
		isProxy = false;
	}
	
	if ( isProxy == false )
	{
		switch ( renderObjectDataType )
		{
		case RENDER_OBJECT_DATA_TYPE_CAMERA:
			{
				Camera* newCamera = NEW_PTR( "Camera" ) Camera;
				mCameras.push_back( newCamera );
				newRenderObjectData = newCamera;
			}
			break;
		case RENDER_OBJECT_DATA_TYPE_CAMERA_VOLUME:
			{
				CameraVolume* newCameraVolume = NEW_PTR( "CameraVolume" ) CameraVolume;
				mCameraVolumes.push_back( newCameraVolume );
				newRenderObjectData = newCameraVolume;
			}
			break;
        case RENDER_OBJECT_DATA_TYPE_TRIGGER:
			{
				Trigger* newTrigger = NEW_PTR( "Trigger" ) Trigger;
				mTriggers.push_back( newTrigger );
				newRenderObjectData = newTrigger;
			}
			break;
		case RENDER_OBJECT_DATA_TYPE_LIGHT:
			{
				Light* newLight = NEW_PTR( "Light" ) Light;		
				mLights.push_back( newLight );
				newRenderObjectData = newLight;
			}
			break;
		case RENDER_OBJECT_DATA_TYPE_MESH:
			{
				Mesh* newMesh = NEW_PTR( "Mesh" ) Mesh;		
				mMeshes.push_back( newMesh );
				newRenderObjectData = newMesh;
			}
			break;
		case RENDER_OBJECT_DATA_TYPE_CURVE:
			{
				Curve* newCurve = NEW_PTR( "Curve" ) Curve;		
				mCurves.push_back( newCurve );
				newRenderObjectData = newCurve;
			}
			break;
		case RENDER_OBJECT_DATA_TYPE_EMPTY:
			{
				newRenderObjectData = NEW_PTR( "Empty" ) RenderObjectData( renderObjectDataType );
				mEmptyObjects.push_back( newRenderObjectData );
			}
			break;
		case RENDER_OBJECT_DATA_TYPE_BOUNDING_VOLUME:
			{
                BoundingVolume* newBoundingVolume = NEW_PTR( "BoundingVolume" ) BoundingVolume;
                mBoundingVolumes.push_back( newBoundingVolume );
				newRenderObjectData = newBoundingVolume;
			}
			break;
		default:
			Assert( false, "" );
			break;
		}

		if ( newRenderObjectData )
		{
			newRenderObjectData->Load( node );
			renderObject->SetRenderObjectData( newRenderObjectData );				
		}
	}
	else
	{
		const char* proxyFilePath = element->Attribute( "proxy_filepath" );
		const char* proxyObjectName = element->Attribute( "proxy_obj_name" );

		BlenderAsset* blenderAsset = GameApp::Get()->GetBlenderAssetManager()->GetBlenderAsset( proxyFilePath );
		RenderObject* proxyRenderObject = blenderAsset->GetRenderObjectByName( proxyObjectName );
		renderObject->SetProxyObject( proxyRenderObject );
        renderObject->SetGameType( proxyRenderObject->GetGameType() );
	}

	renderObject->Load( node );
}