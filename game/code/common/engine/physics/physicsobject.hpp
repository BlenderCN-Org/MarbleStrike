#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/math/matrix44.hpp"
#include "tinyxml2.h"
#include "btBulletDynamicsCommon.h"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class RenderObject;

//////////////////////////////////////////////////////
// CONSTANT
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class PhysicsObject
{
public:

	PhysicsObject();
	~PhysicsObject();
		
	void Create( const RenderObject* renderObject, const Math::Matrix44 &transform, const Math::Matrix44 &localTransform );
	void CreateConstraint( const RenderObject* renderObject );

	btRigidBody* GetRigidBody() const
	{
		return mRigidBody;
	}

	btTypedConstraint* GetConstraint() const
	{
		return mConstraint;
	}
		
protected:		
		
	btRigidBody* mRigidBody;	
	btTypedConstraint* mConstraint;
	btTriangleIndexVertexArray* mTriangleIndexVertexArray;    
};

#endif