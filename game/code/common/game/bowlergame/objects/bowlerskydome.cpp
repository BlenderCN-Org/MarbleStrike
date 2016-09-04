//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/game/gameapp.hpp"
#include "common/engine/render/blenderasset.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/material.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/render/effectmanager.hpp"
#include "common/engine/render/renderutilities.hpp"
#include "common/engine/render/rendernode.hpp"
#include "common/engine/render/renderqueue.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/game/bowlergame/objects/bowlerskydome.hpp"
#include "common/engine/render/texturemanager.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

BowlerSkyDome::BowlerSkyDome()
{
    mSkyDomeEffect = GameApp::Get()->GetEffectManager()->GetEffect( "skydome" );
    mBlenderAsset = NEW_PTR( "BlenderAsset" ) BlenderAsset;
    mBlenderAsset->Load( "game\\bowler\\skydome");
    mRenderTarget = NULL;
    mDiffuseColorHash = System::StringHash( "gDiffuseColor" );
    mObjectToProjectionMatrixHash = System::StringHash( "gObjectToProjectionMatrix" );
	mObjectToCameraMatirxHash = System::StringHash( "gObjectToCameraMatrix" );
    mColorTextureHash = System::StringHash( "gColorTexture" );
	mEmptyTexture = GameApp::Get()->GetTextureManager()->GetTexture( "emptytexture" );
}

//===========================================================================

BowlerSkyDome::~BowlerSkyDome()
{
    DELETE_PTR( mBlenderAsset );
}

//===========================================================================

void BowlerSkyDome::Initialize()
{
    int numObjects = mBlenderAsset->GetNumRenderObjects();
    for ( int i = 0; i < numObjects; ++i )
    {
        RenderObject* renderObject = mBlenderAsset->GetRenderObjectByIndex( i );
        renderObject->Initialize();
    }
}

//===========================================================================

void BowlerSkyDome::Update( float elapsedTime )
{
    int numObjects = mBlenderAsset->GetNumRenderObjects();
    for ( int i = 0; i < numObjects; ++i )
    {
        RenderObject* renderObject = mBlenderAsset->GetRenderObjectByIndex( i );
        renderObject->Update( elapsedTime );
    }
}

//===========================================================================

void BowlerSkyDome::RenderUpdate( const Math::Matrix44 &cameraTransform, const Camera* camera )
{
    Math::Matrix44 projectionMatrix = Renderer::Get()->GetPerspectiveProjection(
        camera->GetFieldOfView(),
        camera->GetAspectRatio(),
        camera->GetZNear(),
        camera->GetZFar()
        );
    projectionMatrix = Render::ProjectionOrientation( projectionMatrix );

    Math::Matrix44 transform;
    Math::Matrix44 viewMatrix = cameraTransform;
    viewMatrix.InverseAffine();
    Math::Matrix44 objectToProjection;
	Math::Matrix44 objectToCamera;

    int numObjects = mBlenderAsset->GetNumRenderObjects();
    for ( int i = 0; i < numObjects; ++i )
    {
        RenderObject* renderObject = mBlenderAsset->GetRenderObjectByIndex( i );
        RenderObjectData* renderData = renderObject->GetRenderObjectData();

        if ( renderData->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_MESH )
        {
            Mesh* mesh = reinterpret_cast<Mesh*>( renderData );
            transform = renderObject->GetTransform();

            objectToProjection = projectionMatrix * viewMatrix * transform;
			objectToCamera = viewMatrix * transform;

            Material* material = mBlenderAsset->GetMaterialByName( mesh->GetMaterialName() );            
            Assert( material != NULL, "" );

            RenderNode* renderNode = GameApp::Get()->GetRenderQueue()->CreateRenderNode();
            Texture* colorTexture = material->GetColorTexture();
            if ( colorTexture == NULL )
			{
				colorTexture = mEmptyTexture;
			}

#if defined ( _DEBUG )
            char buffer[256];
            snprintf( buffer, 256, "RenderSkyDome - %s", mesh->GetMeshName() );
            renderNode->SetDescription( buffer );
#endif
            renderNode->mEffect = material->GetEffect();
            renderNode->mMesh = mesh;
            renderNode->mRenderTarget = mRenderTarget;

            renderNode->SetMatrix( mObjectToProjectionMatrixHash, &objectToProjection );
			renderNode->SetMatrix( mObjectToCameraMatirxHash, &objectToCamera );
            renderNode->SetTexture( mColorTextureHash, colorTexture );				
            renderNode->SetVector( mDiffuseColorHash, material->GetDiffuseColor() );	
        }
    }
}