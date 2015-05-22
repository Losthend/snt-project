#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/Global.h"
#include "../include/FrameRate.h"
#include "../include/SceneObject.h"

/* 
	Objeto de:
		- Tipo 1: estaticos
		- Tipo 2: dinamico, el jugador puede agarrarlo
		- Tipo 3: dinamico, se mueve automaticamente, te mata con colision en los lados y lo matas cayendo encima
*/

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
Object::Object(int objType, SceneObject* sceneObject)
{
	m_sceneObject = sceneObject;
	
	m_sceneObject->mRigidBody.setActivationState(DISABLE_DEACTIVATION);

	FPS = getframeLength();

	m_objType = objType;

	m_moveX = 2;
	m_moveY = 2;

	m_speed = 20;

	m_direction = Ogre::Vector3::ZERO;
}

//---------------------------------------------------------------------------
//Destructor
//---------------------------------------------------------------------------
Object::~Object(void)
{
	
}

//---------------------------------------------------------------------------
//Metodo para controlar las colisiones del objeto
//---------------------------------------------------------------------------
void Object::update()
{
	//Objetos Tipo 2: pueden agarrarse
	if (m_objType == 2)
	{
		//Raycasting para obtener las coordenadas del raton
		OIS::MouseState ms = gMouse->getMouseState();
		Ogre::Ray ray = gCamera->getCameraToViewportRay(ms.X.abs/(float)gWindow->getWidth(),ms.Y.abs/(float)gWindow->getHeight()); 

		//Coordenadas del raton
		Ogre::Vector3 mouseCoord = ray.getPoint(gCamera->getPosition().z);

		//Coordenadas del objeto a mover
		Ogre::Vector3 objCoord = m_sceneObject->mNode.getPosition();

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
		m_sceneObject->mRigidBody.translate(btVector3(x, y, 0));
	}
}