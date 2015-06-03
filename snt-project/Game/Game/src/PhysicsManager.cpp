#include "../include/stdafx.h"
#include "../include/PhysicsManager.h"
#include "../include/Global.h"
#include "../include/SceneObject.h"
#include "../include/Object.h"

#include <OgreSubMesh.h>

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
btRigidBody* PhysicsManager::createBody(const btTransform &transform, float mass, btCollisionShape* shape)
{
	//Averiguamos si el objeto es dinamico o no. Si su masa es 0, Bullet lo toma como estatico (paredes, etc).
	bool isDynamic = (mass != 0.0f);
	btVector3 localInertia(0,0,0);

	//Si es dinamico es necesario comprobar su "inercia", como se comporta ante las fuerzas angulares (giros)
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	//Clase/interfaz entre un motor grafico y el sistema de fisicas (version estandar de Bullet)
	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);

	//Creamos el cuerpo rigido y lo agregamos al mundo
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState, shape, localInertia);
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

	//Shape adaptado al objeto
	Ogre::Vector3 nodeSize = node->_getWorldAABB().getSize();
	btCollisionShape* shape = createBoxShape(nodeSize.x, nodeSize.y, nodeSize.z);

	//Creacion del cuerpo rigido que envuelve al sceneNode
	btRigidBody* body = gPhysics->createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), mass, shape);

	SceneObject* sceneObject = new SceneObject(entity, node, body);

	return sceneObject;
}

//---------------------------------------------------------------------------
//Creacion de BoxShape
//---------------------------------------------------------------------------
btCollisionShape* PhysicsManager::createBoxShape(float x, float y, float z)
{
	btCollisionShape* shape = new btBoxShape(btVector3(x/2, y/2, z/2));
	gCollisionShapes.push_back(shape);

	return shape;
}


//---------------------------------------------------------------------------
//Creacion de ConvexHullShape (mesh)
//---------------------------------------------------------------------------
SceneObject* PhysicsManager::createConvexHullShape(const char *name, const Ogre::Real &size, const Ogre::Vector3 &pos, float mass, Ogre::String meshName)
{
	//Creacion del SceneNode que representara el objeto en pantalla
	Ogre::SceneNode* node = gSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	
	//Creacion de la entidad, attach y scale
	Ogre::Entity *entity = gSceneMgr->createEntity(name, meshName);
	node->attachObject(entity);
	node->setPosition(pos.x, pos.y, pos.z);

	//Escalamos el nodo
	Ogre::Vector3 oScale = node->getScale();
	node->setScale(oScale.x*size, oScale.y*size, oScale.z*size);

	node->_updateBounds();

	//Obtencion de la informacion del mesh, aplicando la escala adecuada a los vertices
	size_t vertex_count,index_count;
	Ogre::Vector3* vertices;
	unsigned long* indices;
	Ogre::Vector3 position = Ogre::Vector3::ZERO;
	Ogre::Quaternion orient = Ogre::Quaternion::IDENTITY;
	Ogre::Vector3 scale = node->getScale();
	getMeshInformation(entity,vertex_count,vertices,index_count,indices, position, orient, scale);

	//Creacion del btConvexHullShape
	btConvexHullShape* shape = new btConvexHullShape();
	for (int i = 0; i < vertex_count; i++)
    {
        Ogre::Vector3 v = vertices[i];
        btVector3 btv = btVector3(v.x, v.y, v.z);
        ((btConvexHullShape*)shape)->addPoint(btv);
    }

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

	btCollisionShape* shape = createBoxShape(size.x, size.y, size.z);

	btRigidBody* body = gPhysics->createBody(btTransform(btQuaternion::getIdentity(), btVector3(pos.x, pos.y, pos.z)), 0, shape);

	SceneObject* sceneObject = new SceneObject(entity, node, body);

	return sceneObject;
}

//---------------------------------------------------------------------------
//Creacion de objetos "magicos"
//---------------------------------------------------------------------------
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
	btCollisionShape* shape = createBoxShape(size.x, size.y, size.z);

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


//---------------------------------------------------------------------------
//Permite obtener toda la informacion relacionada con el mesh de la entidad
//---------------------------------------------------------------------------
void PhysicsManager::getMeshInformation(const Ogre::Entity* const ent,
                        size_t &vertex_count,
                        Ogre::Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale)
{
    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;
 
    // Calculate how many vertices and indices we're going to need
    for ( unsigned short i = 0; i < ent->getMesh()->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = ent->getMesh()->getSubMesh(i);
        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += ent->getMesh()->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }
        // Add the indices
        index_count += submesh->indexData->indexCount;
    }
 
    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];
 
    added_shared = false;
 
    // Run through the submeshes again, adding the data into the arrays
    for (unsigned short i = 0; i < ent->getMesh()->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = ent->getMesh()->getSubMesh(i);
 
        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? ent->getMesh()->sharedVertexData : submesh->vertexData;
 
        if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }
 
            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
 
            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
 
            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
 
            // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
            //  as second argument. So make it float, to avoid trouble when Ogre::Real will
            //  be comiled/typedefed as double:
            //Ogre::Real* pReal;
            float* pReal;
 
            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);
                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);
                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }
 
            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }
 
        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
 
        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
 
        unsigned long* pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);
 
        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
 
        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                                          static_cast<unsigned long>(offset);
            }
        }
 
        ibuf->unlock();
        current_offset = next_offset;
    }
}