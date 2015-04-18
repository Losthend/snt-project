#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/Global.h"
#include "../include/FrameRate.h"
//Para el tratamiento de colisiones
#include "../include/CollisionManagement.h"
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

	FPS = getframeLength();

	m_objType = objType;

	m_moveX = 1;
	m_moveY = 1;

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
		//Activación del objeto
		m_sceneObject->mRigidBody.activate(true);

		//Raycasting para obtener las coordenadas del raton
		OIS::MouseState ms = gMouse->getMouseState();
		Ogre::Ray ray = gCamera->getCameraToViewportRay(ms.X.abs/(float)gWindow->getWidth(),ms.Y.abs/(float)gWindow->getHeight()); 

		//Coordenadas en el mundo del raton
		Ogre::Vector3 coord = ray.getPoint(gCamera->getPosition().z);

		//Posicion absoluta del objeto a mover
		Ogre::Vector3 pos = m_sceneObject->mNode.getPosition();

		//Direccion del impulso
		Ogre::Real x = 0;
		Ogre::Real y = 0;

		//Control X
		if(coord.x > pos.x)
			x = m_moveX;
		else if (coord.x < pos.x)
			x = -m_moveX;

		//Control Y
		if(coord.y > pos.y)
			y = m_moveY;
		else if (coord.y < pos.y)
			y = -m_moveY;

		//Movimiento
		m_sceneObject->mRigidBody.applyCentralImpulse(btVector3(x, y, 0));
	}
}