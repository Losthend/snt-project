#include "../include/stdafx.h"
#include "../include/GameApplication.h"

//Acceso a las variables globales
#include "../include/Global.h"
//Acceso a CCube y Player
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/dotScene.h"

//---------------------------------------------------------------------------
//Constructor del juego (Game)
//---------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
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
	vObjects.push_back(obj1);
	vObjects.push_back(obj2);
	vObjects.push_back(obj3);
	vObjects.push_back(obj4);
	vObjects.push_back(obj5);
	vObjects.push_back(obj6);
	vObjects.push_back(obj7);
	vObjects.push_back(obj8);
	//--------------------------------------------------------------------------------
	

	//Posicionamos la camara en la posicion que nos interesa (donde esta el jugador)
	Ogre::Vector3 playerPos = gPlayer->node->getPosition();
	gCamera->setPosition(playerPos.x, playerPos.y, 450.0);

	//Luz ambiente y fondo
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
}