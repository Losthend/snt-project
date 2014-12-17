#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "CCube.h"
#include "Global.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
CCube::CCube(std::string id)
{
	//Inicializamos variables
	//Colisionable por defecto
	m_collision = true;
	//Velocidad por defecto
	m_moveX = 125;
	m_moveY = 125;

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
	node->setPosition(0.0, 0.0, 0.0);
	node->scale(1.0, 1.0, 1.0);
}
//---------------------------------------------------------------------------
//Destructor
//---------------------------------------------------------------------------
CCube::~CCube(void)
{
}
//---------------------------------------------------------------------------
//Devuelve el nodo asociado al objeto de esta clase
//---------------------------------------------------------------------------
Ogre::SceneNode* CCube::getAssociatedNode(void)
{
	return node;
}