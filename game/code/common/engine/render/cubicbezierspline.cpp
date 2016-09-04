//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/cubicbezierspline.hpp"
#include "common/engine/math/quaternion.hpp"
#include "common/engine/system/stringutilities.hpp"
#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// GLOBALS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

float CubicBezierSpline::CubicBezierEvaluate( float t, float p0, float p1, float p2, float p3 )
{
	float result = 
		p0 * ( 1.0f - t ) * ( 1.0f - t ) * ( 1.0f - t ) + 
		p1 * 3.0f * ( 1.0f - t ) * ( 1.0f - t ) * t + 
		p2 * 3.0f * ( 1.0f - t ) * t * t + 
		p3 * t * t * t;
	return result;
}

//============================================================================

CubicBezierSpline::CubicBezierSpline() 
{    
	mTotalLength = 0;
    mIsCyclic = false;
}

//============================================================================

CubicBezierSpline::~CubicBezierSpline()
{
	for ( int i = 0; i < mBezierPoints.GetSize(); ++i )
	{
		DELETE_PTR( mBezierPoints[i] );
	}
}

//============================================================================

void CubicBezierSpline::SetNumPoints( int size )
{
	mBezierPoints.SetSize( size );
}

//============================================================================

Math::Vector3 CubicBezierSpline::Evaluate( float t )
{
	int numPoints = mBezierPoints.GetSize() - 1;
    //if ( mIsCyclic )
    //{
    //    numPoints += 1;
    //}

	float lengthT = t * numPoints;
	int index0 = static_cast<int>( floorf( lengthT ) );
	int index1 = index0 + 1;
    //if ( mIsCyclic && index1 == mBezierPoints.GetSize() )
    //{
    //    index1 = 0;
    //}

	BezierPoint* point0 = mBezierPoints[index0];
	BezierPoint* point1 = mBezierPoints[index1];

	Math::Vector3 p0 = point0->mPosition;
	Math::Vector3 p1 = point0->mHandleRight;
	Math::Vector3 p2 = point1->mHandleLeft;
	Math::Vector3 p3 = point1->mPosition;

	float tt = lengthT - floor( lengthT );

	//Math::Vector3 finalValue = 
	//	p0 * ( 1.0f - tt ) * ( 1.0f - tt ) * ( 1.0f - tt ) + 
	//	p1 * 3.0f * ( 1.0f - tt ) * ( 1.0f - tt ) * tt + 
	//	p2 * 3.0f * ( 1.0f - tt ) * tt * tt + 
	//	p3 * tt * tt * tt;

	float xResult = CubicBezierEvaluate( tt, p0[0], p1[0], p2[0], p3[0] );
	float yResult = CubicBezierEvaluate( tt, p0[1], p1[1], p2[1], p3[1] );
	float zResult = CubicBezierEvaluate( tt, p0[2], p1[2], p2[2], p3[2] );

	Math::Vector3 finalValue( xResult, yResult, zResult );
	return finalValue;
}

//============================================================================

void CubicBezierSpline::Load( tinyxml2::XMLNode* node )
{
    tinyxml2::XMLElement* splineElement = node->ToElement();
    
    int numPoints = splineElement->IntAttribute( "numpoints" );

    const char* cyclic = splineElement->Attribute( "use_cyclic" );
    if ( cyclic )
    {
        mIsCyclic = false;
        if ( strcmp( cyclic, "True" ) == 0 )
        {
            mIsCyclic = true;
        }
    }

    if ( mIsCyclic )
    {
        numPoints++;
    }

    SetNumPoints( numPoints );

    for ( tinyxml2::XMLNode* pointNode = node->FirstChild(); pointNode != NULL; pointNode = pointNode->NextSibling() )
	{
        tinyxml2::XMLElement* pointElement = pointNode->ToElement();
		if ( pointElement )
		{
			BezierPoint* splinePoint = NEW_PTR( "BezierPoint" ) BezierPoint;
			mBezierPoints.PushBack( splinePoint );

			const char* pointValue = pointElement->Value();
			if ( strcmp( pointValue, "point" ) == 0 )
			{
				const char* position = pointElement->Attribute( "position" );
				const char* handle_left = pointElement->Attribute( "handle_left" );
				const char* handle_right = pointElement->Attribute( "handle_right" );

				std::vector<std::string> tokens;
				tokens.reserve( 3 );
				System::Tokenize( tokens, const_cast<char*>( position ), " " );

				splinePoint->mPosition[0] = static_cast<float>( atof( tokens[0].c_str() ) );
				splinePoint->mPosition[1] = static_cast<float>( atof( tokens[1].c_str() ) );
				splinePoint->mPosition[2] = static_cast<float>( atof( tokens[2].c_str() ) );

				tokens.clear();
				System::Tokenize( tokens, const_cast<char*>( handle_left ), " " );
				splinePoint->mHandleLeft[0] = static_cast<float>( atof( tokens[0].c_str() ) );
				splinePoint->mHandleLeft[1] = static_cast<float>( atof( tokens[1].c_str() ) );
				splinePoint->mHandleLeft[2] = static_cast<float>( atof( tokens[2].c_str() ) );	

				tokens.clear();
				System::Tokenize( tokens, const_cast<char*>( handle_right ), " " );
				splinePoint->mHandleRight[0] = static_cast<float>( atof( tokens[0].c_str() ) );
				splinePoint->mHandleRight[1] = static_cast<float>( atof( tokens[1].c_str() ) );
				splinePoint->mHandleRight[2] = static_cast<float>( atof( tokens[2].c_str() ) );	
			}
		}
	}

    if ( mIsCyclic )
    {
        BezierPoint* splinePoint = NEW_PTR( "BezierPoint" ) BezierPoint;
        *splinePoint = *mBezierPoints[mBezierPoints.GetSize() - 1];
        mBezierPoints.PushBack( splinePoint );
        for ( int i = numPoints - 2; i > 0; --i )
        {
            *mBezierPoints[i] = *mBezierPoints[i-1];
        }
        *mBezierPoints[0] = *splinePoint;
    }
}

//============================================================================

float CubicBezierSpline::GetTotalLength()
{
	if ( mTotalLength != 0 )
	{
		return mTotalLength;
	}
	else
	{
		PositionTableData data;
		data.mT = 0;
		data.mLength = 0;
		mPositionTable.push_back( data );

		float n = 5000.0f;
		float steps = 1.0f / n;
		float t = 0;
		Math::Vector3 current = Evaluate( 0 );
		t += steps;

		mTotalLength = 0;

		while ( t < ( 1.0f - steps ) )
		{
			float nextT = t + steps;

			Math::Vector3 next = Evaluate( nextT );
			Math::Vector3 v = next - current;
			float distance = Math::Vector3::Magnitude( v );

			current = next;

			mTotalLength += distance;
			
			data.mT = t;
			data.mLength = mTotalLength;
			mPositionTable.push_back( data );
			
			t += steps;
		}

		return mTotalLength;
	}	
}

//============================================================================

float CubicBezierSpline::GetPercentageFromLength( float length )
{
	size_t num = mPositionTable.size();
	PositionTableData prevData = mPositionTable[0];
	for ( int i = 0; i < num; ++i )
	{
		PositionTableData currentData = mPositionTable[i];
		if ( currentData.mLength > length )
		{			
			float t = ( length - prevData.mLength ) / ( currentData.mLength - prevData.mLength );
			float finalT = ( currentData.mT - prevData.mT ) * t + prevData.mT;
			return finalT;
		}
		else
		{
			prevData = currentData;
		}
	}

	return 1.0f;
}