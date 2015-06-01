#include "../include/stdafx.h"

#include "../include/Player.h"

#include "../include/Object.h"
#include "../include/Global.h"
#include "../include/FrameRate.h"
#include "../include/SceneObject.h"

//---------------------------------------------------------------------------
Player::Player(SceneObject* sceneObject)
{
	FPS = getframeLength();

	m_sceneObject = sceneObject;
	m_sceneObject->mRigidBody.setAngularFactor(btVector3(0,0,0));

	//Ninja hacia el eje X+, depende del rigidbody
	btTransform transform;
	m_sceneObject->mRigidBody.getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,-1,0,1));
	m_sceneObject->mRigidBody.setMotionState(new btDefaultMotionState(transform));
	m_lookAt = true;

	//Requiere esto ya que, tras cada iteración, si no hay movimiento lineal se bloquea el rigidbody
	m_sceneObject->mRigidBody.setActivationState(DISABLE_DEACTIVATION);

	m_fall = false;
	m_jump = false;
	m_inJump = false;

	m_crouchDown = false;
	m_crouchUp = false;
	m_run = false;

	m_moveX = 2;
	m_moveY = 30;

	//m_sceneObject->mNode.showBoundingBox(true);

	m_catchObj = 0;
	m_tkDistance = 10000;

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
	//CONTROL DE AGARRE
	//---------------------------------------------------------------------
	if (m_catchObj != 0)
		catchActions();

	//-------------------------------------------------------------------
	//CAIDA
	//--------------------------------------------------------------------
	if(!m_fall && !fallManager())
		m_fall = true;

	//-------------------------------------------------------------------
	//SALTOS (Movimiento en eje +Y)
	//---------------------------------------------------------------------

	//Si tienes que saltar, no estas cayendo y no estas saltando, saltas
	if (m_jump && !m_fall && !m_inJump)
	{
		m_sceneObject->mRigidBody.applyCentralImpulse(btVector3(0, m_moveY, 0));
		m_inJump = true;
	}

	//-------------------------------------------------------------------
	//MOVIMIENTO EN EL EJE X
	//--------------------------------------------------------------------

	m_sceneObject->mRigidBody.translate(btVector3(m_direction.x*m_moveX, 0, 0));

	//-------------------------------------------------------------------
	//UPDATES
	//-------------------------------------------------------------------
	//Movimientos en Bullet 
	m_sceneObject->update();

	//Animacion
	animationManager();

	//Camara: centrada en el jugador
	Ogre::Vector3 pos = m_sceneObject->mNode._getWorldAABB().getCenter();
	pos.y = 75 + pos.y;
	pos.z = gCamera->getPosition().z;
	gCamera->setPosition(pos);
}



//------------------------------------------------------------
//Metodo para el agarre de objetos
//------------------------------------------------------------
void Player::catchSolution(Object* obj)
{
	//Control de distancia
	Ogre::Real distance = m_sceneObject->mNode._getWorldAABB().squaredDistance(obj->m_sceneObject->mNode.getPosition());

	//Si la distancia entre el jugador y el objeto es menor de X unidades
	if (distance < m_tkDistance)
	{
		//Agarra el objeto y elimina su efecto de la gravedad
		m_catchObj = obj;
		m_catchObj->m_sceneObject->mRigidBody.setGravity(btVector3(0,0,0));
	}
}

//------------------------------------------------------------
//Metodo para el control de objetos agarrados
//------------------------------------------------------------
void Player::catchActions()
{
	//Control de distancia
	Ogre::Real distance = m_sceneObject->mNode._getWorldAABB().squaredDistance(m_catchObj->m_sceneObject->mNode.getPosition());

	//Si estamos a una distancia mayor
	if (distance >= m_tkDistance)
	{
		//Devuelve el efecto de la gravedad al objeto y suelta el objeto
		m_catchObj->m_sceneObject->mRigidBody.setGravity(btVector3(0,-9.81f,0));
		m_catchObj = 0;
	}
}

//------------------------------------------------------------
//Permite lanzar objetos agarrados
//------------------------------------------------------------
void Player::catchAttack()
{
	Ogre::Vector3 direction = m_catchObj->m_sceneObject->mNode.getPosition() - m_sceneObject->mNode.getPosition();
	
	btScalar power = 1; //DEPENDE DE LA MASA
	btScalar x = direction.x*power;
	btScalar y = direction.y*power;

	m_catchObj->m_sceneObject->mRigidBody.applyCentralImpulse(btVector3(x, y, 0));
	m_catchObj->m_sceneObject->mRigidBody.setGravity(btVector3(0,-9.81f,0));
	m_catchObj = 0;
}


//------------------------------------------------------------
//Gestiona las animaciones
//------------------------------------------------------------
void Player::animationManager()
{
	//Attack1 Attack2 Attack3 Backflip Block Climb Crouch Death1 Death2 HighJump Idle1 Idle2 Idle3 Jump JumpNoHeight Kick SideKick Spin Stealth Walk 
	Ogre::AnimationState* mAnimWalk = m_sceneObject->mEntity.getAnimationState("Walk");
	Ogre::AnimationState* mAnimJump = m_sceneObject->mEntity.getAnimationState("Jump");
	Ogre::AnimationState* mAnimCrouch = m_sceneObject->mEntity.getAnimationState("Crouch");

	//CAMINAR/CORRER (solo si no estas ni saltando ni cayendo)
	if(m_direction.x != 0 && !m_jump && !m_fall) 
		animWalk(mAnimWalk);
	else
		mAnimWalk->setEnabled(false);
	//SALTAR
	if(m_jump)
	{
		if (animJump(mAnimJump))
		{
			m_jump = false;
			m_fall = true;
		}
	}
	//CAER (principio)
	if(m_fall && !fallManager())
	{
		Ogre::Real time = mAnimJump->getTimePosition();
		if (time == 0)
		{
			mAnimJump->setTimePosition(mAnimJump->getLength()*0.9);
			mAnimJump->setEnabled(true);
		}
	}
	//CAER (final)
	if(m_fall && fallManager() && animFall(mAnimJump))
	{
		m_fall = false;
		m_inJump = false;
	}
	//AGACHARSE
	if(m_crouchDown)
		animCrouchDown(mAnimCrouch);
	if(m_crouchUp)
		animCrouchUp(mAnimCrouch);
	
}

