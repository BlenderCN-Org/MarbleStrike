//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/quaternion.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/physics/physicsdata.hpp"
#include "common/engine/physics/physicsconstraintdata.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include "common/engine/render/cubicbezierspline.hpp"
#include "common/engine/render/followpathconstraint.hpp"
#include "common/engine/render/curve.hpp"
#include "common/engine/render/cubicbezierspline.hpp"
#include "common/engine/game/gameapp.hpp"
#include "common/engine/messages/messagehandler.hpp"
#include <string>
#include <algorithm>

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

RenderObject::RenderObject() 
{
	System::StringCopy( mObjectName, 64, "" );
    System::StringCopy( mGameType, 64, "general" );
	mScriptFile[0] = '\0';
	mRenderObjectData = NULL;
	mPhysicsData = NULL;
	mPhysicsConstraintData = NULL;
	mFollowPathConstraint = NULL;
	mFrame = 0;
	mKeyFrame = 0;
	mStartFrame = 0;
	mEndFrame = 0;
	mProxyObject = NULL;	
	mIsVisible = true;
    mAnimationState = ANIMATION_STATE_STOPPED;
    mLoopAnimation = false;
    mStartAnimation = false;
    mResetAnimation = false;
    mParentObject = NULL;
	mOnStopAnimation = TRIGGER_TYPE_NONE;
    mInitStartFrame = 0;
    System::StringCopy( mParentObjectName, 64, "" );
}

//============================================================================

RenderObject::~RenderObject()
{	
	DELETE_PTR( mPhysicsData );
	DELETE_PTR( mPhysicsConstraintData );
	DELETE_PTR( mFollowPathConstraint );
}

//============================================================================

void RenderObject::Initialize()
{
    SetFrame( mInitStartFrame );
    StartAnimation();
    Update( 0 );
    StopAnimation();

    if ( mLoopAnimation )
    {
        LoopAnimation( true );
    }

    if ( mStartAnimation )
    {
        StartAnimation();
    }
}

//============================================================================

void RenderObject::Reset()
{
    if ( mResetAnimation )
    {
        SetFrame( mInitStartFrame );
        StartAnimation();
        Update( 0 );
        StopAnimation();
    }
}

//============================================================================

void RenderObject::Update( float elapsedTime )
{
    if ( mAnimationState == ANIMATION_STATE_PLAYING )
    {
        if ( mFollowPathConstraint )
        {
            RenderObject* targetObject = mFollowPathConstraint->GetTarget();
            Curve* curveObject = reinterpret_cast<Curve*>( targetObject->GetRenderObjectData() );            
            curveObject->Update( elapsedTime );

            if ( curveObject->GetAnimationState() == Curve::CURVE_ANIMATION_STATE_PLAYING )
            {
                CubicBezierSpline* spline = curveObject->GetSpline( 0 );
                float totalLength = spline->GetTotalLength();
                float evaluationTime = curveObject->GetEvaluationTime();
                float pathDuration = curveObject->GetPathDuration();
                float positionLength = totalLength * ( evaluationTime / pathDuration );
                float t = spline->GetPercentageFromLength( positionLength );
                if ( t >= 1.0f )
                {
                    t = 0.99f;
                }

                Math::Vector3 position = spline->Evaluate( t );

                mTransform.SetIdentity();
                mTransform.Translate( position );        
                mTransform = targetObject->GetTransform() * mTransform;
            }
            else
            {
                mAnimationState = ANIMATION_STATE_STOPPED;
            }
        }

        if ( mEndFrame > 0 )
        {
            UpdateAnimationTransform( mFrame, mKeyFrame );

            mFrame += 24.0f * elapsedTime;
            if ( mFrame > mAnimationPosition[0][mKeyFrame+1].mCenter[0] )
            {
                mKeyFrame++;
            }	
            if ( mFrame >= mEndFrame )
            {
				if ( mOnStopAnimation != TRIGGER_TYPE_NONE )
				{
					TriggerMessage* triggerMessage = NEW_PTR( "Message" ) TriggerMessage;
					triggerMessage->mTriggerType = mOnStopAnimation;
					triggerMessage->mData = (void*)mObjectName;
					GameApp::Get()->GetMessageHandler()->SendMsg( PORT_GAME, triggerMessage );
				}

                if ( mLoopAnimation )
                {
                    mFrame = 0;
                    mKeyFrame = 0;
                }
                else
                {
                    mAnimationState = ANIMATION_STATE_STOPPED;
                }
            }
        }
    }
}

