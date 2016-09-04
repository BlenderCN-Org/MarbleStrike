//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/physics/physicsobject.hpp"
#include "common/engine/physics/physicsdata.hpp"
#include "common/engine/physics/physicsconstraintdata.hpp"
#include "common/engine/render/renderobject.hpp"
#include "common/engine/render/renderobjectdata.hpp"
#include "common/engine/render/mesh.hpp"
#include "common/engine/system/memory.hpp"
#include "common/engine/system/assert.hpp"
#include "BulletCollision/Gimpact/btGImpactShape.h"

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

PhysicsObject::PhysicsObject() 
{	
	mRigidBody = NULL;
	mTriangleIndexVertexArray = NULL;
}

//============================================================================

PhysicsObject::~PhysicsObject()
{	
	DELETE_PTR( mTriangleIndexVertexArray );

	if ( mRigidBody != NULL )
	{
		btCompoundShape* compoundShape = reinterpret_cast<btCompoundShape*>( mRigidBody->getCollisionShape() );
		int numShapes = compoundShape->getNumChildShapes();
		for ( int i = 0; i < numShapes; ++i )
		{
			btCollisionShape* shape = compoundShape->getChildShape( i );
			DELETE_PTR( shape );
		}
		DELETE_PTR( compoundShape );

		btMotionState* motionState = mRigidBody->getMotionState();
		DELETE_PTR( motionState );

		DELETE_PTR( mRigidBody );
	}	
}

//============================================================================

void PhysicsObject::Create( const RenderObject* renderObject, const Math::Matrix44 &transform, const Math::Matrix44 &localTransform )
{
	PhysicsData* physicsData = renderObject->GetPhysicsData();
	PhysicsType physicsType = physicsData->GetPhysicsType();
	BoundsType boundsType = physicsData->GetBoundsType();
	float mass = physicsData->GetMass();
	
	if ( physicsType != PHYSICS_NO_COLLISION )
	{
		btCompoundShape* compoundShape = new btCompoundShape;

		btTransform startTransform;
		startTransform.setIdentity();		
		startTransform.setFromOpenGLMatrix( &( transform[0] ) );	

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		Math::Vector3 boundingBox = renderObject->GetBoundingBox();
		boundingBox /= 2.0;

		btCollisionShape* collisionShape = NULL;
		switch ( boundsType )
		{
		case BOUNDS_BOX:
			collisionShape = new btBoxShape( btVector3( boundingBox[0], boundingBox[1], boundingBox[2] ) );
			break;
		case BOUNDS_SPHERE:
			collisionShape = new btSphereShape( physicsData->GetBoundingRadius() );			
			break;
		case BOUNDS_CYLINDER:
			collisionShape = new btCylinderShape( btVector3( boundingBox[0], boundingBox[1], boundingBox[2] ) );
			break;
		case BOUNDS_CONVEX_HULL:
			{
				Assert( renderObject->GetRenderObjectData() &&
					renderObject->GetRenderObjectData()->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_MESH,
					"" );

				Mesh* mesh = reinterpret_cast<Mesh*>( renderObject->GetRenderObjectData() );
				btConvexHullShape* convexHullShape = new btConvexHullShape( mesh->GetPositions(), mesh->GetNumVertices(), sizeof( float ) * 3 );				
				collisionShape = convexHullShape;
			}
            break;
		case BOUNDS_MESH:
			{
				Assert( renderObject->GetRenderObjectData() &&
					renderObject->GetRenderObjectData()->GetRenderObjectDataType() == RENDER_OBJECT_DATA_TYPE_MESH,
					"" );

				Mesh* mesh = reinterpret_cast<Mesh*>( renderObject->GetRenderObjectData() );
				btTriangleIndexVertexArray* triIndexVertexArray = new btTriangleIndexVertexArray(
					mesh->GetNumPrimitives(),
					(int*)mesh->GetFaceIndices(),
					mesh->GetFaceIndicesStride() * sizeof( unsigned int ),
					mesh->GetNumVertices(),
					mesh->GetPositions(),
					mesh->GetPositionsStride() * sizeof( float )
					);
				mTriangleIndexVertexArray = triIndexVertexArray;

				bool useQuantizedAabbCompression = true;
				if ( physicsType == PHYSICS_STATIC )
				{
					collisionShape = new btBvhTriangleMeshShape( triIndexVertexArray, useQuantizedAabbCompression );
				}
				else
				{
					btGImpactMeshShape* newMesh = new btGImpactMeshShape( triIndexVertexArray );
					newMesh->setMargin(0.0f);
					newMesh->updateBound();
					collisionShape = newMesh;
				}
			}
            break;

		default:
			Assert( false, "" );
			break;
		}

		btVector3 localInertia(0,0,0);
		if ( mass > 0 )
		{
			collisionShape->calculateLocalInertia( mass, localInertia );
		}

		btTransform localXForm;
		localXForm.setIdentity();		
        if ( physicsData->GetIsCompound() )
		{
            localXForm.setFromOpenGLMatrix( &( localTransform[0] ) );
        }
		compoundShape->addChildShape( localXForm, collisionShape );

		btRigidBody::btRigidBodyConstructionInfo rbInfo( mass, myMotionState, compoundShape, localInertia );
		mRigidBody = new btRigidBody( rbInfo );		
	}
}

//============================================================================

void PhysicsObject::CreateConstraint( const RenderObject* renderObject )
{
	PhysicsConstraintData* data = renderObject->GetPhysicsConstraintData();
	if ( data )
	{
		Math::Vector3 pivotAxis = data->GetPivotAxis();
		Math::Vector3 pivotPosition = data->GetPivotPosition();

		ConstraintPivotType type = data->GetConstraintPivotType();
		switch ( type )
		{
		case CONSTRAINT_PIVOT_HINGE:
			{				
				Assert( mRigidBody != NULL, "" );

				Math::Vector3 minAngleLimit = data->GetLimitAngleMin();
				Math::Vector3 maxAngleLimit = data->GetLimitAngleMax();
				const btVector3 btPivotA( pivotPosition[0], pivotPosition[1], pivotPosition[2] );                
				btVector3 btAxisA( 1.0f, 0.0f, 0.0f );				
				btHingeConstraint* hingeConstraint = new btHingeConstraint( *mRigidBody, btPivotA, btAxisA );
				hingeConstraint->setLimit( minAngleLimit[0], maxAngleLimit[0] );
				mConstraint = hingeConstraint;
				
			}
			break;
		case CONSTRAINT_PIVOT_6_DOF:
			break;
		default:
			Assert( false, "" );
			break;
		}
	}
}