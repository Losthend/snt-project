#include "stdafx.h"
#include "CCube.h"

//---------------------------------------------------------------------------
CCube::CCube(std::string id)
{
	//Inicializamos variables

		//Posicion
		m_posX = 0.0;
		m_posY = 0.0;
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

	//Creamos el cubo con su entidad y nodo correspondiente

		//Identificadores
		std::string entName = "cube" + id;
		std::string nodeName = "cubeNode" + id;
		
		//Creacion (entidad y nodo)
		//-----------Cubo creado de forma automatica---------------
		//Ogre::Entity* cube = mSceneMgr->createEntity(cubeName, "cube.mesh");
		//-----------Cubo creado de forma manual-------------------
		entity = gSceneMgr->createEntity(entName, "ManualObjectCube");
		node = gSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
		node->attachObject(entity);

		//Posicionamiento y escalado (por defecto)
		node->setPosition(Ogre::Vector3(m_posX, m_posY, m_posZ));
		node->scale(Ogre::Vector3(m_scaleX, m_scaleY, m_scaleZ));
}
//---------------------------------------------------------------------------
CCube::~CCube(void)
{
}

//---------------------------------------------------------------------------
//--Posicionamiento
//---------------------------------------------------------------------------
void CCube::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
	node->setPosition(Ogre::Vector3(m_posX, m_posY, m_posZ));
}

//---------------------------------------------------------------------------
//--Escalado
//---------------------------------------------------------------------------
void CCube::setScale(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_scaleX = x;
	m_scaleY = y;
	m_scaleZ = z;
	node->scale(Ogre::Vector3(m_scaleX, m_scaleY, m_scaleZ));
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
		node->rotate(Ogre::Vector3::UNIT_X,Ogre::Degree(m_pitch));
	}
	if (m_yaw != 0.0)
	{
		node->rotate(Ogre::Vector3::UNIT_Y,Ogre::Degree(m_yaw));
	}
	if (m_roll != 0.0)
	{
		node->rotate(Ogre::Vector3::UNIT_Z,Ogre::Degree(m_roll));
	}
}

Ogre::SceneNode* CCube::getAssociatedNode(void)
{
	return node;
}