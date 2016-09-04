#ifndef OCTREE_HPP
#define OCTREE_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/system/array.hpp"
#include "common/engine/math/vector3.hpp"
#include "common/engine/math/intersections.hpp"
#include "common/engine/render/octreenode.hpp"
#include <stdio.h>

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class Octree
{
public:    
	
	Octree();
	~Octree();
	
	void InsertTriangleToLeaf( const Math::Triangle &triangle );
	void InsertPointToLeaf( const PointData &point );	
	void ComputeDistanceToClosestTriangle( const Math::Vector3 &point, float &distance );
	void Set( float min, float max, float minLength );

	Math::Vector3 GetMin() const 
	{ 
		return mMin; 
	}

	Math::Vector3 GetMax() const 
	{
		return mMax; 
	} 

	OctreeNode* GetRootNode() const 
	{ 
		return mRoot; 
	}

private:

	OctreeNode* mRoot;
	float mMinLength;
	Math::Vector3 mMin;
	Math::Vector3 mMax;
};

#endif