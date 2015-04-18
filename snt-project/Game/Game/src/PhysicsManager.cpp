#include "../include/stdafx.h"
#include "../include/PhysicsManager.h"
#include "../include/Global.h"
#include "../include/SceneObject.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
PhysicsManager::PhysicsManager()
{
	mCollisionDispatcher = new btCollisionDispatcher(&mCollisionConfig);
	mWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, &mBroadphase, &mConstraintSolver, &mCollisionConfig);
	mWorld->setGravity(btVector3(0,-9.81f,0));
}

//---------------------------------------------------------------------------
//Destructor
//---------------------------------------------------------------------------
PhysicsManager::~PhysicsManager(void)
{
	delete mWorld;
	delete mCollisionDispatcher;

	//Destruir los cuerpos rigidos agregados al mundo cuando desaparezca el PhysicsManager
	for (int i=mWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = mWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		mWorld->removeCollisionObject( obj );
		delete obj;
	}
}

//---------------------------------------------------------------------------
//Metodo para la creacion de cuerpos rigidos
//Transform: clase de Bullet para representar cambios, velocidad de objeto, posicion, etc
//mass: numero en coma flotante que indica la masa del objeto
//shape: una referencia a la forma de colision que el objeto va a utilizar
//---------------------------------------------------------------------------
btRigidBody* PhysicsManager::createBody(const btTransform &transform, float mass, btCollisionShape &shape)
{
	//Averiguamos si el objeto es dinamico o no. Si su masa es 0, Bullet lo toma como estatico (paredes, etc).
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0,0,0);

	//Si es dinamico es necesario comprobar su "inercia", como se comporta ante las fuerzas angulares (giros)
	if (isDynamic)
		shape.calculateLocalInertia(mass, localInertia);

	//Clase/interfaz entre un motor grafico y el sistema de fisicas (version estandar de Bullet)
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);

	//Creamos el cuerpo rigido y lo agregamos al mundo
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState, &shape, localInertia);
	btRigidBody* body = new btRigidBody(rbInfo);
	mWorld->addRigidBody(body);
	
	//Movimiento solo en 2D
	body->setLinearFactor(btVector3(1,1,0));
	body->setAngularFactor(btVector3(1,1,0));

	return body;

}

//---------------------------------------------------------------------------
//Metodo para la simulacion fisica de los objetos en movimiento, el tratamiento de colisiones y la actualizacion de los motionState
//---------------------------------------------------------------------------
void PhysicsManager::update(float ticks)
{
	mWorld->stepSimulation(ticks, 0);
}

//---------------------------------------------------------------------------
//Metodo
//---------------------------------------------------------------------------
btCollisionShape& PhysicsManager::createBoxShape(float x, float y, float z)
{
	btCollisionShape* shape = new btBoxShape(btVector3(x/2, y/2, z/2));
	gCollisionShapes.push_back(shape);

	return *shape;
}

//---------------------------------------------------------------------------
//Metodo para la creacion de objetos caja entre Ogre y Bullet
//---------------------------------------------------------------------------
SceneObject* PhysicsManager::createBoxObject(const char *name, const Ogre::Vector3 &size, const Ogre::Vector3 &pos, float mass, btCollisionShape &shape)
{
	//Creacion del SceneNode que representara el objeto en pantalla
	Ogre::SceneNode* node1 = gSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	
	//Creacion del cuerpo rigido que envuelve al sceneNode
	btRigidBody* body = gPhysics->createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), mass, shape);

	//Creacion de la entidad y attach de la misma al objeto
	Ogre::Entity *entity = gSceneMgr->createEntity(name, "ManualObjectCube");
	node1->attachObject(entity);

	node1->setScale(size.x / 10.f, size.y / 10.f, size.z / 10.f);

	SceneObject* sceneObject = new SceneObject(*node1, *body);

	return sceneObject;

}