//============================================================================

void RenderObject::SetObjectName( const char* name )
{
	System::StringCopy( mObjectName, 64, name );
}

//============================================================================

void RenderObject::SetGameType( const char* type )
{
	System::StringCopy( mGameType, 64, type );
}

//============================================================================

const char* RenderObject::GetGameType()
{
    if ( mProxyObject )
    {
        return mProxyObject->GetGameType();
    }
    else
    {
        return mGameType;
    }
}

//============================================================================

RenderObjectData* RenderObject::GetRenderObjectData() const
{
    if ( mProxyObject )
    {        
        return mProxyObject->GetRenderObjectData();
    }
    else
    {
        return mRenderObjectData;
    }
}

//============================================================================

Math::Matrix44 RenderObject::GetTransform() const
{
    Math::Matrix44 parentTransform;
    if ( mParentObject )
    {
        parentTransform = mParentObject->GetTransform();
    }    
    
    if ( mProxyObject )
    {        
        return parentTransform * mTransform * mProxyObject->GetTransform();
    }
    else
    {
        return parentTransform * mTransform;
    }
}

//============================================================================

PhysicsData* RenderObject::GetPhysicsData() const
{	
    if ( mProxyObject )
    {
        return mProxyObject->GetPhysicsData();
    }
    else
    {
        return mPhysicsData;
    }
}

//============================================================================

