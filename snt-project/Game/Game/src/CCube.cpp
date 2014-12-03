#include "stdafx.h"
#include "CCube.h"

//---------------------------------------------------------------------------
CCube::CCube(void)
{
	//Posicion
	m_posX = 0.0;
	m_posY = -80.0;
	m_posZ = 0.0;
	//Rotacion
	m_pitch = 0.0;
	m_yaw = 0.0;
	m_roll = 0.0;
	//Escalar
	m_scaleX = 1.0;
	m_scaleY = 1.0;
	m_scaleZ = 1.0;
	//Colisionable por defecto
	m_collision = true;
	//Velocidad por defecto
	m_Move = 125;
}
//---------------------------------------------------------------------------
CCube::~CCube(void)
{
}

//---------------------------------------------------------------------------
void CCube::createCube(Ogre::SceneManager* mSceneMgr, std::string id)
{
	//Identificadores
	std::string cubeName = "cube" + id;
	std::string cubeNodeName = "cubeNode" + id;

	//Creacion (entidad y nodo)
	//-----------Cubo creado de forma automatica---------------
	//Ogre::Entity* cube = mSceneMgr->createEntity(cubeName, "cube.mesh");
	//-----------Cubo creado de forma manual-------------------
	Ogre::Entity* cube = mSceneMgr->createEntity(cubeName, "ManualObjectCube");
	cubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(cubeNodeName);
	cubeNode->attachObject(cube);

	//Posicionamiento y escalado (por defecto)
	cubeNode->setPosition(Ogre::Vector3(0.0, -80.0, 0.0));
	cubeNode->scale(Ogre::Vector3(1.0, 1.0, 1.0));
}

//---------------------------------------------------------------------------
//--Posicionamiento
//---------------------------------------------------------------------------
void CCube::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
	cubeNode->setPosition(Ogre::Vector3(m_posX, m_posY, m_posZ));
}

//---------------------------------------------------------------------------
//--Escalado
//---------------------------------------------------------------------------
void CCube::setScale(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_scaleX = x;
	m_scaleY = y;
	m_scaleZ = z;
	cubeNode->scale(Ogre::Vector3(m_scaleX, m_scaleY, m_scaleZ));
}

//---------------------------------------------------------------------------
//--Rotacion
//---------------------------------------------------------------------------
void CCube::setRotation(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_pitch = x;
	m_yaw = y;
	m_roll = z;

	if (m_pitch != 0.0)
	{
		cubeNode->rotate(Ogre::Vector3::UNIT_X,Ogre::Degree(m_pitch));
	}
	if (m_yaw != 0.0)
	{
		cubeNode->rotate(Ogre::Vector3::UNIT_Y,Ogre::Degree(m_yaw));
	}
	if (m_roll != 0.0)
	{
		cubeNode->rotate(Ogre::Vector3::UNIT_Z,Ogre::Degree(m_roll));
	}
}

Ogre::SceneNode* CCube::getAssociatedNode(void)
{
	return cubeNode;
}