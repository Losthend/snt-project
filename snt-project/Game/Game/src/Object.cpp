#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/Global.h"
#include "../include/FrameRate.h"
#include "../include/SceneObject.h"
#include "../include/PhysicsManager.h"
#include "../include/CollisionManager.h"

/* 
	Objeto de:
		- Tipo 1: estaticos
		- Tipo 2: dinamico, el jugador puede agarrarlo
		- Tipo 3: gallinas
		- Tipo 4: Enemigo oso
*/

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
Object::Object(int objType, SceneObject* sceneObject)
{
	m_sceneObject = sceneObject;
	
	m_sceneObject->mRigidBody->setActivationState(DISABLE_DEACTIVATION);

	FPS = getframeLength();

	m_objType = objType;
	
	switch (m_objType)
	{
	case 3:
		m_moveX = 1;
		m_moveY = 2;
		break;
	default:
		m_moveX = 3;
		m_moveY = 2;
		break;
	}
	
	m_direction = Ogre::Vector3::ZERO;

	m_lookAt = true;
}

//---------------------------------------------------------------------------
//Destructor
//---------------------------------------------------------------------------
Object::~Object(void)
{
	for(unsigned x = 0; x < gObjects.size(); x++)
		if(gObjects[x]->m_sceneObject->mNode->getName() == m_sceneObject->mNode->getName())
			gObjects.erase(gObjects.begin()+x);

	gSceneMgr->destroyEntity(m_sceneObject->mEntity->getName());

	gSceneMgr->destroySceneNode(m_sceneObject->mNode->getName());

	gPhysics->mWorld->removeRigidBody(m_sceneObject->mRigidBody);
}

//---------------------------------------------------------------------------
//Metodo para controlar las colisiones del objeto
//---------------------------------------------------------------------------
void Object::update()
{

	//Objetos Tipo 2: solo si este objeto es de tipo 2, el jugador esta agarrando un objeto y dicho objeto es el mismo que este 
	if (m_objType == 2 && gPlayer->m_catchObj != 0 && (gPlayer->m_catchObj->m_sceneObject->mNode->getName() == m_sceneObject->mNode->getName()))
		type2();
	//Objetos tipo 3: Gallinas
	else if(m_objType == 3)
		type3();
	//Objetos tipo 3: Osos
	else if(m_objType == 4)
		type4();
	else if(m_objType == 5)
		type5();

	//Update en Bullet
	m_sceneObject->update();

}

//---------------------------------------------------------------------------
//Objeto de tipo 2: Telequineticos
//---------------------------------------------------------------------------
void Object::type2()
{
	//Raycasting para obtener las coordenadas del raton
	OIS::MouseState ms = gMouse->getMouseState();
	Ogre::Ray ray = gCamera->getCameraToViewportRay(ms.X.abs/(float)gWindow->getWidth(),ms.Y.abs/(float)gWindow->getHeight()); 

	//Coordenadas del raton
	Ogre::Vector3 mouseCoord = ray.getPoint(gCamera->getPosition().z);

	//Coordenadas del objeto a mover
	Ogre::Vector3 objCoord = m_sceneObject->mNode->getPosition();

	//Direccion del impulso
	Ogre::Real x = 0;
	Ogre::Real y = 0;

	//Margen minimo de acercamiento "mouse/obj"
	Ogre::Real minNear = 5;

	//Control X
	if(mouseCoord.x-minNear > objCoord.x)
		x = m_moveX;
	else if (mouseCoord.x+minNear < objCoord.x)
		x = -m_moveX;

	//Control Y
	if(mouseCoord.y-minNear > objCoord.y)
		y = m_moveY;
	else if (mouseCoord.y+minNear < objCoord.y)
		y = -m_moveY;

	//Movimiento
	m_sceneObject->mRigidBody->translate(btVector3(x, y, 0));
}

