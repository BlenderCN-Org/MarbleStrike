
LOCAL_PATH:= $(call my-dir)/../../..

include $(CLEAR_VARS)

LOCAL_MODULE := bullet_android

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/sdk/bullet-2.81-rev2613/src \

LOCAL_CFLAGS    := -Werror
LOCAL_CFLAGS += -std=gnu++11

LOCAL_SRC_FILES := \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btAxisSweep3.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btBroadphaseProxy.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btDbvt.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btDbvtBroadphase.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btDispatcher.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btMultiSapBroadphase.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btOverlappingPairCache.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btQuantizedBvh.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/BroadphaseCollision/btSimpleBroadphase.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btActivatingCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btBoxBoxCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btBoxBoxDetector.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btCollisionDispatcher.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btCollisionObject.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btCollisionWorld.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btCompoundCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btConvexConcaveCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btConvexConvexAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btConvexPlaneCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btGhostObject.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btInternalEdgeUtility.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btManifoldResult.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btSimulationIslandManager.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btSphereBoxCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/btUnionFind.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionDispatch/SphereTriangleDetector.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btBox2dShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btBoxShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btBvhTriangleMeshShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btCapsuleShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btCollisionShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btCompoundShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConcaveShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConeShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConvex2dShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConvexHullShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConvexInternalShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConvexPointCloudShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConvexPolyhedron.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConvexShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btConvexTriangleMeshShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btCylinderShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btEmptyShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btHeightfieldTerrainShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btMinkowskiSumShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btMultimaterialTriangleMeshShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btMultiSphereShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btOptimizedBvh.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btPolyhedralConvexShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btShapeHull.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btSphereShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btStaticPlaneShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btStridingMeshInterface.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btTetrahedronShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btTriangleBuffer.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btTriangleCallback.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btTriangleIndexVertexArray.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btTriangleIndexVertexMaterialArray.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btTriangleMesh.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btTriangleMeshShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/CollisionShapes/btUniformScalingShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/btContactProcessing.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/btGenericPoolAllocator.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/btGImpactBvh.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/btGImpactQuantizedBvh.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/btGImpactShape.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/btTriangleShapeEx.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/gim_box_set.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/gim_contact.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/gim_memory.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/Gimpact/gim_tri_collision.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btContinuousConvexCollision.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btConvexCast.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btGjkConvexCast.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btGjkEpa2.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btGjkEpaPenetrationDepthSolver.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btGjkPairDetector.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btPersistentManifold.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btPolyhedralContactClipping.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btRaycastCallback.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btSubSimplexConvexCast.cpp \
	sdk/bullet-2.81-rev2613/src/BulletCollision/NarrowPhaseCollision/btVoronoiSimplexSolver.cpp	 \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/Character/btKinematicCharacterController.cpp	 \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btConeTwistConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btContactConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btGearConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btGeneric6DofSpringConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btHinge2Constraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btHingeConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btPoint2PointConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btSliderConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btTypedConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/ConstraintSolver/btUniversalConstraint.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/Dynamics/btDiscreteDynamicsWorld.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/Dynamics/btRigidBody.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/Dynamics/btSimpleDynamicsWorld.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/Dynamics/Bullet-C-API.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/Vehicle/btRaycastVehicle.cpp \
	sdk/bullet-2.81-rev2613/src/BulletDynamics/Vehicle/btWheelInfo.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btAlignedAllocator.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btConvexHull.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btConvexHullComputer.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btGeometryUtil.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btPolarDecomposition.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btQuickprof.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btSerializer.cpp \
	sdk/bullet-2.81-rev2613/src/LinearMath/btVector3.cpp \
	
include $(BUILD_STATIC_LIBRARY)