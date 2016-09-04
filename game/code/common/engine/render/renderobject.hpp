#ifndef RENDER_OBJECT_HPP
#define RENDER_OBJECT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "common/engine/math/vector2.hpp"
#include "common/engine/render/trigger.hpp"
#include "tinyxml2.h"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderObjectData;
class PhysicsData;
class PhysicsConstraintData;
class FollowPathConstraint;

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class RenderObject
{
public:
    enum RenderObjectAnimationState
    {    
        ANIMATION_STATE_PLAYING,    
        ANIMATION_STATE_STOPPED
    };

public:

    RenderObject();
    ~RenderObject();

    void Initialize();
    void Reset();
    void Update( float elapsedTime );	
    void Load( tinyxml2::XMLNode* node );
    void LoadTransform( tinyxml2::XMLNode* node, Math::Matrix44 &transform );
    void LoadAnimation( tinyxml2::XMLNode* node );

    void SetObjectName( const char* name );
    const char* GetObjectName()
    {
        return mObjectName;
    }

    void SetGameType( const char* type );
    const char* GetGameType();

    RenderObjectData* GetRenderObjectData() const;

    void SetRenderObjectData( RenderObjectData* data )
    {
        mRenderObjectData = data;
    }

    Math::Matrix44 GetTransform() const;

    void SetTransform( const Math::Matrix44& mat )
    {		
        mTransform = mat;
    }	

    Math::Matrix44 GetLocalTransform() const
    {	
        return mLocalTransform;
    }

    void SetLocalTransform( const Math::Matrix44& mat )
    {		
        mLocalTransform = mat;
    }	

    Math::Vector3 GetBoundingBox() const
    {
        return mBoundingBox;
    }

    void SetProxyObject( RenderObject* object )
    {
        mProxyObject = object;
    }

    RenderObject* GetProxyObject()
    {
        return mProxyObject;
    }

    PhysicsData* GetPhysicsData() const;

    PhysicsConstraintData* GetPhysicsConstraintData() const
    {	
        return mPhysicsConstraintData;
    }

    bool GetIsVisible()
    {
        return mIsVisible;
    }

    void SetIsVisible( bool value )
    {
        mIsVisible = value;
    }

    FollowPathConstraint* GetFollowPathConstraint()
    {
        return mFollowPathConstraint;
    }

    void StartAnimation();
    void StopAnimation();
    void SetFrame( float frame );
    void LoopAnimation( bool value );

    const char* GetParentObjectName()
    {
        return mParentObjectName;
    }

    void SetParentObject( RenderObject* parent )
    {
        mParentObject = parent;
    }

	const char* GetScriptFile()
	{
		return mScriptFile;
	}

	Math::Vector3 GetScale()
	{
		return mScale;
	}

protected:

    struct AnimationData
    {	
        Math::Vector2 mCenter;
        Math::Vector2 mLeftHandle;
        Math::Vector2 mRightHandle;
    };

    float Solve2DCubicBezier( float xTarget, Math::Vector2 p0, Math::Vector2 p1, Math::Vector2 p2, Math::Vector2 p3 );
    float AnimationDataInterpolate( float frame, AnimationData &data0, AnimationData &data1 );
    void UpdateAnimationTransform( float frame, int keyFrame );
    int FindStartingKeyFrame( float frame );

    char mObjectName[64];
    char mGameType[64];
	char mScriptFile[64];
    Math::Matrix44 mTransform;
    Math::Matrix44 mLocalTransform;    
	Math::Vector3 mScale;

    RenderObjectData* mRenderObjectData;
    PhysicsData* mPhysicsData;
    PhysicsConstraintData* mPhysicsConstraintData;
    FollowPathConstraint* mFollowPathConstraint;

    Math::Vector3 mBoundingBox;

    std::vector<AnimationData> mAnimationPosition[3];
    std::vector<AnimationData> mAnimationRotation[3];
	std::vector<AnimationData> mAnimationScale[3];

    float mFrame;
    int mKeyFrame;	
    int mStartFrame;
    int mEndFrame;

    bool mIsVisible;

    RenderObject* mProxyObject;

    char mParentObjectName[64];
    RenderObject* mParentObject;

    bool mLoopAnimation;
    bool mStartAnimation;
    bool mResetAnimation;
    float mInitStartFrame;
    RenderObjectAnimationState mAnimationState;	
	TriggerType mOnStopAnimation;
};

#endif