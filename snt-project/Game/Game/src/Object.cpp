#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "Object.h"
#include "Player.h"
#include "Global.h"
#include "Collision.h"
#include "FrameRate.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
Object::Object(std::string id, int objType)
{
	//Inicializamos variables

	//Obtenemos la duracion de cada frame
	FPS = getframeLength();

	//Tipo de objeto
	m_objType = objType;

	//Vector de movimiento/direccion
	m_direction = Ogre::Vector3::ZERO;

	//Velocidad por defecto
	m_moveX = 125;
	m_moveY = 300;

	//Gravedad
	m_gravity = 9.8;

	//Identificadores
	m_entName = "objEnt_" + id;
	m_nodeName = "objNode_" + id;
	
	//Creacion (entidad y nodo)
	//-----------Cubo creado de forma manual-------------------
	m_entity = gSceneMgr->createEntity(m_entName, "ManualObjectCube");
	m_node = gSceneMgr->getRootSceneNode()->createChildSceneNode(m_nodeName);
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

//---------------------------------------------------------------------------
//Metodo para aplicar gravedad a los objetos dinamicos
//---------------------------------------------------------------------------
void Object::gravity()
{
	Object* obj = gPlayer->m_catchObj;
	//Si el jugador no ha cogido ningun objeto || Si ha cogido un objeto pero NO es el mismo que este
	if ( obj == 0 || (obj != 0 && obj->m_nodeName != m_nodeName))
	{
		//Probamos a aplicar la gravedad
		m_direction.y = m_direction.y - m_gravity;
		Ogre::Vector3 vDistance = m_direction * FPS;	
		bool collision = update(vDistance);
		//Si hay colision detenemos el efecto de la gravedad
		if (collision)
		{
			m_direction.y = 0;
		}
	}
}


//---------------------------------------------------------------------------
//Metodo para mover el objeto si el jugador lo esta agarrando
//---------------------------------------------------------------------------
bool Object::update(Ogre::Vector3 vDistance)
{
	//Simulamos ese movimiento
	std::vector<Ogre::Vector3> vSimCoords = simulateOccupiedCoords(m_node, vDistance);
	//Comprobamos si hay colisiones
	Object* obj = testCollisionAABB(vSimCoords, m_nodeName);
	bool playerCollision = testCollisionWithSingleNode(vSimCoords, gPlayer->node);
	//Si NO hay colision, se mueve
	if (obj == 0 && !playerCollision)
	{
		m_node->translate(vDistance, Ogre::Node::TS_LOCAL);
		return false;
	}
	return true;
}