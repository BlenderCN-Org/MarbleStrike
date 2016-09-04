//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "common/engine/database/database.hpp"
#include "common/engine/math/matrix44.hpp"
#include "common/engine/physics/physicsmanager.hpp"
#include "common/engine/render/camera.hpp"
#include "common/engine/render/renderer.hpp"
#include "common/engine/system/memory.hpp"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"

//////////////////////////////////////////////////////
// DEFINES
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASS METHODS
//////////////////////////////////////////////////////

PhysicsManager::PhysicsManager()
:	mCollisionConfiguration( NULL )
,	mDispatcher( NULL )
,	mOverlappingPairCache( NULL )
,	mSolver( NULL )
,	mDynamicsWorld( NULL )
,	mRunning( true )
{
}

//===========================================================================

PhysicsManager::~PhysicsManager()
{
}

//===========================================================================

void PhysicsManager::Initialize()
{
	mCollisionConfiguration = NEW_PTR( "Bullet Collision Config" ) btDefaultCollisionConfiguration();
	mDispatcher = NEW_PTR( "Bullet Collision Dispatcher" ) btCollisionDispatcher( mCollisionConfiguration );

	btGImpactCollisionAlgorithm::registerAlgorithm(mDispatcher);

	btVector3 worldAabbMin( -10000, -10000, -10000 );
	btVector3 worldAabbMax( 10000, 10000, 10000 );
	mOverlappingPairCache = NEW_PTR( "Pair Cache" ) btDbvtBroadphase;
	mSolver = new btSequentialImpulseConstraintSolver;
	mDynamicsWorld = new btDiscreteDynamicsWorld( mDispatcher, mOverlappingPairCache, mSolver, mCollisionConfiguration );	
}

//===========================================================================

void PhysicsManager::Shutdown()
{
	DELETE_PTR( mDynamicsWorld );
	DELETE_PTR( mSolver );
	DELETE_PTR( mOverlappingPairCache );
	DELETE_PTR( mDispatcher );
	DELETE_PTR( mCollisionConfiguration );
}

//===========================================================================

void PhysicsManager::Update( float elapsedTime )
{		
	if ( mRunning )
	{
		mDynamicsWorld->setGravity( btVector3( 0, -90.0f, 0 ) );
		mDynamicsWorld->stepSimulation( elapsedTime, 7 );
	}
}

//===========================================================================

void PhysicsManager::AddRigidBody( btRigidBody* body )
{
	mDynamicsWorld->addRigidBody( body );
}

//===========================================================================

void PhysicsManager::RemoveRigidBody( btRigidBody* body )
{
	mDynamicsWorld->removeRigidBody( body );
}

//===========================================================================

void PhysicsManager::AddConstraint( btTypedConstraint* constraint )
{
	mDynamicsWorld->addConstraint( constraint );	
}

//===========================================================================

void PhysicsManager::RemoveConstraint( btTypedConstraint* constraint )
{
	mDynamicsWorld->removeConstraint( constraint );	
}