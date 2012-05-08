/**
 * Includes
 */
#include <GL/glfw.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <LinearMath/btAlignedObjectArray.h>
#include <btBulletDynamicsCommon.h>
#include <math.h>

#include "lib.h"

#define PI 3.14
using namespace std;

/**
 * Code
 */
class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;


class World {
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

	btBroadphaseInterface*	m_broadphase;

	btCollisionDispatcher*	m_dispatcher;

	btConstraintSolver*	m_solver;

	enum
	{
		USE_CCD=1,
		USE_NO_CCD
	};
	int 	m_ccdMode;

	btDefaultCollisionConfiguration* m_collisionConfiguration;

	public:

	World();

	virtual ~World() {
		exitPhysics();
	}

	void initPhysics() {
		// setTexturing(true);
		// setShadows(true);

		m_ShootBoxInitialSpeed = 4000.f;


		///collision configuration contains default setup for memory, collision setup
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		//	m_collisionConfiguration->setConvexConvexMultipointIterations();

		///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
		m_dispatcher->registerCollisionCreateFunc(BOX_SHAPE_PROXYTYPE,BOX_SHAPE_PROXYTYPE,m_collisionConfiguration->getCollisionAlgorithmCreateFunc(CONVEX_SHAPE_PROXYTYPE,CONVEX_SHAPE_PROXYTYPE));

		m_broadphase = new btDbvtBroadphase();

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
		m_solver = sol;

		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
		m_dynamicsWorld ->setDebugDrawer(&sDebugDrawer);
		m_dynamicsWorld->getSolverInfo().m_splitImpulse=true;
		m_dynamicsWorld->getSolverInfo().m_numIterations = 20;


		if (m_ccdMode==USE_CCD)
		{
			m_dynamicsWorld->getDispatchInfo().m_useContinuous=true;
		} else
		{
			m_dynamicsWorld->getDispatchInfo().m_useContinuous=false;
		}

		m_dynamicsWorld->setGravity(btVector3(0,-10,0));

		///create a few basic rigid bodies
		btBoxShape* box = new btBoxShape(btVector3(btScalar(110.),btScalar(1.),btScalar(110.)));
		//	box->initializePolyhedralFeatures();
		btCollisionShape* groundShape = box;

		//	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),50);

		m_collisionShapes.push_back(groundShape);
		m_collisionShapes.push_back(new btCylinderShape (btVector3(CUBE_HALF_EXTENTS,CUBE_HALF_EXTENTS,CUBE_HALF_EXTENTS)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		//groundTransform.setOrigin(btVector3(5,5,5));

		//We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
		{
			btScalar mass(0.);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0,0,0);
			if (isDynamic)
				groundShape->calculateLocalInertia(mass,localInertia);

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
			btRigidBody* body = new btRigidBody(rbInfo);

			//add the body to the dynamics world
			m_dynamicsWorld->addRigidBody(body);
		}


		{
			//create a few dynamic rigidbodies
			// Re-using the same collision is better for memory usage and performance

			btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));

			//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
			m_collisionShapes.push_back(colShape);

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar	mass(1.f);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0,0,0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass,localInertia);

			int gNumObjects = 120;
			int i;
			for (i=0;i<gNumObjects;i++)
			{
				btCollisionShape* shape = m_collisionShapes[1];

				btTransform trans;
				trans.setIdentity();

				//stack them
				int colsize = 10;
				int row = (i*CUBE_HALF_EXTENTS*2)/(colsize*2*CUBE_HALF_EXTENTS);
				int row2 = row;
				int col = (i)%(colsize)-colsize/2;


				if (col>3)
				{
					col=11;
					row2 |=1;
				}

				btVector3 pos(col*2*CUBE_HALF_EXTENTS + (row2%2)*CUBE_HALF_EXTENTS,
						row*2*CUBE_HALF_EXTENTS+CUBE_HALF_EXTENTS+EXTRA_HEIGHT,0);

				trans.setOrigin(pos);

				float mass = 1.f;

				btRigidBody* body = localCreateRigidBody(mass,trans,shape);


				///when using m_ccdMode
				if (m_ccdMode==USE_CCD)
				{
					body->setCcdMotionThreshold(CUBE_HALF_EXTENTS);
					body->setCcdSweptSphereRadius(0.9*CUBE_HALF_EXTENTS);
				}
			}
		}

	}


	void shootBox(const btVector3& destination) {
		if (m_dynamicsWorld) {
			float mass = 1.f;
			btTransform startTransform;
			startTransform.setIdentity();
			btVector3 camPos = getCameraPosition();
			startTransform.setOrigin(camPos);

			setShootBoxShape ();


			btRigidBody* body = this->localCreateRigidBody(mass, startTransform,m_shootBoxShape);
			body->setLinearFactor(btVector3(1,1,1));
			//body->setRestitution(1);

			btVector3 linVel(destination[0]-camPos[0],destination[1]-camPos[1],destination[2]-camPos[2]);
			linVel.normalize();
			linVel*=m_ShootBoxInitialSpeed;

			body->getWorldTransform().setOrigin(camPos);
			body->getWorldTransform().setRotation(btQuaternion(0,0,0,1));
			body->setLinearVelocity(linVel);
			body->setAngularVelocity(btVector3(0,0,0));
			body->setContactProcessingThreshold(1e30);

			///when using m_ccdMode, disable regular CCD
			if (m_ccdMode==USE_CCD)
			{
				body->setCcdMotionThreshold(CUBE_HALF_EXTENTS);
				body->setCcdSweptSphereRadius(0.4f);
			}

		}
	}




	void exitPhysics() {

		//cleanup in the reverse order of creation/initialization

		//remove the rigidbodies from the dynamics world and delete them
		int i;
		for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject( obj );
			delete obj;
		}

		//delete collision shapes
		for (int j=0;j<m_collisionShapes.size();j++)
		{
			btCollisionShape* shape = m_collisionShapes[j];
			delete shape;
		}
		m_collisionShapes.clear();

		delete m_dynamicsWorld;

		delete m_solver;

		delete m_broadphase;

		delete m_dispatcher;

		delete m_collisionConfiguration;


	}



};
