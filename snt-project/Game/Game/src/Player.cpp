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

	//Requiere esto ya que, tras cada iteración, si no hay movimiento lineal se bloquea el rigidbody
	m_sceneObject->mRigidBody.setActivationState(DISABLE_DEACTIVATION);

	m_fall = false;
	m_jump = false;
	m_jumpCount = 0;
	m_maxNumJump = 2;

	m_crouchDown = false;
	m_crouchUp = false;
	m_run = false;

	m_moveX = 2;
	m_moveY = 20;

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
	//SALTOS (Movimiento en eje +Y)
	//---------------------------------------------------------------------

	//Controlar la caida para saber cuando acaba
	if(m_fall)
		fallManager();
	else
		m_jumpCount = 0;		

	//Se permite saltar un numero limitado de veces (m_jumpCount) controlado en KeyboardMouse
	if (m_jump)
	{
		//Aplicamos el salto (DEPENDE DE LA MASA)
		m_sceneObject->mRigidBody.applyCentralImpulse(btVector3(0, m_direction.y*m_moveY, 0));
		//Acumulamos los saltos
		m_jumpCount += 1;
		//Reset de los valores (obliga a pulsar de nuevo la tecla SPACE para saltar de nuevo)
		m_jump = false;
		m_direction.y = 0;
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

	//Camara: sigue al jugador
	Ogre::Vector3 pos = m_sceneObject->mNode.getPosition();
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

	//Si estamos a una distancia aceptable
	if (distance < m_tkDistance)
	{
		//Movemos el objeto segun el raton
		m_catchObj->update();
	}
	else
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
	Ogre::Real time;
	Ogre::Real maxTime;

	//***********************************************************************
	mAnimationState = m_sceneObject->mEntity.getAnimationState("Jump");
	time = mAnimationState->getTimePosition();
	maxTime = mAnimationState->getLength();
	//SALTAR
	if (m_jumpCount > 0 && time < (maxTime*0.75))
	{
		mAnimationState->setLoop(false);
		mAnimationState->setEnabled(true);
		mAnimationState->addTime(FPS);
	}
	//CAER
	if(!m_fall && time >= maxTime)
		mAnimationState->setTimePosition(0);
	else if (!m_fall && time > 0)
	{
		mAnimationState->setLoop(false);
		mAnimationState->setEnabled(true);
		mAnimationState->addTime(FPS);
	}
	//***********************************************************************
	//AGACHARSE
	if (m_crouchDown)
	{
		mAnimationState = m_sceneObject->mEntity.getAnimationState("Crouch");
		time = mAnimationState->getTimePosition();
		maxTime = mAnimationState->getLength();
		if(time < (maxTime/2))
		{
			mAnimationState->setLoop(false);
			mAnimationState->setEnabled(true);
			mAnimationState->addTime(FPS);
		}
	}
	//LEVANTARSE
	if (m_crouchUp)
	{
		mAnimationState = m_sceneObject->mEntity.getAnimationState("Crouch");
		time = mAnimationState->getTimePosition();
		maxTime = mAnimationState->getLength();
		//Si la animacion se encuentra en el estado inicial o en el final, habrá terminado y se reinicia
		if ( time == 0 || (time >= maxTime) )
		{
			mAnimationState->setTimePosition(0);
			m_crouchUp = false;
		}
		//Si la animacion no ha terminado el recorrido, deshacerla
		if (time < (maxTime/2))
		{
			mAnimationState->setLoop(false);
			mAnimationState->setEnabled(true);
			mAnimationState->setTimePosition(time-FPS);
		}
		//Si la animación ha terminado el recorrido, continuar la animacion
		else if(time > (maxTime/2))
		{
			mAnimationState->setLoop(false);
			mAnimationState->setEnabled(true);
			mAnimationState->addTime(FPS);
		}
	}
	//***********************************************************************
}

//------------------------------------------------------------
//Controla la caida posterior al salto
//------------------------------------------------------------
void Player::fallManager()
{
	Ogre::Vector3 nearLeft = m_sceneObject->mNode._getWorldAABB().getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::Vector3 nearRight = m_sceneObject->mNode._getWorldAABB().getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
	Ogre::Vector3 center = (nearRight + nearLeft ) / 2;

	//Comprobamos si durante la caida colisionará con algun objeto
	Object* obj1 = rayFromPoint(nearLeft, Ogre::Vector3::NEGATIVE_UNIT_Y);
	Object* obj2 = rayFromPoint(nearRight, Ogre::Vector3::NEGATIVE_UNIT_Y);
	Object* obj3 = rayFromPoint(center, Ogre::Vector3::NEGATIVE_UNIT_Y);

	//Obtenemos las distancias
	Ogre::Real dist1 = 5;
	Ogre::Real dist2 = 5;
	Ogre::Real dist3 = 5;

	if(obj1 != 0)
		dist1 = obj1->m_sceneObject->mNode._getWorldAABB().squaredDistance(nearLeft);
	if(obj2 != 0)
		dist2 = obj2->m_sceneObject->mNode._getWorldAABB().squaredDistance(nearRight);
	if(obj3 != 0)
		dist3 = obj3->m_sceneObject->mNode._getWorldAABB().squaredDistance(center);

	//Comprobamos la distancia entre el objeto y el punto, finalizando la caida cuando sea correcto
	Ogre::Real minDist = std::min(dist1, std::min(dist2, dist3));
	if(minDist < 5)
		m_fall = false;
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
	mRaySceneQuery->setSortByDistance(true, 1);

	Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	//OPTIMIZABLE con inCameraFrustrumObjects

	if(itr != result.end())
		for(int x = 0; x < gObjects.size(); x++)
			if(itr->movable->getParentNode()->getName() == gObjects[x]->m_sceneObject->mNode.getName())
				obj = gObjects[x];

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