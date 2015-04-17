#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/Global.h"
#include "../include/FrameRate.h"
//Para el tratamiento de colisiones
#include "../include/CollisionManagement.h"
#include "../include/SceneObject.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
Object::Object(int objType, SceneObject* sceneObject)
{
	m_SceneObject = sceneObject;

	FPS = getframeLength();

	m_objType = objType;

	m_moveX = 125;
	m_moveY = 300;

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
int Object::update(Ogre::Vector3 vDistance)
{
	return 0;
}

//---------------------------------------------------------------------------
//Metodo para aplicar movimiento a los enemigos dinamicos
//---------------------------------------------------------------------------
void Object::move()
{

}