void RenderObject::Load( tinyxml2::XMLNode* node )
{
	tinyxml2::XMLElement* element = node->ToElement();

	const char* name = element->Attribute( "name" );	
	SetObjectName( name );	

    const char* gameType = element->Attribute( "game_type" );
    if ( gameType )
    {
        System::StringCopy( mGameType, 64, gameType );
    }

    const char* parentObject = element->Attribute( "parent" );
    if ( parentObject )
    {
        System::StringCopy( mParentObjectName, 64, parentObject );
    }

    const char* startAnimation = element->Attribute( "start_animation" );
    if ( startAnimation )
    {        
        if ( System::StringICmp( startAnimation, "true" ) == 0 )
        {
            mStartAnimation = true;
        }
        else if ( System::StringICmp( startAnimation, "false" ) == 0 )
        {
            mStartAnimation = false;
        }        
    }
    else
    {
        mStartAnimation = true;
    }

    const char* loopAnimation = element->Attribute( "loop_animation" );
    if ( loopAnimation )
    {        
        if ( System::StringICmp( loopAnimation, "true" ) == 0 )
        {
            mLoopAnimation = true;
        }
        else if ( System::StringICmp( loopAnimation, "false" ) == 0 )
        {
            mLoopAnimation = false;
        }        
    }
    else
    {
        mLoopAnimation = true;
    }

	const char* onStopAnimation = element->Attribute( "on_stop_animation" );
    if ( onStopAnimation )
    {        
        if ( System::StringICmp( onStopAnimation, "REMOVE_OBJECT" ) == 0 )
        {
            mOnStopAnimation = TRIGGER_TYPE_REMOVE_OBJECT;
        }        
    }

    double value = element->DoubleAttribute( "start_frame" );
    mInitStartFrame = static_cast<float>( value );

    const char* resetAnimation = element->Attribute( "reset_animation" );
    if ( resetAnimation )
    {        
        if ( System::StringICmp( resetAnimation, "true" ) == 0 )
        {
            mResetAnimation = true;
        }
        else if ( System::StringICmp( resetAnimation, "false" ) == 0 )
        {
            mResetAnimation = false;
        }        
    }
    else
    {
        mResetAnimation = false;
    }

	const char* scriptFile = element->Attribute( "script" );
	if ( scriptFile )
	{
		System::StringCopy( mScriptFile, 64, scriptFile );
	}

	const char* visible = element->Attribute( "visible" );
	if ( visible && strcmp( visible, "True" ) == 0 )
	{
		mIsVisible = true;
	}
	else
	{
		mIsVisible = false;
	}

	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();
			if ( strcmp( elementValue, "xform" ) == 0 )
			{
				LoadTransform( currentNode, mTransform );                
			}
			else if ( strcmp( elementValue, "xform_local" ) == 0 )
			{
				LoadTransform( currentNode, mLocalTransform );                
			}
			else if ( strcmp( elementValue, "boundingbox" ) == 0 )
			{
                std::string str = element->GetText();
                str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                const char* boundingBoxText = str.c_str();

				std::vector<std::string> tokens;
				tokens.reserve( 4 );
				System::Tokenize( tokens, const_cast<char*>( boundingBoxText ), " " );			
				mBoundingBox[0] = static_cast<float>( atof( tokens[0].c_str() ) );
				mBoundingBox[1] = static_cast<float>( atof( tokens[1].c_str() ) );
				mBoundingBox[2] = static_cast<float>( atof( tokens[2].c_str() ) );
			}
			else if ( strcmp( elementValue, "scale" ) == 0 )
			{
				double x = element->DoubleAttribute( "x" );
				double y = element->DoubleAttribute( "y" );
				double z = element->DoubleAttribute( "z" );

				mScale[0] = static_cast<float>( x );
				mScale[1] = static_cast<float>( y );
				mScale[2] = static_cast<float>( z );
			}
			else if ( strcmp( elementValue, "physics" ) == 0 )
			{
				mPhysicsData = NEW_PTR( "PhysicsData" ) PhysicsData;
				mPhysicsData->Load( currentNode );
			}
			else if ( strcmp( elementValue, "constraint" ) == 0 )
			{
				const char* type = element->Attribute( "type" );

				if ( strcmp( type, "RIGID_BODY_JOINT" ) == 0 )
				{
					mPhysicsConstraintData = NEW_PTR( "PhysicsConstraintData" ) PhysicsConstraintData;
					mPhysicsConstraintData->Load( currentNode );
				}
				else if ( strcmp( type, "FOLLOW_PATH" ) == 0 )
				{					
					mFollowPathConstraint = NEW_PTR( "mFollowPathConstraint" ) FollowPathConstraint;
					mFollowPathConstraint->Load( currentNode );
				}
			}
			else if ( strcmp( elementValue, "animation" ) == 0 )
			{			
				mStartFrame = element->IntAttribute( "start" );
				mEndFrame = element->IntAttribute( "end" );

				LoadAnimation( currentNode );

                UpdateAnimationTransform( 0, 0 );
			}		
		}
    }
}

//============================================================================

void RenderObject::LoadTransform( tinyxml2::XMLNode* node, Math::Matrix44 &transform )
{
    tinyxml2::XMLElement* element = node->ToElement();
    
    std::string str = element->GetText();
    str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
    str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

    const char* transformText = str.c_str();

    Assert( transformText != NULL, "" );

    std::vector<std::string> tokens;
    tokens.reserve( 16 );
    System::Tokenize( tokens, const_cast<char*>( transformText ), " " );			
    for ( int i = 0; i < 16; ++i )
    {
        transform[i] = static_cast<float>( atof( tokens[i].c_str() ) );
    }
}

//===========================================================================

