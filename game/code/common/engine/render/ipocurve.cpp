//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/ipocurve.hpp"

#include "common/engine/system/utilities.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

IpoCurve::IpoCurve()
:	mCurrentFrame( 0 )
,	mKeyFrameIndex( 0 )
,	mNextKeyFrameIndex( 1 )
,	mValue( 0 )
{
}

//===========================================================================

IpoCurve::~IpoCurve()
{
}

//===========================================================================

void IpoCurve::Update( float /*elapsedTime*/ )
{
	if ( mBezierTriples.GetSize() > 1 )
	{
		BezierTriple point0;
		BezierTriple point1;

		mCurrentFrame++;

		if ( mCurrentFrame >= mBezierTriples[mKeyFrameIndex].mPoint[1][0] )
		{
			float t = 
				( mCurrentFrame - mBezierTriples[mKeyFrameIndex].mPoint[1][0] ) / 
				( mBezierTriples[mNextKeyFrameIndex].mPoint[1][0] - mBezierTriples[mKeyFrameIndex].mPoint[1][0] );

			point0 = mBezierTriples[mKeyFrameIndex];
			point1 = mBezierTriples[mNextKeyFrameIndex];
	
			if ( mCurrentFrame >= mBezierTriples[mNextKeyFrameIndex].mPoint[1][0] )
			{
				mKeyFrameIndex++;
				mNextKeyFrameIndex++;
			}

			//mValue = point1.mPoint[1][1] * t + point0.mPoint[1][1] * ( 1 - t );

			float p0 = point0.mPoint[1][1];
			float p1 = point0.mPoint[2][1];
			float p2 = point1.mPoint[0][1];
			float p3 = point1.mPoint[1][1];
			float c = 3 * ( p1 - p0 ); 
			float b = 3 * ( p2 - p1 ) - c;
			float a = p3 - p0 - c - b;
			mValue = ( a * t * t * t ) + ( b * t * t ) + ( c * t ) + p0; 

			if ( mNextKeyFrameIndex >= mBezierTriples.GetSize() )
			{
				mCurrentFrame = 0;
				mKeyFrameIndex = 0;
				mNextKeyFrameIndex = 1;
			}
		}
	}
}