void Player::animWalk(Ogre::AnimationState* mAnimWalk)
{
	bool rotate = false;

	//Comprobamos si el jugador mira en la direccion adecuada
	btTransform transform;
	m_sceneObject->mRigidBody.getMotionState()->getWorldTransform(transform);
	btScalar rotationY = transform.getRotation().y();

	//Si va hacia la derecha y esta mirando a la izquierda (m_lookAt=FALSE), o viceversa, hay que rotar
	if(m_direction.x == 1 && !m_lookAt || m_direction.x == -1 && m_lookAt)
		rotate = true;

	if (rotate)
	{
		transform.setRotation(btQuaternion(0,-m_direction.x,0,1));
		m_sceneObject->mRigidBody.setMotionState(new btDefaultMotionState(transform));
		m_lookAt = !m_lookAt;
	}
	else
	{
		mAnimWalk->setLoop(true);
		mAnimWalk->setEnabled(true);
		if(!m_run)
			mAnimWalk->addTime(Ogre::Real(FPS)*2);
		else
			mAnimWalk->addTime(Ogre::Real(FPS)*3);
	}
}


bool Player::animJump(Ogre::AnimationState* mAnimJump)
{
	Ogre::Real time = mAnimJump->getTimePosition();
	Ogre::Real maxTime = mAnimJump->getLength();
	//SALTAR
	if (time < (maxTime*0.75))
	{
		mAnimJump->setLoop(false);
		mAnimJump->setEnabled(true);
		mAnimJump->addTime(Ogre::Real(FPS));
		return false;
	}
	else
		return true;
}

bool Player::animFall(Ogre::AnimationState* mAnimJump)
{
	Ogre::Real time = mAnimJump->getTimePosition();
	Ogre::Real maxTime = mAnimJump->getLength();

	if(time >= maxTime)
	{
		mAnimJump->setTimePosition(0);
		mAnimJump->setEnabled(false);
		return true;
	}
	else
	{
		mAnimJump->setLoop(false);
		mAnimJump->setEnabled(true);
		mAnimJump->addTime(Ogre::Real(FPS));
		return false;
	}
}

void Player::animCrouchDown(Ogre::AnimationState* mAnimCrouch)
{
	Ogre::Real time = mAnimCrouch->getTimePosition();;
	Ogre::Real maxTime = mAnimCrouch->getLength();
	//AGACHARSE
	if(time < (maxTime/2))
	{
		mAnimCrouch->setLoop(false);
		mAnimCrouch->setEnabled(true);
		mAnimCrouch->addTime(Ogre::Real(FPS));
	}
}

void Player::animCrouchUp(Ogre::AnimationState* mAnimCrouch)
{
	Ogre::Real time = mAnimCrouch->getTimePosition();
	Ogre::Real maxTime = mAnimCrouch->getLength();
	//Si la animacion se encuentra en el estado inicial o en el final, habrá terminado y se reinicia
	if ( time == 0 || (time >= maxTime) )
	{
		mAnimCrouch->setTimePosition(0);
		mAnimCrouch->setEnabled(false);
		m_crouchUp = false;
	}
	//Si la animacion no ha terminado el recorrido, deshacerla
	else if (time < (maxTime/2))
	{
		mAnimCrouch->setLoop(false);
		mAnimCrouch->setEnabled(true);
		mAnimCrouch->setTimePosition(Ogre::Real(time-FPS));
	}
	//Si la animación ha terminado el recorrido, continuar la animacion
	else if(time > (maxTime/2))
	{
		mAnimCrouch->setLoop(false);
		mAnimCrouch->setEnabled(true);
		mAnimCrouch->addTime(Ogre::Real(FPS));
	}
}




//------------------------------------------------------------
//Controla la caida posterior al salto
//------------------------------------------------------------
bool Player::fallManager()
{
	//Esquinas
	Ogre::Vector3 nearLeft = m_sceneObject->mNode._getWorldAABB().getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::Vector3 nearRight = m_sceneObject->mNode._getWorldAABB().getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
	//5% desde la esquina hacia el interior descartado
	Ogre::Real sizeX = m_sceneObject->mNode._getWorldAABB().getSize().x;
	nearLeft.x = nearLeft.x + sizeX*0.05;
	nearRight.x = nearRight.x - sizeX*0.05;
	//Punto central
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
		dist1 = obj1->m_sceneObject->mNode._getWorldAABB().squaredDistance(nearLeft);
	if(obj2 != 0)
		dist2 = obj2->m_sceneObject->mNode._getWorldAABB().squaredDistance(nearRight);
	if(obj3 != 0)
		dist3 = obj3->m_sceneObject->mNode._getWorldAABB().squaredDistance(center);

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
			if(itr->movable->getParentNode()->getName() == gObjects[x]->m_sceneObject->mNode.getName())
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