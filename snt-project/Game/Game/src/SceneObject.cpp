#include "../include/stdafx.h"
#include "../include/SceneObject.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
SceneObject::SceneObject(Ogre::SceneNode &node, btRigidBody &body):
	mNode(node),
	mRigidBody(body)
{
	//Empty
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
	mRigidBody.getMotionState()->getWorldTransform(transform);
	
	//Copiamos su Quaternion y la rotacion del SceneNode
	btQuaternion rotation(transform.getRotation());
	mNode.setOrientation(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());

	//Finalmente, tomamos la posición del cuerpo rigido y se la aplicamos al SceneNode
	const btVector3 &origin = transform.getOrigin();
	mNode.setPosition(origin.getX(), origin.getY(), origin.getZ());

}

