#ifndef BOWLER_RUN_STATE_HPP
#define BOWLER_RUN_STATE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "btBulletDynamicsCommon.h"
#include "common/engine/math/frustum.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/vector2.hpp"
#include "common/game/bowlergame/bowlerdatabase.hpp"
#include "common/game/bowlergame/states/bowlerstate.hpp"
#include <list>
#include <map>
#include <stack>

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//#define SHOW_DRAW_CALLS

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class BlenderAsset;
class BoundingVolume;
class BowlerDatabase;
class BowlerInput;
class BowlerObject;
class BowlerRunState;
class BowlerSkyDome;
class btTriangleIndexVertexArray;
class Camera;
class CameraVolume;
class Effect;
class LevelScript;
class Light;
class ParticleSystem;
class PhysicsObject;
class PostFX;
class RenderTarget;
class Resource;

namespace System
{
    class Timer;
}

namespace Audio
{
	class AudioManager;
}

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

enum LevelGameState
{
    LEVEL_GAME_STATE_LOADING,
    LEVEL_GAME_STATE_RUNNING
};

enum FinishedState
{
    FINISHED_STATE_NONE,
    FINISHED_STATE_START_TIMER,
    FINISHED_STATE_COMPLETE,    
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

struct RayCallBack : public btCollisionWorld::AllHitsRayResultCallback
{
    RayCallBack( const btVector3 &rayFromWorld, const btVector3 &rayToWorld )
        :	AllHitsRayResultCallback( rayFromWorld, rayToWorld )
    {
    }

