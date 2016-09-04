#ifndef PHYSICSMANAGER_HPP
#define PHYSICSMANAGER_HPP

//////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

//////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////

class Camera;

//////////////////////////////////////////////////////
// CONSTANTS
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// CLASSES
//////////////////////////////////////////////////////

class PhysicsManager
{
public:
	
	PhysicsManager();
	~PhysicsManager();

	void Initialize();
	void Shutdown();	
	void Update( float elapsedTime );

	void AddRigidBody( btRigidBody* body );
	void RemoveRigidBody( btRigidBody* body );

	void AddConstraint( btTypedConstraint* constraint );
	void RemoveConstraint( btTypedConstraint* constraint );

	btCollisionWorld* GetCollisionWorld()
	{
		return mDynamicsWorld;
	}

	void TurnOnSimulation()
	{
		mRunning = true;
	}

	void TurnOffSimulation()
	{
		mRunning = false;
	}

private:

	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher* mDispatcher;
	btBroadphaseInterface* mOverlappingPairCache;
	btConstraintSolver*	mSolver;
	btDynamicsWorld* mDynamicsWorld;
	bool mRunning;
};

#endif
