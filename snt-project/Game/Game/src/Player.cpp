#include "../include/stdafx.h"

#include "../include/Player.h"

#include "../include/Object.h"
#include "../include/Global.h"
#include "../include/FrameRate.h"
#include "../include/SceneObject.h"
#include "../include/PhysicsManager.h"
#include "../include/AnimationManager.h"

//---------------------------------------------------------------------------
Player::Player(SceneObject* sceneObject)
{
	FPS = getframeLength();

	m_sceneObject = sceneObject;
	m_sceneObject->mRigidBody->setAngularFactor(btVector3(0,0,0));

	//Ninja hacia el eje X+, depende del rigidbody
	btTransform transform;
	m_sceneObject->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,1,0,1));
	m_sceneObject->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	m_lookAt = true;

	//Requiere esto ya que, tras cada iteración, si no hay movimiento lineal se bloquea el rigidbody
	m_sceneObject->mRigidBody->setActivationState(DISABLE_DEACTIVATION);

	m_fall = false;
	m_jump = false;
	m_inJump = false;

	m_run = false;

	m_SwordsDrawn = false;
	m_attack = false;

	m_moveX = 2;
	m_moveY = -gPhysics->mWorld->getGravity().y()/2;

	m_catchObj = 0;

	m_direction = Ogre::Vector3::ZERO;
}
//---------------------------------------------------------------------------
Player::~Player(void)
{

}

//---------------------------------------------------------------------------
//--Control del jugador
//---------------------------------------------------------------------------
void Player::update()
{
	//-------------------------------------------------------------------
	//GIROS
	//--------------------------------------------------------------------
	//Si va hacia la derecha y esta mirando a la izquierda (m_lookAt=FALSE), o viceversa, hay que rotar
	if(m_direction.x == 1 && !m_lookAt || m_direction.x == -1 && m_lookAt)
	{
		btTransform transform;
		m_sceneObject->mRigidBody->getMotionState()->getWorldTransform(transform);
		transform.setRotation(btQuaternion(0,m_direction.x,0,1));
		m_sceneObject->mRigidBody->setMotionState(new btDefaultMotionState(transform));
		m_lookAt = !m_lookAt;
	}

	//-------------------------------------------------------------------
	//SALTOS (Movimiento en eje +Y)
	//---------------------------------------------------------------------

	//Si tienes que saltar, no estas cayendo y no estas saltando, saltas
	if (m_jump && !m_fall && !m_inJump)
	{
		m_sceneObject->mRigidBody->applyCentralImpulse(btVector3(0, m_moveY*m_sceneObject->mMass, 0));
		m_inJump = true;
	}

	//-------------------------------------------------------------------
	//MOVIMIENTO EN EL EJE X
	//--------------------------------------------------------------------

	m_sceneObject->mRigidBody->translate(btVector3(m_direction.x*m_moveX, 0, 0));

	//-------------------------------------------------------------------
	//UPDATES
	//-------------------------------------------------------------------
	//Movimientos en Bullet 
	m_sceneObject->update();

	//Animaciones
	m_animMgr->addTime(Ogre::Real(0.017));

	//Camara: centrada en el jugador
	Ogre::Vector3 pos = m_sceneObject->mNode->getPosition();
	pos.y = 50 + pos.y;
	pos.z = gCamera->getPosition().z;
	gCamera->setPosition(pos);
}

//------------------------------------------------------------
//Permite lanzar objetos agarrados
//------------------------------------------------------------
void Player::catchAttack()
{
	//Ogre::Vector3 direction = m_catchObj->m_sceneObject->mNode->getPosition() - m_sceneObject->mNode->getPosition();
	Ogre::Vector3 direction = m_catchObj->m_sceneObject->mNode->_getWorldAABB().getCenter() - m_sceneObject->mNode->_getWorldAABB().getCenter();
	btScalar power = 5*m_catchObj->m_sceneObject->mMass;
	btScalar x = direction.x*power;
	btScalar y = direction.y*power;

	m_catchObj->m_sceneObject->mRigidBody->applyCentralImpulse(btVector3(x, y, 0));
	m_catchObj->m_sceneObject->mRigidBody->setGravity(gPhysics->mWorld->getGravity());
	m_catchObj = 0;
}

//------------------------------------------------------------
//Controla la caida posterior al salto
//------------------------------------------------------------
bool Player::fallManager()
{
	//Extremos del bounding box
	Ogre::Vector3 nearLeft = m_sceneObject->mNode->_getWorldAABB().getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::Vector3 nearRight = m_sceneObject->mNode->_getWorldAABB().getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);

	//Evita que "escale" por superficies BOX (optimizable)
	Ogre::Real sizeX = m_sceneObject->mNode->_getWorldAABB().getSize().x;
	nearLeft.x = Ogre::Real(nearLeft.x + sizeX*0.065);
	nearRight.x = Ogre::Real(nearRight.x - sizeX*0.065);

	//Centro del bounding box
	Ogre::Vector3 center = (nearRight + nearLeft ) / 2;

	//Comprobamos si durante la caida colisionará con algun objeto
	Object* obj1 = rayFromPoint(nearLeft, Ogre::Vector3::NEGATIVE_UNIT_Y);
	Object* obj2 = rayFromPoint(nearRight, Ogre::Vector3::NEGATIVE_UNIT_Y);
	Object* obj3 = rayFromPoint(center, Ogre::Vector3::NEGATIVE_UNIT_Y);

	//Obtenemos las distancias
	Ogre::Real dist1 = 10;
	Ogre::Real dist2 = 10;
	Ogre::Real dist3 = 10;

	if(obj1 != 0)
		dist1 = obj1->m_sceneObject->mNode->_getWorldAABB().squaredDistance(nearLeft);
	if(obj2 != 0)
		dist2 = obj2->m_sceneObject->mNode->_getWorldAABB().squaredDistance(nearRight);
	if(obj3 != 0)
		dist3 = obj3->m_sceneObject->mNode->_getWorldAABB().squaredDistance(center);

	//Comprobamos la distancia entre el objeto y el punto, finalizando la caida cuando sea correcto
	Ogre::Real minDist = std::min(dist1, std::min(dist2, dist3));
	if(minDist < 10)
		return true;
	else
		return false;
}

//------------------------------------------------------------
//Devuelve el primer objeto detectado por el rayo
//-----------------------------------------------------------
Object* Player::rayFromPoint(Ogre::Vector3 origin, Ogre::Vector3 direction)
{
	Object* obj = 0;

	Ogre::Ray ray(Ogre::Vector3(origin.x, origin.y, origin.z), direction);

	Ogre::RaySceneQuery* mRaySceneQuery = gSceneMgr->createRayQuery(ray); 
	mRaySceneQuery->setRay(ray);
	mRaySceneQuery->setSortByDistance(true);

	Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	//OPTIMIZABLE con inCameraFrustrumObjects para no buscar por todos los objetos

	while(itr != result.end() && obj == 0)
	{
		for(unsigned x = 0; x < gObjects.size(); x++)
			if(itr->movable->getParentNode()->getName() == gObjects[x]->m_sceneObject->mNode->getName())
				obj = gObjects[x];
		itr++;
	}

	return obj;
}


/*
Ogre::String mValue = "Frase";
OutputDebugString(mValue.c_str());
OutputDebugString("\n");

int num = 1;
OutputDebugString(Ogre::StringConverter::toString(num).c_str());
OutputDebugString("\n");
*/