    bool needsCollision( btBroadphaseProxy* /*proxy0*/ ) const
    {
        return true;
    }
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class BowlerRunState : public BowlerState
{
public:

    void OnMessageCallback( void* userData );

    BowlerRunState( BowlerInput* bowlerInput, PhysicsManager* physicsManager, Audio::AudioManager* audioManager, BowlerDatabase* bowlerDatabase );
    ~BowlerRunState();

    void Initialize();
    void Shutdown();
    void InitializeLevel();
    void Update( float elapsedTime );
    void RenderUpdate();

    void RemoveCollectibleObject( BowlerObject* object );

    void SetLevel( int level )
    {
        mCurrentLevel = level;
    }

    int GetNumCollectibles()
    {
        return (int)mCollectibleObjects.size();
    }

    BowlerObject* GetCollectibleObject( int i )
    {
        return mCollectibleObjects[i];
    }

    int GetNumGoalRings()
    {
        return (int)mGoalRingObjects.size();
    }
        
    BowlerObject* GetGoalRingObject( int i )
    {
        return mGoalRingObjects[i];
    }

    int GetNumTriggers()
    {
        return (int)mTriggerObjects.size();
    }

    BowlerObject* GetTriggerObject( int i )
    {
        return mTriggerObjects[i];
    }

    void AddToRemoveObject( BowlerObject* object )
    {
        mToRemove.push_back( object );
    }
    
private:

	void SelectResolutionVariables();
	void CreateLevelScript();
    void InitializeCameras();
    void InitializeBoundingVolume();
	void InitializeObjects();
    void Restart();
    void ResetCameras();
    void UpdateWorld( float elapsedTime );
    void UpdateCameras(  float elapsedTime );
    void UpdateInput( float elapsedTime );
	CameraVolume* GetCurrentCameraVolume();
    void SeparateOpaqueAlpha();	
    void RenderShadow();
    void RenderMain();	
    void RenderText();
    void RenderPhysicsObjectsShadow();
    void RenderPhysicsShadowObject( 
        BowlerObject* bowlerObject,
        const Math::Matrix44 &projectionMatrix,
        const Math::Matrix44 &viewMatrix        
        );
    void RenderPhysicsObjectsOpaque(
        const Math::Matrix44 &cameraMatrix,
        const Math::Matrix44 &projectionMatrix,
        const Math::Matrix44 &viewMatrix,
        const Math::Matrix44 &projectionLightMatrix,
        const Math::Matrix44 &viewLightMatrix,
        const Math::Frustum &frustum
        );	
    void RenderPhysicsObjectsAlpha(
        const Math::Matrix44 &cameraMatrix,
        const Math::Matrix44 &projectionMatrix,
        const Math::Matrix44 &viewMatrix,
        const Math::Matrix44 &projectionLightMatrix,
        const Math::Matrix44 &viewLightMatrix,
        const Math::Frustum &frustum
        );
    void RenderPhysicsObject( 
        BowlerObject* bowlerObject,
        const Math::Matrix44 &cameraMatrix,
        const Math::Matrix44 &projectionMatrix,
        const Math::Matrix44 &viewMatrix,
        const Math::Matrix44 &projectionLightMatrix,
        const Math::Matrix44 &viewLightMatrix,
        const Math::Frustum &frustum
        );	
    void RenderDebugDraw( 
        const Math::Matrix44 &projectionMatrix,
        const Math::Matrix44 &viewMatrix		
        );	
    void DrawMap( int x, int y, int width, int height, int techniqueIndex, Texture* texture );
    void DrawControlUI();
    void UpdateCollectibles();
    void UpdateGoal();
    void UpdateBallFalloff();
    Math::Matrix44 GetLightProjection();
    Math::Matrix44 GetLightView();
	Math::Matrix44 GetCurrentPhysicsObjectTransform( PhysicsObject* physicsObject );
	void SetupGameMode();

    bool mPause;
    int mCurrentLevel;

    Resource* mBlenderAssetResource;

    Effect* mShadowEffect;
    int mShadowTechnique;
    RenderTarget* mShadowRT;

    RenderTarget* mMainRT;
    PostFX* mPostFX;

    Texture* mEmptyTexture;
    Texture* mToonTexture;

    BlenderAsset* mBlenderAsset;

    Math::Matrix44 mRenderCameraTransform;
	Math::Matrix44 mOriginalCameraTransform;
	Math::Matrix44 mCurrentCameraTransform;
	Math::Matrix44 mTargetCameraTransform;	
	Math::Vector3 mCurrentCameraPosition;
	Math::Vector3 mTargetCameraPosition;
	    
	Math::Matrix44 mOriginalTrackToCameraTransform;
    Math::Matrix44 mTrackToCameraTransform;
    Math::Vector3 mTargetTrackToCameraPosition;    

    Camera* mCamera;
    std::vector<RenderObject*> mCameraVolumeRenderObject;
	    
    RenderObject* mLight;	
    Math::Matrix44 mLightStartTransform;

    BowlerSkyDome* mBowlerSkyDome;
        
    Math::Vector3 mBallStartPosition;
    Math::Vector3 mBallPrevPosition;
    BowlerObject* mBallObject;	

    std::vector<BowlerObject*> mRenderObjects;
    std::vector<BowlerObject*> mGoalRingObjects;    
    std::vector<BowlerObject*> mCollectibleObjects;	
    std::vector<BowlerObject*> mTriggerObjects;
    std::vector<BowlerObject*> mToRemove;    
    std::vector<BowlerObject*> mOpaqueRenderObjects;
    std::vector<BowlerObject*> mAlphaRenderObjects;
    std::vector<BowlerObject*> mObstructionObjects;	
    std::stack<BowlerObject*> mUserCollectibles;	
	std::vector<BowlerObject*> mRemovedObjectsToRestore;

    std::vector<btTypedConstraint*> mPhysicsConstraints;    
    std::vector<RenderObject*> mProxyObjects;

    Math::Vector3 mCurrentAngleOffset;
    Math::Vector3 mOriginalAngleOffset;
    Math::Vector3 mDesiredAngleOffset;
    Math::Vector3 mCurrentPositionOffset;
    Math::Vector3 mOriginalPositionOffset;
    Math::Vector3 mDesiredPositionOffset;
    
    int mItemsCollected;
    int mGoalRingsRemaining;

    System::Timer* mTimer;    
    float mTotalElapsedTime;

    DatabaseDataUser mCurrentData;

    bool mIsControlOn;
    int mControlStickID;
    int mJumpControlID;
    int mJumpControlIDPC;
    Math::Vector2 mOldControlPosition;
    Math::Vector2 mCurrentControlPosition;
    Math::Vector2 mControlSize;
    Math::Vector2 mJumpControlPosition;
    Math::Vector2 mJumpControlSize;

    Texture* mControlCircleTexture;
    Texture* mControlThumbTexture;
    Texture* mControlJumpTexture;

    Math::Vector3 mOriginalMaxBB;
    Math::Vector3 mOriginalMinBB;
    Math::Vector3 mMaxBB;
    Math::Vector3 mMinBB;

    int mCameraPositionHash;
    int mDiffuseColorHash;
    int mSpecularColorHash;
    int mLightColorHash;
    int mLightDirectionHash;
    int mObjectToProjectionMatrixHash;    
    int mObjectToWorldMatrixHash;
    int mObjectToProjectionLightMatrixHash;
	int mInverseTransposeObjectToWorldMatrixHash;
    int mToonTextureHash;
    int mColorTextureHash;
    int mShadowTextureHash;

    System::Timer* mFinishedTimer;
    FinishedState mFinishedState;
    float mTotalFinishedTime;
    float mTimeAttackTime;

    LevelGameState mLevelGameState;

    BoundingVolume* mBoundingVolume;

	ParticleSystem* mParticleSystem;	
	Math::Matrix44 mParticleSystemTransform;
	ParticleSystem* mParticleSystemGoalSplash;
	Math::Matrix44 mParticleSystemGoalSplashTransform;

    bool mIsShutdown;

	LevelScript* mLevelScript;

	BowlerObject* mTouchingObject;
	Math::Matrix44 mCurrentTransform;
	int mObjectID;

#if defined ( SHOW_DRAW_CALLS )
    int mNumDrawCalls;
#endif

    float mJoystickControlPositionLeft;
    float mJoystickControlPositionRight;
    float mJumpControlPositionLeft;
    float mJumpControlPositionRight;
    float mJoystickSize;
    float mJoystickControlPositionHeight;
	float mCircleGraphicSize;
	float mThumbGraphicSize;
	float mJumpControlPositionHeight;
	float mJumpGrahpicSize;
	float mJumpControlWidth;

	std::string mJoyStickCircleTextureFilename;
	std::string mJoyStickThumbTextureFilename;
	std::string mJumpIconTextureFilename;
};

#endif