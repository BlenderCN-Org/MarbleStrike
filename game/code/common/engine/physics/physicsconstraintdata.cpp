//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/physics/physicsconstraintdata.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

PhysicsConstraintData::PhysicsConstraintData() 
{
	mConstraintPivotType = CONSTRAINT_PIVOT_HINGE;
}

//============================================================================

PhysicsConstraintData::~PhysicsConstraintData()
{
}

//============================================================================

void PhysicsConstraintData::Load( tinyxml2::XMLNode* node )
{
	tinyxml2::XMLElement* element = node->ToElement();
	
	double value = 0;

    value = element->DoubleAttribute( "axis_x" );
	mPivotAxis[0] = static_cast<float>( value );
    value = element->DoubleAttribute( "axis_y" );
	mPivotAxis[1] = static_cast<float>( value );
    value = element->DoubleAttribute( "axis_z" );
	mPivotAxis[2] = static_cast<float>( value );

    value = element->DoubleAttribute( "pivot_x" );
	mPivotPosition[0] = static_cast<float>( value );
    value = element->DoubleAttribute( "pivot_y" );
	mPivotPosition[1] = static_cast<float>( value );
    value = element->DoubleAttribute( "pivot_z" );
	mPivotPosition[2] = static_cast<float>( value );

	const char* pivotType = element->Attribute( "pivot" );
	if ( strcmp( pivotType, "HINGE" ) == 0 )
	{
		mConstraintPivotType = CONSTRAINT_PIVOT_HINGE;
				
        value = element->DoubleAttribute( "limit_angle_max_x" );
		mLimitAngleMax[0] = static_cast<float>( value );		

        value = element->DoubleAttribute( "limit_angle_min_x" );
		mLimitAngleMin[0] = static_cast<float>( value );
	}
	else if ( strcmp( pivotType, "GENERIC_6_DOF" ) == 0 )
	{
		mConstraintPivotType = CONSTRAINT_PIVOT_6_DOF;

        value = element->DoubleAttribute( "limit_angle_max_x" );
		mLimitAngleMax[0] = static_cast<float>( value );
        value = element->DoubleAttribute( "limit_angle_max_y" );
		mLimitAngleMax[1] = static_cast<float>( value );
        value = element->DoubleAttribute( "limit_angle_max_z" );
		mLimitAngleMax[2] = static_cast<float>( value );

        value = element->DoubleAttribute( "limit_angle_min_x" );
		mLimitAngleMin[0] = static_cast<float>( value );
        value = element->DoubleAttribute( "limit_angle_min_y" );
		mLimitAngleMin[1] = static_cast<float>( value );
        value = element->DoubleAttribute( "limit_angle_min_z" );
		mLimitAngleMin[2] = static_cast<float>( value );
	}
}