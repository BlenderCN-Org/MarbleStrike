//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/octreenode.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/math/mathfunctions.hpp"
#include "common/engine/math/intersections.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

OctreeNode::OctreeNode() 
:	mIsLeaf( false )
{    
	for( int i = 0; i < 2; i++ ) 
	{
		for ( int j = 0; j < 2; j++ ) 
		{
			for ( int k = 0; k < 2; k++ ) 
			{
				mChildren[i][j][k] = NULL;	
			}
		}
	}
}

//============================================================================

OctreeNode::~OctreeNode()
{
	for ( int i = 0; i < 2; i++ )
	{ 
		for ( int j = 0; j < 2; j++ ) 
		{
			for ( int k=0; k < 2; k++ ) 
			{
				if ( mChildren[i][j][k] != NULL )
				{
					DELETE_PTR( mChildren[i][j][k] );
				}
			}
		}
	}
}

//============================================================================

OctreeNode* OctreeNode::GetChild( int i, int j, int k ) const
{
	return mChildren[i][j][k];
}

//============================================================================

void OctreeNode::InsertTriangleToLeaf( const Math::Triangle &triangle, const float minLength )
{
	if( 
		( mMax[0] - mMin[0] ) <= minLength ||
		( mMax[1] - mMin[1] ) <= minLength ||
		( mMax[2] - mMin[2] ) <= minLength 
		)
	{
		mTriangleList.PushBack( triangle );				
		mIsLeaf = true;

		return;
	}

	Math::Vector3 avg = ( mMax - mMin ) * 0.5f;
	Math::Vector3 curMin, curMax;

	for ( int k = 0; k < 2; k++ ) 
	{
		for ( int j = 0; j < 2; j++ ) 
		{
			for ( int i = 0; i < 2; i++ ) 
			{										
				curMin[0] = mMin[0] + i*avg[0];
				curMin[1] = mMin[1] + j*avg[1];
				curMin[2] = mMin[2] + k*avg[2];
				curMax[0] = mMin[0] + avg[0] + i*avg[0];
				curMax[1] = mMin[1] + avg[1] + j*avg[1];
				curMax[2] = mMin[2] + avg[2] + k*avg[2];

				Math::Vector3 sides;
				sides = curMax - curMin;
				sides /= 2.0f;

				Math::Vector3 position;
				position = curMin + sides;

				if ( Math::IntersectStatic( Math::AABB( sides, position ), triangle ) ) 
				{
					if ( mChildren[i][j][k] == NULL ) 
					{
						mChildren[i][j][k] = NEW_PTR( "Octree Node") OctreeNode;
						mChildren[i][j][k]->mMin = curMin;
						mChildren[i][j][k]->mMax = curMax;
					}
					mChildren[i][j][k]->InsertTriangleToLeaf( triangle, minLength );
				}
			}
		}
	}
}

//============================================================================

void OctreeNode::InsertPointToLeaf( const PointData &point, const float minLength )
{
	if( 
		( mMax[0] - mMin[0] ) <= minLength ||
		( mMax[1] - mMin[1] ) <= minLength ||
		( mMax[2] - mMin[2] ) <= minLength 
		)
	{
		mPointList.PushBack( point );				
		mIsLeaf = true;

		return;
	}

	Math::Vector3 avg = ( mMax - mMin ) * 0.5f;
	Math::Vector3 curMin;
	Math::Vector3 curMax;
	
	float kfloat = 0;
	for ( int k = 0; k < 2; k++ ) 
	{
		float jfloat = 0;
		float kAvg = kfloat * avg[2];
		for ( int j = 0; j < 2; j++ ) 
		{
			float ifloat = 0;
			float jAvg = jfloat * avg[1];
			for ( int i = 0; i < 2; i++ ) 
			{						
				float iAvg = ifloat * avg[0];

				curMin[0] = mMin[0] + iAvg;
				curMin[1] = mMin[1] + jAvg;
				curMin[2] = mMin[2] + kAvg;
				curMax[0] = mMin[0] + avg[0] + iAvg;
				curMax[1] = mMin[1] + avg[1] + jAvg;
				curMax[2] = mMin[2] + avg[2] + kAvg;
				
				if ( 
					point.mPosition[0] >= curMin[0] && 
					point.mPosition[0] <= curMax[0] &&
					point.mPosition[1] >= curMin[1] && 
					point.mPosition[1] <= curMax[1] &&
					point.mPosition[2] >= curMin[2] && 
					point.mPosition[2] <= curMax[2] 
				)
				{
					if ( mChildren[i][j][k] == NULL ) 
					{
						mChildren[i][j][k] = NEW_PTR( "Octree Node") OctreeNode;
						mChildren[i][j][k]->mMin = curMin;
						mChildren[i][j][k]->mMax = curMax;
					}
					mChildren[i][j][k]->InsertPointToLeaf( point, minLength );
				}
				ifloat++;
			}
			jfloat++;
		}
		kfloat++;
	}
}

//============================================================================

void OctreeNode::ComputeDistanceToClosestTriangle( const Math::Vector3 &point, float &distance )
{
	if ( mIsLeaf == true )
	{
		int numTriangles = mTriangleList.GetSize();
		for ( int i = 0; i < numTriangles; ++i )
		{
			float currentDistance = Math::PointToTriangleDistance( mTriangleList[i], point );
			if ( currentDistance < distance )
			{
				distance = currentDistance;
			}
		}

		return;
	}

	Math::Vector3 avg = ( mMax - mMin ) * 0.5f;
	Math::Vector3 curMin, curMax;

	for ( int k = 0; k < 2; k++ ) 
	{
		for ( int j = 0; j < 2; j++ ) 
		{
			for ( int i = 0; i < 2; i++ ) 
			{			
				if ( mChildren[i][j][k] != NULL )
				{
					curMin[0] = mMin[0] + i*avg[0];
					curMin[1] = mMin[1] + j*avg[1];
					curMin[2] = mMin[2] + k*avg[2];
					curMax[0] = mMin[0] + avg[0] + i*avg[0];
					curMax[1] = mMin[1] + avg[1] + j*avg[1];
					curMax[2] = mMin[2] + avg[2] + k*avg[2];

					Math::Vector3 sides;
					sides = curMax - curMin;
					sides /= 2.0f;

					Math::Vector3 position;
					position = curMin + sides;

					if ( Math::IntersectStatic( Math::AABB( sides, position ), point ) ) 
					{						
						mChildren[i][j][k]->ComputeDistanceToClosestTriangle( point, distance );
					}
				}
			}
		}
	}
}