#ifndef PHYSICS_CONSTRAINT_DATA_HPP
#define PHYSICS_CONSTRAINT_DATA_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/vector3.hpp"
#include "tinyxml2.h"
#include "btBulletDynamicsCommon.h"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

enum ConstraintPivotType
{
	CONSTRAINT_PIVOT_HINGE,
	CONSTRAINT_PIVOT_6_DOF
};

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class PhysicsConstraintData
{
public:

	PhysicsConstraintData();
	~PhysicsConstraintData();
		
	void Load( tinyxml2::XMLNode* node );

	ConstraintPivotType GetConstraintPivotType()
	{
		return mConstraintPivotType;
	}

	Math::Vector3 GetPivotAxis()
	{
		return mPivotAxis;
	}

	Math::Vector3 GetPivotPosition()
	{
		return mPivotPosition;
	}

	Math::Vector3 GetLimitAngleMax()
	{
		return mLimitAngleMax;
	}

	Math::Vector3 GetLimitAngleMin()
	{
		return mLimitAngleMin;
	}
			
private:		
	
	ConstraintPivotType mConstraintPivotType;
	Math::Vector3 mPivotAxis;
	Math::Vector3 mPivotPosition;
	Math::Vector3 mLimitAngleMax;
	Math::Vector3 mLimitAngleMin;
};

#endif