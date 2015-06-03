#include "../include/stdafx.h"
#include "../include/PhysicsManager.h"
#include "../include/Global.h"
#include "../include/SceneObject.h"
#include "../include/Object.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
PhysicsManager::PhysicsManager()
{
	mCollisionDispatcher = new btCollisionDispatcher(&mCollisionConfig);
	mWorld = new btDiscreteDynamicsWorld(mCollisionDispatcher, &mBroadphase, &mConstraintSolver, &mCollisionConfig);
	mWorld->setGravity(btVector3(0,-9.81f,0));
	m_magicCount = 0;
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
	body->setAngularFactor(btVector3(1,0,1));

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
//Metodo para la creacion de objetos BOX entre Ogre y Bullet
//---------------------------------------------------------------------------
SceneObject* PhysicsManager::createBoxObject(const char *name, const Ogre::Vector3 &size, const Ogre::Vector3 &pos, float mass, Ogre::String meshName)
{
	//Creacion del SceneNode que representara el objeto en pantalla
	Ogre::SceneNode* node = gSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	
	//Creacion de la entidad, attach y scale
	Ogre::Entity *entity = gSceneMgr->createEntity(name, meshName);
	node->attachObject(entity);
	node->setScale(size.x / 10.f, size.y / 10.f, size.z / 10.f);
	node->setPosition(pos.x, pos.y, pos.z);
	node->_updateBounds();

	Ogre::AxisAlignedBox aabb = node->_getWorldAABB();

	//Shape adaptado al objeto
	Ogre::Vector3 nodeSize = aabb.getSize();
	btCollisionShape &shape = createBoxShape(nodeSize.x, nodeSize.y, nodeSize.z);

	//Creacion del cuerpo rigido que envuelve al sceneNode
	btRigidBody* body = gPhysics->createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), mass, shape);

	SceneObject* sceneObject = new SceneObject(entity, node, body);

	return sceneObject;
}



//---------------------------------------------------------------------------
//Creacion de planos/suelo entre Ogre y bullet
//---------------------------------------------------------------------------
SceneObject* PhysicsManager::createGroundObject(Ogre::String name, Ogre::Vector3 size, Ogre::Vector3 pos, Ogre::Vector2 repeat, Ogre::String material)
{
	//Plano
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().createPlane(name, 
																		  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
																		  plane, size.x, size.z, 
																		  20, 20, true, 1, repeat.x, repeat.y, Ogre::Vector3::UNIT_Z);
	//Entidad 
	Ogre::Entity *entity = gSceneMgr->createEntity(name, name);
	entity->setMaterialName(material);
	//Nodo
	Ogre::SceneNode *node = gSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(entity);
	node->setPosition(pos.x, pos.y, pos.z);
	node->_updateBounds();

	btCollisionShape &shape = createBoxShape(size.x, size.y, size.z);

	btRigidBody* body = gPhysics->createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), 0, shape);

	SceneObject* sceneObject = new SceneObject(entity, node, body);

	return sceneObject;
}

void PhysicsManager::magicGenerator(Ogre::Vector3 pos)
{
	Ogre::String id;
	id = Ogre::StringConverter::toString(m_magicCount).c_str();

	if(m_magicCount>9)
	{
		bool chickenDelete = false;
		for(unsigned x = 0; x < gObjects.size() && !chickenDelete; x++)
		{
			for (unsigned y = 9; y > 0 && !chickenDelete; y--)
			{
				id = Ogre::StringConverter::toString(y).c_str();
				if(gObjects[x]->m_sceneObject->mNode->getName() == "magic"+id)
				{
					gObjects[x]->~Object();
					m_magicCount--;
					chickenDelete = true;
				}
			}
		}
	}

	m_magicCount++;

	Ogre::Entity *entity = gSceneMgr->createEntity("magic"+id, "chicken.mesh");

	//Nodo
	Ogre::SceneNode *node = gSceneMgr->getRootSceneNode()->createChildSceneNode("magic"+id);
	node->attachObject(entity);
	node->setScale(2, 2, 2);
	node->setPosition(pos.x, pos.y, 0);
	node->_updateBounds();

	Ogre::Vector3 size = node->_getWorldAABB().getSize();
	btCollisionShape &shape = createBoxShape(size.x, size.y, size.z);

	btRigidBody* body = gPhysics->createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), 1, shape);

	SceneObject* sceneObject = new SceneObject(entity, node, body);

	//Mirar inicialmente hacia el eje X+
	btTransform transform;
	sceneObject->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,1,0,1));
	sceneObject->mRigidBody->setMotionState(new btDefaultMotionState(transform));

	Object* obj = new Object(3, sceneObject);
	obj->m_direction.x = 1;
	gObjects.push_back(obj);

}