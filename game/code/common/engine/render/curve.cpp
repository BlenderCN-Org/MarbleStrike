//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/cubicbezierspline.hpp"
#include "common/engine/render/curve.hpp"
#include "common/engine/math/quaternion.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/utilities.hpp"
#include <string>
#include <algorithm>

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Curve::Curve() 
    : RenderObjectData( RENDER_OBJECT_DATA_TYPE_CURVE )
{    
    mPathDuration = 0;
    mCurrentEvalTime = 0;
    mFrame = 0;
    mKeyFrame = 0;
    mStartFrame = 0;
    mEndFrame = 0;
    mLoopAnimation = false;
    mCurveAnimationState = CURVE_ANIMATION_STATE_STOPPED;
}

//============================================================================

Curve::~Curve()
{
    for ( int i = 0; i < mSplines.GetSize(); ++i )
    {
        DELETE_PTR( mSplines[i] );
    }
}

//============================================================================

void Curve::Update( float elapsedTime )
{
    if ( mCurveAnimationState == CURVE_ANIMATION_STATE_PLAYING )
    {
        if ( mEndFrame > 0 )
        {
            float xvalue = AnimationDataInterpolate( mFrame, mEvaulationData[mKeyFrame], mEvaulationData[mKeyFrame+1] );

            mFrame += 24.0f * elapsedTime;
            if ( mFrame > mEvaulationData[mKeyFrame+1].mCenter[0] )
            {
                mKeyFrame++;
            }	
            if ( mFrame >= mEndFrame )
            {
                if ( mLoopAnimation )
                {
                    mFrame = 0;
                    mKeyFrame = 0;
                }
                else
                {
                    mCurveAnimationState = CURVE_ANIMATION_STATE_STOPPED;
                }
            }

            mCurrentEvalTime = xvalue;
        }
    }
}

//============================================================================

void Curve::SetLoopAnimation( bool value )
{
    mLoopAnimation = value;
}

//============================================================================

void Curve::StartAnimation()
{
    mCurveAnimationState = CURVE_ANIMATION_STATE_PLAYING;
}

//============================================================================

void Curve::StopAnimation()
{
    mCurveAnimationState = CURVE_ANIMATION_STATE_STOPPED;
}

//============================================================================

void Curve::SetFrame( float frame  )
{
    if ( frame >= mStartFrame && frame <= mEndFrame )
    {    
        mFrame = frame;
        mKeyFrame = FindStartingKeyFrame( frame );
    }
    else
    {
        Assert( false, "" );
    }
}

//============================================================================

void Curve::SetName( const char* name )
{
    System::StringCopy( mName, 64, name );
}

//============================================================================

void Curve::Load( tinyxml2::XMLNode* node )
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
                SetName( name );

                int numSplines = element->IntAttribute( "numsplines" );
                mSplines.SetSize( numSplines );		

                double value = element->DoubleAttribute( "path_duration" );
                mPathDuration = static_cast<float>( value );

                LoadCurve( currentNode );
            }
        }
    }
}

//============================================================================

void Curve::LoadCurve( tinyxml2::XMLNode* node )
{
    for ( tinyxml2::XMLNode* currentNode  = node->FirstChild(); currentNode != NULL; currentNode = currentNode->NextSibling() )
    {
        tinyxml2::XMLElement* element = currentNode->ToElement();
        if ( element )
        {            
            const char* elementValue = element->Value();

            if ( strcmp( elementValue, "spline" ) == 0 )
            {
                CubicBezierSpline* spline = NEW_PTR( "Spline" ) CubicBezierSpline;                
                spline->Load( currentNode );
                mSplines.PushBack( spline );
            }
            else if ( strcmp( elementValue, "animation" ) == 0 )
            {
                mStartFrame = element->IntAttribute( "start" );
                mEndFrame = element->IntAttribute( "end" );

                LoadAnimation( currentNode );
            }
        }
    }
}


//===========================================================================

void Curve::LoadAnimation( tinyxml2::XMLNode* node )
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
                if ( strcmp( name, "eval_time" ) == 0 )
                {
                    int numKeys = element->IntAttribute( "keys" );

                    const char* text = element->GetText();

                    std::string str = text;
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
                        mEvaulationData.push_back( newData );
                    }
                }				
            }
        }
    }
}

//===========================================================================

float Curve::Solve2DCubicBezier( 
    float xTarget, 
    const Math::Vector2 &p0, 
    const Math::Vector2 &p1, 
    const Math::Vector2 &p2, 
    const Math::Vector2 &p3 ) 
{
    float xTolerance = 0.01f;

    float lower = 0.0f;
    float upper = 1.0f;
    float percent = (upper + lower) / 2.0f;

    float xResult = CubicBezierSpline::CubicBezierEvaluate( percent, p0[0], p1[0], p2[0], p3[0] );

    if ( xTarget > 0 )
    {
        while ( fabs( xTarget - xResult ) > xTolerance ) 
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
    }
    else
    {
        percent = 0.0f;
    }

    float yResult = CubicBezierSpline::CubicBezierEvaluate( percent, p0[1], p1[1], p2[1], p3[1] );
    return yResult;
};

//===========================================================================

float Curve::AnimationDataInterpolate( float frame, AnimationData &data0, AnimationData &data1 )
{	
    float finalValue = Solve2DCubicBezier( frame, data0.mCenter, data0.mRightHandle, data1.mLeftHandle, data1.mCenter );
    return finalValue;
}
//===========================================================================

int Curve::FindStartingKeyFrame( float frame )
{
    int foundKeyFrame = 0;

    size_t numAnimationPositions = mEvaulationData.size();
    for ( int i = 0; i < numAnimationPositions; ++i )
    {
        float currentFrame = mEvaulationData[i].mCenter[0];
        if ( frame <= currentFrame )
        {
            break;
        }
        else
        {
            foundKeyFrame++;
        }
    }

    return foundKeyFrame;
}