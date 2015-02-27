#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/Global.h"
#include "../include/Collision.h"
#include "../include/FrameRate.h"
#include "../include/funcionesGenerales.h"
//Para el tratamiento de colisiones
#include "../include/CollisionManagement.h"

//---------------------------------------------------------------------------
//Constructor
//---------------------------------------------------------------------------
Object::Object(std::string id, int objType, std::string EntityMeshFilename)
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

	/*m_entity = gSceneMgr->createEntity(m_entName,EntityMeshFilename);
	m_node = gSceneMgr->getRootSceneNode()->createChildSceneNode(m_nodeName);
	m_node->attachObject(m_entity);*/

	//Posicionamiento y escalado (por defecto)
	m_node->setPosition(0.0, 0.0, 0.0);
	m_node->scale(1.0, 1.0, 1.0);

	//lista de puntos entre los que tiene que patrullar 8en caso de ser de tipo 3)
    //m_WalkList.push_back(Ogre::Vector3(100.0f,  0.0f,  0.0f ));
    //m_WalkList.push_back(Ogre::Vector3(0.0f,  0.0f, 0.0f));
	m_WalkList.push_back(Ogre::Vector3(350.0f,  0.0f,  0.0f ));
    m_WalkList.push_back(Ogre::Vector3(250.0f,  0.0f, 0.0f));

	//apuntamos al primer punto de la ruta
	targetPosition = m_WalkList[0];
}

//---------------------------------------------------------------------------
//Destructor
//---------------------------------------------------------------------------
Object::~Object(void)
{
	//Eliminamos el objeto del vector global (el unico lugar desde el que pueden acceder a el)
	for(unsigned i = 0; i < vObjects.size(); i++)
		if(vObjects[i]->m_nodeName == m_nodeName)
			vObjects.erase(vObjects.begin()+i);

	//Eliminamos definitivamente la entidad y el nodo
	m_node->detachObject(m_entity);
	gSceneMgr->destroyEntity(m_entity);
	gSceneMgr->destroySceneNode(m_node);
}

//---------------------------------------------------------------------------
//Metodo para controlar las colisiones del objeto
//---------------------------------------------------------------------------
int Object::update(Ogre::Vector3 vDistance)
{

	//========================PRE-VARIABLES================================

	//Definimos variables para los procesos posteriores
	std::vector<Ogre::Vector3> vCoords;
	bool playerCollisionX = false;
	bool playerCollisionY = false;
	Object* objX = 0;
	Object* objY = 0;

	//========================PRE-MOVIMIENTO=============================

	Object* obj = gPlayer->m_catchObj;

	//Si el jugador no ha cogido ningun objeto || si ha cogido un objeto pero NO es el mismo que este... 
	//El jugador no ha interactuado con el objeto, por lo que realizara sus tareas normales basadas en "m_direction"
	if ( obj == 0 || (obj != 0 && obj->m_nodeName != m_nodeName))
	{
		m_direction.y = Ogre::Real(m_direction.y - m_gravity);
		vDistance.y = Ogre::Real(m_direction.y * FPS);
		vDistance.x = Ogre::Real(m_direction.x * FPS);
	}

	//========================MOVIMIENTO_X================================

	//MOVIMIENTO EN EL EJE X, solo si hay movimiento en X
	if (vDistance.x != 0)
	{
		Ogre::Vector3 tempMoveX = Ogre::Vector3(vDistance.x, 0, 0);

		//Simulamos el movimiento
		vCoords = simulateOccupiedCoords(m_node, tempMoveX);
		objX = testCollisionAABB(vCoords, m_nodeName);

		playerCollisionX = testCollisionWithSingleNode(vCoords, gPlayer->node);

		//Si hay colision
		if (objX != 0 || playerCollisionX)
		{	
			vDistance.x = 0;
		}
	}

	//========================MOVIMIENTO_Y================================

	Ogre::Vector3 tempMoveY = Ogre::Vector3(0, vDistance.y, 0);

	//Simulamos el movimiento
	vCoords = simulateOccupiedCoords(m_node, tempMoveY);
	objY = testCollisionAABB(vCoords, m_nodeName);

	playerCollisionY = testCollisionWithSingleNode(vCoords, gPlayer->node);

	//Si hay colision
	if (objY != 0 || playerCollisionY)
	{
		vDistance.y = 0;
	}

	//========================MOVIMIENTO FINAL================================

	if (vDistance.x != 0 || vDistance.y != 0)
		m_node->translate(vDistance, Ogre::Node::TS_LOCAL);

	//========================RESETS================================

	//Reset del movimiento en Y
	if (objY != 0 && m_direction.y <= 0)
	{
		m_direction.y = 0;
	}

	//Reset del movimiento en X
	m_direction.x = 0;	

	//=========================RETURN======================================
	
	//Si no se ha realizado movimiento en Y
	if (vDistance.y == 0)
		//No hay colisiones en Y
		if (objX != 0)
			return 1;
		else if (playerCollisionX)
			return 2;
		else
			return 0;
	//Si no se ha realizado movimiento en X
	else if (vDistance.x == 0)
	{
		//No hay colision en X
		if (objY != 0)
			return 3;
		else if (playerCollisionY)
			return 4;
		else
			return 0;
	}
	else
		//Nos hemos movido tanto en X como en Y, ergo no hay colisiones
		return 0;
}

//---------------------------------------------------------------------------
//Metodo para aplicar movimiento a los enemigos dinamicos
//---------------------------------------------------------------------------
void Object::move(){

	//Si aun no hemos alcanzado la posicion objetivo, la mantenemos como objetivo y nos movemos hacia ella
	if(m_node->getPosition().x < targetPosition.x)
	{
		targetPosition = m_WalkList[0];
		m_direction.x = m_moveX;
	}
	//Si hemos alcanzado la posicion objetivo, cambiamos de objetivo y nos movemos hacia ella
	else
	{
		targetPosition = m_WalkList[1];
		m_direction.x = -m_moveX;
	}

	//Control de colision
	int collision = update(Ogre::Vector3::ZERO);

	//=================================================================================
	//Como actuar ante la colision

	//Si colisionas con un objeto o con el jugador en X, inviertes la direccion
	if (collision == 1 || collision == 2)
	{
		//Invertimos la direccion de movimiento
		if (targetPosition == m_WalkList[0])
			targetPosition = m_WalkList[1];
		else
			targetPosition = m_WalkList[0];
	}

	//Si has colisionado en X con el jugador, matas al jugador
	if (collision == 2)
		collisionManagementX(this);

	//Si has colisionado en Y con el jugador, mueres
	if (collision == 4)
		collisionManagementY(this);		
}