//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/physics/physicsdata.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/system/assert.hpp"
#include "common/engine/system/memory.hpp"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

PhysicsData::PhysicsData() 
{	
	mBoundingRadius = 0;	
	mBoundsType = BOUNDS_BOX;
	mMass = 1.0;
    mIsCompound = false;
}

//============================================================================

PhysicsData::~PhysicsData()
{
}

//============================================================================

void PhysicsData::Load( tinyxml2::XMLNode* node )
{
	tinyxml2::XMLElement* element = node->ToElement();

	double value = element->DoubleAttribute( "radius" );
	mBoundingRadius = static_cast<float>( value );

	value = element->DoubleAttribute( "mass" );
	mMass = static_cast<float>( value );

    const char* isCompound = element->Attribute( "is_compound" );
    mIsCompound = false;
    if ( isCompound && strcmp( isCompound, "True" ) == 0 )
	{
        mIsCompound = true;
    }

	const char* boundsType = element->Attribute( "boundstype" );
	if ( strcmp( boundsType, "BOX" ) == 0 )
	{
		mBoundsType = BOUNDS_BOX;
	}
	else if ( strcmp( boundsType, "SPHERE" ) == 0 )
	{
		mBoundsType = BOUNDS_SPHERE;
	}
	else if ( strcmp ( boundsType, "CYLINDER" ) == 0 )
	{
		mBoundsType = BOUNDS_CYLINDER;
	}
    else if ( strcmp ( boundsType, "CONVEX_HULL" ) == 0 )
    {
        mBoundsType = BOUNDS_CONVEX_HULL;
    }
	else if ( strcmp( boundsType, "TRIANGLE_MESH" ) == 0 )
	{
		mBoundsType = BOUNDS_MESH;
	}

	const char* physicsType = element->Attribute( "physicstype" );
	if ( strcmp( physicsType, "STATIC" ) == 0 )
	{
		mMass = 0;
		mPhysicsType = PHYSICS_STATIC;
	}
	else if ( strcmp( physicsType, "DYNAMIC" ) == 0 )
	{
		mPhysicsType = PHYSICS_DYNAMIC;
	}
	else if ( strcmp( physicsType, "RIGID_BODY" ) == 0 )
	{
		mPhysicsType = PHYSICS_RIGID_BODY;
	}
	else if ( strcmp( physicsType, "NO_COLLISION" ) == 0 )
	{
		mPhysicsType = PHYSICS_NO_COLLISION;
	}	
}