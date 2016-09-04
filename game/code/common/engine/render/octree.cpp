//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/render/octree.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/math/mathfunctions.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

Octree::Octree()
{
	mRoot = NULL;
	mMinLength = 1.0f;
}

//============================================================================

Octree::~Octree()
{
	DELETE_PTR( mRoot );
}

//============================================================================

void Octree::InsertTriangleToLeaf( const Math::Triangle &triangle )
{
	Math::Vector3 sides;
	sides = mMax - mMin;
	sides /= 2.0f;

	Math::Vector3 position;
	position = mMin + sides;
	if ( Math::IntersectStatic( Math::AABB( sides, position ), triangle ) == true )
	{	
		if ( mRoot == NULL )
		{
			mRoot = NEW_PTR( "Root Node" ) OctreeNode;
		}

		mRoot->SetBoxSize( mMax, mMin );
		mRoot->InsertTriangleToLeaf( triangle, mMinLength );
	}
}

//============================================================================

void Octree::InsertPointToLeaf( const PointData &point )
{
	if ( 
		point.mPosition[0] >= mMin[0] && 
		point.mPosition[0] <= mMax[0] &&
		point.mPosition[1] >= mMin[1] && 
		point.mPosition[1] <= mMax[1] &&
		point.mPosition[2] >= mMin[2] && 
		point.mPosition[2] <= mMax[2] 
	)
	{	
		if ( mRoot == NULL )
		{
			mRoot = NEW_PTR( "Root Node" ) OctreeNode;
			mRoot->SetBoxSize( mMax, mMin );
		}
		
		mRoot->InsertPointToLeaf( point, mMinLength );
	}
}

//============================================================================

void Octree::ComputeDistanceToClosestTriangle( const Math::Vector3 &point, float &distance )
{
	if ( mRoot != NULL )
	{
		mRoot->ComputeDistanceToClosestTriangle( point, distance );		
	}
}

//============================================================================

void Octree::Set( float min, float max, float minLength )
{
	mMin.Set( min, min, min );
	mMax.Set( max, max, max );
	mMinLength = minLength;	
}