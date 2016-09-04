#ifndef OCTREE_NODE_HPP
#define OCTREE_NODE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/array.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/triangle.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// FORWARD DECLARATION
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// STRUCTURES
//////////////////////////////////////////////////////

struct PointData
{
	Math::Vector3 mPosition;
	Math::Vector3 mNormal;
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class OctreeNode
{
public:

	OctreeNode();
	~OctreeNode();

	OctreeNode* GetChild( int i, int j, int k ) const;

	void InsertTriangleToLeaf( const Math::Triangle &triangle, const float minLength );
	void InsertPointToLeaf( const PointData &point, const float minLength );
	void InsertPoint( const PointData &point, const float minLength );
	void ComputeDistanceToClosestTriangle( const Math::Vector3 &point, float &distance );

	int GetNumPoints()
	{
		return mPointList.GetSize();
	}

	PointData GetPoint( int index )
	{
		return mPointList[index];
	}

	void SetBoxSize( const Math::Vector3& max, const Math::Vector3& min )
	{
		mMax = max;
		mMin = min;
	}

	Math::Vector3 GetMax() const
	{ 
		return mMax; 
	}

	Math::Vector3 GetMin() const
	{ 
		return mMin; 
	}

	bool IsLeaf() const
	{ 
		return mIsLeaf; 
	}

private:

	Array<Math::Triangle> mTriangleList;
	Array<PointData> mPointList;
	Math::Vector3 mMin;
	Math::Vector3 mMax;    
	OctreeNode* mChildren[2][2][2];

	bool mIsLeaf;
};

#endif