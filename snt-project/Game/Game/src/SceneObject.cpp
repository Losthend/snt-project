#include "../include/stdafx.h"
#include "../include/SceneObject.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
SceneObject::SceneObject(Ogre::Entity *entity, Ogre::SceneNode *node, btRigidBody *body, float mass)
{
	mEntity = entity;
	mNode = node;
	mRigidBody = body;
	mMass = mass;
}

//---------------------------------------------------------------------------
//Destructor
//---------------------------------------------------------------------------
SceneObject::~SceneObject(void)
{

}

//---------------------------------------------------------------------------
//Metodo para mantener un SceneNode sincronizado con si cuerpo rigido
//---------------------------------------------------------------------------
void SceneObject::update()
{
	//Creamos un objeto que almacena las transformaciones de Bullet
	btTransform transform;
	
	//Accedemos al cuerpo rigido y obtenemos su "estado de movimiento"
	mRigidBody->getMotionState()->getWorldTransform(transform);
	
	//Copiamos su Quaternion y la rotacion del SceneNode
	btQuaternion rotation(transform.getRotation());
	mNode->setOrientation(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());

	//Finalmente, tomamos la posici�n del cuerpo rigido y se la aplicamos al SceneNode
	//NOTA: independientemente del origen del nodo, centro del nodo y origen (centro) del rigidbody deben coincidir
	const btVector3 &origin = transform.getOrigin();
	//Ogre::Vector3 nodeCenter = mNode->_getWorldAABB().getCenter();
	//btVector3 center(nodeCenter.x, nodeCenter.y, nodeCenter.z);
	//btVector3 rute = origin - center;
	//mNode->translate(rute.x(), rute.y(), rute.z());
	mNode->setPosition(origin.getX(), origin.getY(), origin.getZ());
}