void RenderObject::LoadAnimation( tinyxml2::XMLNode* node )
{
	for ( tinyxml2::XMLNode* currentNode = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
	{
        tinyxml2::XMLElement* element = currentNode->ToElement();
		if ( element )
		{	
			const char* elementValue = element->Value();

            if ( strcmp( elementValue, "curve" ) == 0 )
            {		
                const char* name = element->Attribute( "name" );
                if ( strcmp( name, "location" ) == 0 )
                {
                    int index = element->IntAttribute( "index" );

                    int numKeys = element->IntAttribute( "keys" );

                    std::string str = element->GetText();
                    str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                    str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

                    std::vector<std::string> tokens;
                    tokens.reserve( numKeys * 6 );
                    System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

                    int count = 0;
                    for ( int i = 0; i < numKeys; ++i )
                    {
						AnimationData newData;
						newData.mCenter[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mCenter[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mLeftHandle[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mLeftHandle[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mRightHandle[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mRightHandle[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						mAnimationPosition[index].push_back( newData );
					}
				}
				else if ( strcmp( name, "rotation_euler" )  == 0 )
				{
					int index = element->IntAttribute( "index" );

					int numKeys = element->IntAttribute( "keys" );
                    
                    std::string str = element->GetText();
                    str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                    str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

					std::vector<std::string> tokens;
					tokens.reserve( numKeys * 6 );
					System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

					int count = 0;
					for ( int i = 0; i < numKeys; ++i )
					{
						AnimationData newData;
						newData.mCenter[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mCenter[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mLeftHandle[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mLeftHandle[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mRightHandle[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mRightHandle[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						mAnimationRotation[index].push_back( newData );
					}
				}
				else if ( strcmp( name, "scale" )  == 0 )
				{
					int index = element->IntAttribute( "index" );

					int numKeys = element->IntAttribute( "keys" );
                    
                    std::string str = element->GetText();
                    str.erase( std::remove( str.begin(), str.end(), '\n' ), str.end() );
                    str.erase( std::remove( str.begin(), str.end(), '\t' ), str.end() );

					std::vector<std::string> tokens;
					tokens.reserve( numKeys * 6 );
					System::Tokenize( tokens, const_cast<char*>( str.c_str() ), " " );

					int count = 0;
					for ( int i = 0; i < numKeys; ++i )
					{
						AnimationData newData;
						newData.mCenter[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mCenter[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mLeftHandle[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mLeftHandle[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mRightHandle[0] = static_cast<float>( atof( tokens[count++].c_str() ) );
						newData.mRightHandle[1] = static_cast<float>( atof( tokens[count++].c_str() ) );
						mAnimationScale[index].push_back( newData );
					}
				}
			}
        }
    }
}

//===========================================================================

void RenderObject::StartAnimation()
{
    if ( mProxyObject )
    {
        mProxyObject->StartAnimation();
    }

    if ( mFollowPathConstraint )
    {
        RenderObject* targetObject = mFollowPathConstraint->GetTarget();
        Curve* curveObject = reinterpret_cast<Curve*>( targetObject->GetRenderObjectData() );
        curveObject->StartAnimation();
    }

    mAnimationState = ANIMATION_STATE_PLAYING;
}

//===========================================================================

void RenderObject::StopAnimation()
{
    if ( mProxyObject )
    {
        mProxyObject->StopAnimation();
    }

    if ( mFollowPathConstraint )
    {
        RenderObject* targetObject = mFollowPathConstraint->GetTarget();
        Curve* curveObject = reinterpret_cast<Curve*>( targetObject->GetRenderObjectData() );
        curveObject->StopAnimation();
    }

    mAnimationState = ANIMATION_STATE_STOPPED;
}

//===========================================================================

void RenderObject::SetFrame( float frame )
{
    if ( frame >= mStartFrame && frame <= mEndFrame )
    {    
        if ( mProxyObject )
        {
            mProxyObject->SetFrame( frame );
        }

        if ( mFollowPathConstraint )
        {
            RenderObject* targetObject = mFollowPathConstraint->GetTarget();
            Curve* curveObject = reinterpret_cast<Curve*>( targetObject->GetRenderObjectData() );
            curveObject->SetFrame( frame );
        }

        mFrame = frame;
        mKeyFrame = FindStartingKeyFrame( frame );
    }
    else
    {
        Assert( false, "" );
    }
}

//===========================================================================

void RenderObject::LoopAnimation( bool value )
{
    if ( mProxyObject )
    {
        mProxyObject->LoopAnimation( value );
    }

    if ( mFollowPathConstraint )
    {
        RenderObject* targetObject = mFollowPathConstraint->GetTarget();
        Curve* curveObject = reinterpret_cast<Curve*>( targetObject->GetRenderObjectData() );
        curveObject->SetLoopAnimation( value );
    }

    mLoopAnimation = value;
}

//===========================================================================

float RenderObject::Solve2DCubicBezier( float xTarget, Math::Vector2 p0, Math::Vector2 p1, Math::Vector2 p2, Math::Vector2 p3 ) 
{
	float xTolerance = 0.0001f;

	float lower = 0.0f;
	float upper = 1.0f;
	float percent = (upper + lower) / 2.0f;

	float xResult = CubicBezierSpline::CubicBezierEvaluate( percent, p0[0], p1[0], p2[0], p3[0] );
    
	while ( fabs( xTarget - xResult ) > xTolerance && percent < 1.0f ) 
	{
		if ( xTarget > xResult ) 
		{
			lower = percent;
		}
		else 
		{
			upper = percent;
		}

		percent = (upper + lower) / 2.0f;
		xResult = CubicBezierSpline::CubicBezierEvaluate( percent, p0[0], p1[0], p2[0], p3[0] );
	}

	float yResult = CubicBezierSpline::CubicBezierEvaluate( percent, p0[1], p1[1], p2[1], p3[1] );
    return yResult;
};

//===========================================================================

float RenderObject::AnimationDataInterpolate( float frame, AnimationData &data0, AnimationData &data1 )
{	
    float finalValue = Solve2DCubicBezier( frame, data0.mCenter, data0.mRightHandle, data1.mLeftHandle, data1.mCenter );
    return finalValue;
}

//===========================================================================

void RenderObject::UpdateAnimationTransform( float frame, int keyFrame )
{
    float xvalue = AnimationDataInterpolate( frame, mAnimationPosition[0][keyFrame], mAnimationPosition[0][keyFrame+1] );
    float yvalue = AnimationDataInterpolate( frame, mAnimationPosition[1][keyFrame], mAnimationPosition[1][keyFrame+1] );
    float zvalue = AnimationDataInterpolate( frame, mAnimationPosition[2][keyFrame], mAnimationPosition[2][keyFrame+1] );
    float xrvalue = AnimationDataInterpolate( frame, mAnimationRotation[0][keyFrame], mAnimationRotation[0][keyFrame+1] );
    float yrvalue = AnimationDataInterpolate( frame, mAnimationRotation[1][keyFrame], mAnimationRotation[1][keyFrame+1] );
    float zrvalue = AnimationDataInterpolate( frame, mAnimationRotation[2][keyFrame], mAnimationRotation[2][keyFrame+1] );
	float xsvalue = AnimationDataInterpolate( frame, mAnimationScale[0][keyFrame], mAnimationScale[0][keyFrame+1] );
    float ysvalue = AnimationDataInterpolate( frame, mAnimationScale[1][keyFrame], mAnimationScale[1][keyFrame+1] );
    float zsvalue = AnimationDataInterpolate( frame, mAnimationScale[2][keyFrame], mAnimationScale[2][keyFrame+1] );

    mTransform.SetIdentity();    
    mTransform.Translate( xvalue, yvalue, zvalue );				
    mTransform.Rotate( Math::RadiansToDegree( yrvalue ), 0, 1, 0 );
    mTransform.Rotate( Math::RadiansToDegree( zrvalue ), 0, 0, 1 );		
    mTransform.Rotate( Math::RadiansToDegree( xrvalue ), 1, 0, 0 );
	mScale.Set( xsvalue, ysvalue, -zsvalue );
}

//===========================================================================

int RenderObject::FindStartingKeyFrame( float frame )
{
    int foundKeyFrame = 0;

    size_t numAnimationPositions = mAnimationPosition[0].size();
    for ( int i = 0; i < numAnimationPositions; ++i )
    {
        float currentFrame = mAnimationPosition[0][i].mCenter[0];
        if ( frame <= currentFrame )
        {
            break;
        }
        else
        {
            foundKeyFrame++;
        }
    }

    if ( foundKeyFrame > 0 )
    {
        foundKeyFrame--;
    }

    return foundKeyFrame;
}
