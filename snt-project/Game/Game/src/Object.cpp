#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "Object.h"
#include "Global.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
Object::Object(std::string id, int objType)
{
	//Inicializamos variables

	//Tipo de objeto
	m_objType = objType;

	//Colisionable por defecto
	m_collision = true;

	//Velocidad por defecto
	m_moveX = 125;
	m_moveY = 125;

	//Identificadores
	std::string entName = "objEnt_" + id;
	std::string nodeName = "objNode_" + id;
	
	//Creacion (entidad y nodo)
	//-----------Cubo creado de forma manual-------------------
	m_entity = gSceneMgr->createEntity(entName, "ManualObjectCube");
	m_node = gSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
	m_node->attachObject(m_entity);

	//Posicionamiento y escalado (por defecto)
	m_node->setPosition(0.0, 0.0, 0.0);
	m_node->scale(1.0, 1.0, 1.0);
}

//---------------------------------------------------------------------------
//Destructor
//---------------------------------------------------------------------------
Object::~Object(void)
{
}