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
		- Tipo 2: telequinesis
		- Tipo 3: gallinas
		- Tipo 4: Enemigo (oso)
		- Tipo 5: Puente
		- Tipo 6: Plataforma
		- Tipo 7: Trampa
		- Tipo 8: "Jefe" final
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
	case 4:
		m_moveX = 2;
		m_moveY = 2;
		m_sceneObject->mRigidBody->setLinearFactor(btVector3(0,1,0));
		m_sceneObject->mRigidBody->setAngularFactor(btVector3(0,0,0));
		break;
	default:
		m_moveX = 3;
		m_moveY = 2;
		break;
	}
	
	m_direction = Ogre::Vector3::ZERO;

	m_lookAt = true;

	m_lifeCounter = 100;
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
	//Objetos tipo 4: Osos
	else if(m_objType == 4)
		type4();
	//Objetos tipo 5: Puente
	else if(m_objType == 5)
		type5();
	//Objetos tipo 6: Plataforma
	else if(m_objType == 6)
		type6();
	//Objetos tipo 7: Trampa
	else if(m_objType == 7)
		type7();
	//Objetos tipo 8: Jefe
	else if(m_objType == 8)
		type8();

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
	Ogre::AnimationState* mAnimIdel = m_sceneObject->mEntity->getAnimationState("idel_a");
	Ogre::AnimationState* mAnimStand = m_sceneObject->mEntity->getAnimationState("idle_stand");
	Ogre::AnimationState* mAnimRun = m_sceneObject->mEntity->getAnimationState("run");
	Ogre::AnimationState* mAnimDie = m_sceneObject->mEntity->getAnimationState("die");

	//IDEA
	if (m_lifeCounter > 0){
		//Si la vida es mayor que 10
		if(m_lifeCounter > 10){
			//Comprobar distancia
			Ogre::Real distance = m_sceneObject->mNode->_getWorldAABB().squaredDistance(gPlayer->m_sceneObject->mNode->getPosition());
			//Accion 1: Quieto
			if (distance > 30000){
				mAnimStand->setLoop(false);
				mAnimStand->setEnabled(false);
				mAnimStand->setTimePosition(0);
				mAnimIdel->setLoop(false);
				mAnimIdel->setEnabled(false);
				mAnimIdel->setLoop(true);
				mAnimIdel->setEnabled(true);
				mAnimIdel->addTime(Ogre::Real(FPS)*2);
			}
			//Accion 2: Alerta
			else if (distance > 12000){
				mAnimIdel->setLoop(false);
				mAnimIdel->setEnabled(false);
				mAnimRun->setLoop(false);
				mAnimRun->setEnabled(false);
				mAnimStand->setLoop(true);
				mAnimStand->setEnabled(true);
				mAnimStand->addTime(Ogre::Real(FPS)*2);
			}
			//Accion 3: Atacar
			else{
				mAnimIdel->setLoop(false);
				mAnimIdel->setEnabled(false);
				mAnimStand->setLoop(false);
				mAnimStand->setEnabled(false);
				mAnimStand->setTimePosition(0);
				mAnimRun->setLoop(true);
				mAnimRun->setEnabled(true);
				mAnimRun->addTime(Ogre::Real(FPS)*2);
				//Corriendo en direccion al jugador
				Ogre::Real playerX = gPlayer->m_sceneObject->mNode->getPosition().x;
				Ogre::Real objectX = m_sceneObject->mNode->getPosition().x;
				if (objectX <= playerX)
					m_direction.x = 1;
				else
					m_direction.x = -1;
				//Cambio de direccion
				btTransform transform;
				m_sceneObject->mRigidBody->getMotionState()->getWorldTransform(transform);
				if (transform.getRotation().getY() != m_direction.x){
					transform.setRotation(btQuaternion(0,m_direction.x,0,1));
					m_sceneObject->mRigidBody->setMotionState(new btDefaultMotionState(transform));
					m_lookAt = !m_lookAt;
				}
				//Movimiento
				m_sceneObject->mRigidBody->translate(btVector3(m_direction.x*m_moveX, 0, 0));
			}
		}
		//Si su vida es menor de 10
		else{
				mAnimIdel->setLoop(false);
				mAnimIdel->setEnabled(false);
				mAnimStand->setLoop(false);
				mAnimStand->setEnabled(false);
				mAnimRun->setLoop(false);
				mAnimRun->setEnabled(false);
				//Reproduce una vez la muerte "die" y pone la vida a cero para no realizar ninguna accion mas
				mAnimDie->setLoop(true);
				mAnimDie->setEnabled(true);
				mAnimDie->addTime(Ogre::Real(FPS));
				if (mAnimDie->getLength()*0.90 < mAnimDie->getTimePosition())
					m_lifeCounter = 0;
			}
	}
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

//---------------------------------------------------------------------------
//Objeto de tipo 6: plataforma
//---------------------------------------------------------------------------
void Object::type6()
{
	Ogre::Real pos = m_sceneObject->mNode->getPosition().z;
	if (pos >= 0)
		m_lookAt = true;
	else if (pos <= -280)
		m_lookAt = false;
	//Movimiento
	if (m_lookAt)
		m_sceneObject->mRigidBody->translate(btVector3(0, 0, -3));
	else
		m_sceneObject->mRigidBody->translate(btVector3(0, 0, 2));
}

//---------------------------------------------------------------------------
//Objeto de tipo 7: trampa
//---------------------------------------------------------------------------
void Object::type7()
{
	m_sceneObject->mRigidBody->applyTorqueImpulse(btVector3(0,0,300));
}

//---------------------------------------------------------------------------
//Objeto de tipo 8: "jefe" final
//---------------------------------------------------------------------------
void Object::type8()
{
	//idleA run walk
	Ogre::AnimationState* mAnim = m_sceneObject->mEntity->getAnimationState("Idle");

	mAnim->setLoop(true);
	mAnim->setEnabled(true);
	mAnim->addTime(Ogre::Real(FPS));
}