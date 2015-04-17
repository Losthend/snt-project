#include "../include/stdafx.h"

#include "../include/Player.h"

#include "../include/Object.h"
#include "../include/Global.h"
#include "../include/FrameRate.h"
#include "../include/CollisionManagement.h"
#include "../include/SceneObject.h"

//---------------------------------------------------------------------------
Player::Player(SceneObject* sceneObject)
{
	FPS = getframeLength();

	m_sceneObject = sceneObject;
	//m_sceneObject->mRigidBody.setActivationState(DISABLE_DEACTIVATION);

	m_jump = false;
	m_jumpCount = 0;
	m_maxNumJump = 2;

	m_crouchDown = false;
	m_run = false;

	m_moveX = 2;
	m_moveY = 20;

	m_Speed = 20;

	m_direction = Ogre::Vector3::ZERO;
}
//---------------------------------------------------------------------------
Player::~Player(void)
{

}

//---------------------------------------------------------------------------
//--Control, mediante teclado, del jugador
//---------------------------------------------------------------------------
void Player::update()
{
	//Obtenemos la velocidad lineal del rigidbody
	btVector3 linearVelocity = m_sceneObject->mRigidBody.getLinearVelocity();
	btScalar velX = linearVelocity.x();
	btScalar velY = linearVelocity.y();

	//-------------------------------------------------------------------
	//SALTOS (Movimiento en eje +Y)
	//---------------------------------------------------------------------

	//2ª condicion: alcanzado el numero maximo de saltos, se permitira saltar cuando lleges dejes de caer/saltar
	if (m_jumpCount >= m_maxNumJump && velY < 0.001 && velY > -0.001)
	{
		m_jumpCount = 0;
	}

	//1ª condicion: se permite saltar un numero limitado de veces (m_jumpCount) controlado en KeyboardMouse
	if (m_jump == true)
	{
		m_sceneObject->mRigidBody.activate(true);
		//Aplicamos el salto
		m_sceneObject->mRigidBody.applyCentralImpulse(btVector3(0, m_direction.y*m_moveY, 0));
		//Acumulamos los saltos
		m_jumpCount += 1;
		//Reset de los valores (obliga a pulsar de nuevo la tecla SPACE para saltar)
		m_jump = false;
		m_direction.y = 0;
	}

	//-------------------------------------------------------------------
	//MOVIMIENTO EN EL EJE X
	//--------------------------------------------------------------------

	//Limitamos la velocidad de movimiento (LinearVelocity) en X segun la velocidad del jugador (m_speed)
	if ( (m_direction.x > 0 &&  velX < m_Speed) || (m_direction.x < 0 &&  velX > -m_Speed))
	{
		//Requiere activacion, dado que tras cada iteración, si no hay movimiento lineal, se bloquea el rigidbody
		m_sceneObject->mRigidBody.activate(true);
		//Movimiento mediante impulse (mas realista)
		m_sceneObject->mRigidBody.applyCentralImpulse(btVector3(m_direction.x*m_moveX, 0, 0));
	}
}


/*
Ogre::String mValue = "Frase";
OutputDebugString(mValue.c_str());
OutputDebugString("\n");

int num = 1;
OutputDebugString(Ogre::StringConverter::toString(num).c_str());
OutputDebugString("\n");
*/