//---------------------------------------------------------------------------
//Objeto de tipo 3: Gallinas
//---------------------------------------------------------------------------
void Object::type3()
{
	//_@Crow _@Drink _@Peck _@Peck2 _@ScratchL _@ScratchR _@Sit _Run _Stand _Walk

	Ogre::AnimationState* mAnimWalk = m_sceneObject->mEntity->getAnimationState("_Run");
	bool rotate = false;
	btTransform transform;

	m_sceneObject->mRigidBody->getMotionState()->getWorldTransform(transform);

	//Deteccion de colision + cambio de direccion
	CollisionManager cm = CollisionManager();
	gPhysics->mWorld->contactPairTest(m_sceneObject->mRigidBody, gPlayer->m_sceneObject->mRigidBody, cm);
	if (cm.m_collision)
	{
		m_direction.x = -m_direction.x;
		transform.setRotation(btQuaternion(0,m_direction.x,0,1));
		m_sceneObject->mRigidBody->setMotionState(new btDefaultMotionState(transform));
		m_lookAt = !m_lookAt;
	}
	cm.~CollisionManager();

	mAnimWalk->setLoop(true);
	mAnimWalk->setEnabled(true);
	mAnimWalk->addTime(Ogre::Real(FPS));

	//Mover la gallina
	m_sceneObject->mRigidBody->translate(btVector3(m_direction.x*m_moveX, 0, 0));
}

//---------------------------------------------------------------------------
//Objeto de tipo 4: Osos
//---------------------------------------------------------------------------
void Object::type4()
{
	//die idel_a idle_b idle_stand run
	Ogre::AnimationState* mAnim = m_sceneObject->mEntity->getAnimationState("idel_a");

	//IDEA
	//Si el jugador esta a X distancia, se mantiene quieto "idel_a"
	//Si el jugador esta a Y distancia, se pone en alerta "idle_stand"
	//Si el jugador esta a Z distancia, ataca "run"
	//Si su vida es menor de 10, reproduce una vez la muerte "die" y pone la vida a cero para no realizar ninguna accion mas

	mAnim->setLoop(true);
	mAnim->setEnabled(true);
	mAnim->addTime(Ogre::Real(FPS)*2);
}

//---------------------------------------------------------------------------
//Objeto de tipo 5: Puente
//---------------------------------------------------------------------------
void Object::type5()
{
	//Si el jugador colisiona con el puente, este se cae (es necesario re-crear el objeto)
	if (m_sceneObject->mMass == 0)
	{
		CollisionManager cm = CollisionManager();
		gPhysics->mWorld->contactPairTest(m_sceneObject->mRigidBody, gPlayer->m_sceneObject->mRigidBody, cm);
		if (cm.m_collision)
		{
			//Almacenamos los datos necesarios
			Ogre::String name = m_sceneObject->mNode->getName();
			Ogre::Vector3 pos = m_sceneObject->mNode->getPosition();
			//Eliminamos aquello que cambia y provoca conflicto
			gSceneMgr->destroyEntity(m_sceneObject->mEntity->getName());
			gSceneMgr->destroySceneNode(name);
			gPhysics->mWorld->removeRigidBody(m_sceneObject->mRigidBody);
			//Creamos el "nuevo" objeto
			//SceneObject* sceneObj = gPhysics->createPrimitiveShape(name, size, pos, 10, "WoodPallet.mesh");	
			SceneObject* sceneObj = gPhysics->createConvexHullShape(name, Ogre::Real(70), pos, 10, "stone.mesh");
			sceneObj->mEntity->setMaterialName("Material/stone2");
			//Asignamos los valores del "nuevo" objeto a este objeto
			m_sceneObject->mRigidBody = sceneObj->mRigidBody;
			m_sceneObject->mNode = sceneObj->mNode;
			m_sceneObject->mEntity = sceneObj->mEntity;
			m_sceneObject->mMass = sceneObj->mMass;
		}
		cm.~CollisionManager();
	}
}