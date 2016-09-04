#ifndef CURVE_HPP
#define CURVE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector2.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/system/fixedarray.hpp"
#include "common/engine/render/renderobjectdata.hpp"
#include "tinyxml2.h"
#include <vector>

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class CubicBezierSpline;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Curve : public RenderObjectData
{
public:
    enum CurveAnimationState
    {    
        CURVE_ANIMATION_STATE_PLAYING,    
        CURVE_ANIMATION_STATE_STOPPED
    };

public:

    Curve();
    ~Curve();

    void Update( float elapsedTime );

    void SetName( const char* name );
    const char* GetName()
    {
        return mName;
    }
    void Load( tinyxml2::XMLNode* node );

    void SetLoopAnimation( bool value );
    void StartAnimation();
    void StopAnimation();
    void SetFrame( float frame  );

    int GetNumSplines()
    {
        return mSplines.GetSize();
    }

    CubicBezierSpline* GetSpline( int index )
    {
        return mSplines[index];
    }

    float GetEvaluationTime()
    {
        return mCurrentEvalTime;
    }

    void SetPathDuration( float duration )
    {
        mPathDuration = duration;
    }

    float GetPathDuration()
    {
        return mPathDuration;
    }

    CurveAnimationState GetAnimationState()
    {
        return mCurveAnimationState;
    }

public:

    struct AnimationData
    {	
        Math::Vector2 mCenter;
        Math::Vector2 mLeftHandle;
        Math::Vector2 mRightHandle;
    };

    void LoadCurve( tinyxml2::XMLNode* node );
    void LoadAnimation( tinyxml2::XMLNode* node );

    float Solve2DCubicBezier( 
        float xTarget, 
        const Math::Vector2 &p0, 
        const Math::Vector2 &p1, 
        const Math::Vector2 &p2, 
        const Math::Vector2 &p3
        );
    float AnimationDataInterpolate( float frame, AnimationData &data0, AnimationData &data1 );
    int FindStartingKeyFrame( float frame );

    char mName[64];
    FixedArray<CubicBezierSpline*> mSplines;
    std::vector<AnimationData> mEvaulationData;
    float mPathDuration;
    float mCurrentEvalTime;

    float mFrame;
    int mKeyFrame;	
    int mStartFrame;
    int mEndFrame;
    bool mLoopAnimation;
    CurveAnimationState mCurveAnimationState;
};

#endif