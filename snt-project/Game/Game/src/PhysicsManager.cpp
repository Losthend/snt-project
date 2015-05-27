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

	SceneObject* sceneObject = new SceneObject(*node, *body);

	return sceneObject;
}



//---------------------------------------------------------------------------
//Creacion de planos/suelo entre Ogre y bullet
//---------------------------------------------------------------------------
SceneObject* PhysicsManager::createGroundObject(Ogre::String name, Ogre::Vector3 pos, Ogre::Vector3 size, bool grass)
{
	//Plano
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().createPlane(name, 
																		  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
																		  plane, size.x, size.z, 
																		  20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	//Entidad 
	Ogre::Entity *entity = gSceneMgr->createEntity(name, name);
	entity->setMaterialName("Examples/GrassFloor");
	//Nodo
	Ogre::SceneNode *node = gSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(entity);
	node->setPosition(pos.x, pos.y, pos.z);
	node->_updateBounds();
 
	//Hierba
	if(grass)
		createGroundGrass(size, pos);

	float mass = 0;

	btCollisionShape &shape = createBoxShape(size.x, size.y, size.z);

	btRigidBody* body = gPhysics->createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), mass, shape);

	SceneObject* sceneObject = new SceneObject(*node, *body);

	return sceneObject;
}


//---------------------------------------------------------------------------
//Añade hierba al suelo
//---------------------------------------------------------------------------
void PhysicsManager::createGroundGrass(Ogre::Vector3 vSize, Ogre::Vector3 vPos)
{
	Ogre::Entity* grass = gSceneMgr->createEntity("manualObjectGrass");
	Ogre::StaticGeometry* sg = gSceneMgr->createStaticGeometry("GrassArea");

	int amount = 10;

	sg->setRegionDimensions(Ogre::Vector3(vSize.x, vSize.y, vSize.z));
	sg->setOrigin(Ogre::Vector3(vSize.x, vSize.y, vSize.z));

	for (int x = -vSize.x/2; x < vSize.x/2; x += (vSize.x / amount))
	{
		for (int z = -vSize.z/2; z < vSize.z/2; z += (vSize.z / amount))
		{
			Ogre::Real offsetX = -vSize.x / (float)amount / 2;
			Ogre::Real offsetZ = -vSize.z / (float)amount / 2;
			Ogre::Vector3 pos(
				x + Ogre::Math::RangeRandom(-offsetX, offsetX),
				vPos.y,
				z + Ogre::Math::RangeRandom(-offsetZ, offsetZ));
			Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.9, 1.1), 1);
			Ogre::Quaternion quat;
			quat.FromAngleAxis(
				Ogre::Degree(Ogre::Math::RangeRandom(0, 359)),
				Ogre::Vector3::UNIT_Y);
			sg->addEntity(grass, pos, quat, scale);
		}
	}
	sg->build();
}