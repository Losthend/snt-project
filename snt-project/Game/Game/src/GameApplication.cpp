#include "../include/stdafx.h"
#include "../include/GameApplication.h"

//Acceso a las variables globales
#include "../include/Global.h"
//Acceso a Object y Player
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/dotScene.h"
//Para el uso de fisicas
#include "../include/PhysicsManager.h"
//Para la creacion de SceneObjects
#include "../include/SceneObject.h"

//---------------------------------------------------------------------------
//Constructor del juego (Game)
//---------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
	//Variable global del PhysicsManager para el control de fisicas y colisiones de Bullet
	mPhysics = new PhysicsManager();
	gPhysics = mPhysics;

	//Al crear el juego se crea el personaje (solo uno)
	gPlayer = new Player();

	//Tambien el "mundo"
	createScene();
}
//---------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
}

//---------------------------------------------------------------------------
//Establece aquello que se muestra en la escena y sus caracteristicas, incluida la iluminacion
//---------------------------------------------------------------------------
void GameApplication::createScene(void)
{
	//--------------------------------------------------------------------------------
	//Al crear una escena vaciamos el posible contenido de vObjects (vector con los cubos creados en la escena, nodo incluido)
	vObjects.clear();
	//--------------------------------------------------------------------------------

	//parseDotScene("fabrica00.scene", "Nuestros", gSceneMgr);

	
	//--------------------------------------------------------------------------------
	//CREACION DE OBJETOS MEDIANTE BULLET
	//-------------------------------------------------------------------------------

	btCollisionShape &planeShape = mPhysics->createBoxShape(200, 1.0f, 200);
	btCollisionShape &cubeShape = mPhysics->createBoxShape(10, 10, 10);
	btCollisionShape &plane2Shape = mPhysics->createBoxShape(50, 1.0f, 50);

	
	SceneObject* obj1 = gPhysics->createBoxObject("cube1", Ogre::Vector3(10, 10, 10), Ogre::Vector3(0, 100, 0), 1, cubeShape);
	SceneObject* obj2 = gPhysics->createBoxObject("cube2", Ogre::Vector3(10, 10, 10), Ogre::Vector3(0, 80, 0), 5, cubeShape);
    SceneObject* obj3 = gPhysics->createBoxObject("cube3", Ogre::Vector3(10, 10, 10), Ogre::Vector3(0, 130, 0), 5, cubeShape);
    SceneObject* obj4 = gPhysics->createBoxObject("cube4", Ogre::Vector3(10, 10, 10), Ogre::Vector3(30, 150, 0), 10, cubeShape);
    SceneObject* obj5 = gPhysics->createBoxObject("plane2", Ogre::Vector3(50, 1, 50), Ogre::Vector3(20, 110, 0), 1, plane2Shape);
 
	SceneObject* obj6 = gPhysics->createBoxObject("floor", Ogre::Vector3(200, 1, 200), Ogre::Vector3(0, -50, 0), 0, planeShape);


	//Guardado del cuerpo rigido en gObjects (vector global)
	gObjects.push_back(obj1);
	gObjects.push_back(obj2);
	gObjects.push_back(obj3);
	gObjects.push_back(obj4);
	gObjects.push_back(obj5);
	gObjects.push_back(obj6);

	//-------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------
	

	//Posicionamos la camara en la posicion que nos interesa (donde esta el jugador)
	Ogre::Vector3 playerPos = gPlayer->node->getPosition();
	gCamera->setPosition(playerPos.x, playerPos.y, 450.0);

	//Luz ambiente y fondo
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
}














/*
//CODIGO ANTERIOR PARA CARGA DE OBJETOS

//Suelo
	Object* obj1 = new Object("1", 1, "ManualObjectCube");
	obj1->m_node->scale(30, 1.0, 1.0);
	obj1->m_node->setPosition(80.0, -125.0, 0.0);

	//Pared izquierda
	Object* obj2 = new Object("2", 1, "ManualObjectCube");
	obj2->m_node->scale(1.0, 11.0, 1.0);
	obj2->m_node->setPosition(-225.0, -25.0, 0.0);

	//Pared derecha
	Object* obj3 = new Object("3", 1, "ManualObjectCube");
	obj3->m_node->scale(1.0, 11.0, 1.0);
	obj3->m_node->setPosition(375.0, -25.0, 0.0);

	//Techo 1
	Object* obj4 = new Object("4", 1,"ManualObjectCube");
	obj4->m_node->scale(5.0, 1.0, 1.0);
	obj4->m_node->setPosition(-120.0, -85.0, 0.0);

	//Techo 2
	Object* obj5 = new Object("5", 1,"ManualObjectCube");
	obj5->m_node->scale(5.0, 1.0, 1.0);
	obj5->m_node->setPosition(120.0, -80.0, 0.0);


	//Tamaño de re-escalado
	Ogre::Real value;

	//Objeto dinamico grande
	Object* obj6 = new Object("6", 2, "ManualObjectCube");
	value = Ogre::Real(1.4);
	obj6->m_node->scale(value, value, value);
	obj6->m_node->setPosition(40.0, -80.0, 0.0);
	//obj6->m_node->setPosition(200.0, -80.0, 0.0);

	//Objeto dinamico pequeño
	Object* obj7 = new Object("7", 2, "ManualObjectCube");
	value = Ogre::Real(0.7);
	obj7->m_node->scale(value, value, value);
	obj7->m_node->setPosition(-40.0, -80.0, 0.0);
	//obj7->m_node->setPosition(-200.0, -80.0, 0.0);

	//Objeto dinamico enemigo
	Object* obj8 = new Object("8", 3, "ManualObjectCube");
	obj8->m_node->setPosition(350.0, -80.0, 0.0);
	
	//--------------------------------------------------------------------------------
	//Almacenamos los objetos de la clase Object en el array global de Objects
	gObjects.push_back(obj1);
	gObjects.push_back(obj2);
	vObjects.push_back(obj3);
	vObjects.push_back(obj4);
	vObjects.push_back(obj5);
	vObjects.push_back(obj6);
	vObjects.push_back(obj7);
	vObjects.push_back(obj8);
	//--------------------------------------------------------------------------------

	*/