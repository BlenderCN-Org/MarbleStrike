#ifndef PHYSICS_DATA_HPP
#define PHYSICS_DATA_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "tinyxml2.h"
#include "btBulletDynamicsCommon.h"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderObject;

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

enum BoundsType
{
	BOUNDS_BOX,
	BOUNDS_SPHERE,
	BOUNDS_CYLINDER,
	BOUNDS_CONVEX_HULL,
	BOUNDS_MESH	
};

enum PhysicsType
{
	PHYSICS_RIGID_BODY,	
	PHYSICS_STATIC,
	PHYSICS_DYNAMIC,
	PHYSICS_NO_COLLISION,
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class PhysicsData
{
public:

	PhysicsData();
	~PhysicsData();
		
	void Load( tinyxml2::XMLNode* node );

	float GetBoundingRadius() const
	{
		return mBoundingRadius;
	}

	BoundsType GetBoundsType() const
	{
		return mBoundsType;
	}

	PhysicsType GetPhysicsType() const
	{
		return mPhysicsType;
	}

	float GetMass() const
	{
		return mMass;
	}

    bool GetIsCompound() const
    {
        return mIsCompound;
    }
	
protected:		
	
	float mBoundingRadius;
	float mMass;	
    bool mIsCompound;

	BoundsType mBoundsType;
	PhysicsType mPhysicsType;
};

#endif