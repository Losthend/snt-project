#include "stdafx.h"
#include "GameApplication.h"

//Acceso a las variables globales
#include "Global.h"
//Acceso a CCube y Player
#include "Object.h"
#include "Player.h"

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

	//Suelo
	Object* obj1 = new Object("1", 1);
	obj1->m_node->scale(14, 1.0, 1.0);
	obj1->m_node->setPosition(0.0, -125.0, 0.0);

	//Pared izquierda
	Object* obj2 = new Object("2", 1);
	obj2->m_node->scale(1.0, 11.0, 1.0);
	obj2->m_node->setPosition(-150.0, 0.0, 0.0);

	//Pared derecha
	Object* obj3 = new Object("3", 1);
	obj3->m_node->scale(1.0, 11.0, 1.0);
	obj3->m_node->setPosition(150.0, 0.0, 0.0);

	//Techo
	Object* obj4 = new Object("4", 1);
	obj4->m_node->scale(14.0, 1.0, 1.0);
	obj4->m_node->setPosition(0.0, 125.0, 0.0);

	//Objeto dinamico
	Object* obj5 = new Object("5", 2);
	obj5->m_node->setPosition(50.0, 0.0, 0.0);

	//--------------------------------------------------------------------------------
	//Almacenamos los objetos de la clase CCube en el array global de Objects
	vObjects.push_back(obj1);
	vObjects.push_back(obj2);
	vObjects.push_back(obj3);
	vObjects.push_back(obj4);
	vObjects.push_back(obj5);
	//--------------------------------------------------------------------------------

	//Luz ambiente y fondo
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
}