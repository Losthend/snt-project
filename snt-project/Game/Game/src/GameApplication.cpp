#include "stdafx.h"
#include "GameApplication.h"

//Acceso a las variables globales
#include "Global.h"
//Acceso a CCube y CPlayer
#include "CCube.h"
#include "CPlayer.h"

//---------------------------------------------------------------------------
//Constructor del juego (Game)
//---------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
	//Al crear el juego se crea el personaje (solo uno)
	gPlayer = new CPlayer();
	//Tambien el "mundo"
	createScene();

	//Se asocia el nodo del jugador a la camara
	//NOTA:Las colisiones no funcionan al hacer esto...
	//gPlayer->node->attachObject(gCamera);
	//NOTA: necesario para que no se invierta el teclado tras asociarlo
	//gCamera->roll(Ogre::Degree(180));
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
	//Al crear una escena vaciamos el posible contenido de vCubes (vector con los cubos creados en la escena, nodo incluido)
	vCubes.clear();
	//--------------------------------------------------------------------------------

	/*
	Ogre::Entity* mEntidadPrubas = gSceneMgr->createEntity("escenrioPruebas","bugSPARK.mesh");
	Ogre::SceneNode* nodoEscenario = gSceneMgr->getRootSceneNode()->createChildSceneNode("nodoEscenrioPruebas");
	nodoEscenario->attachObject(mEntidadPrubas);
	nodoEscenario->yaw(Ogre::Radian(150));
	nodoEscenario->setPosition(Ogre::Vector3(0.0, -80.0, 0.0));
	nodoEscenario->scale(Ogre::Vector3(10, 10, 10));
	*/

	//Suelo
	CCube* cube2 = new CCube("2");
	cube2->getAssociatedNode()->scale(14, 1.0, 1.0);
	cube2->getAssociatedNode()->setPosition(0.0, -125.0, 0.0);

	//Pared izquierda
	CCube* cube3 = new CCube("3");
	cube3->getAssociatedNode()->scale(1.0, 11.0, 1.0);
	cube3->getAssociatedNode()->setPosition(-150.0, 0.0, 0.0);

	//Pared derecha
	CCube* cube4 = new CCube("4");
	cube4->getAssociatedNode()->scale(1.0, 11.0, 1.0);
	cube4->getAssociatedNode()->setPosition(150.0, 0.0, 0.0);

	//Techo
	CCube* cube5 = new CCube("5");
	cube5->getAssociatedNode()->scale(14.0, 1.0, 1.0);
	cube5->getAssociatedNode()->setPosition(0.0, 125.0, 0.0);

	//--------------------------------------------------------------------------------
	//Almacenamos los objetos de la clase CCube en el array global de Objects
	vCubes.push_back(cube2);
	vCubes.push_back(cube3);
	vCubes.push_back(cube4);
	vCubes.push_back(cube5);
	//--------------------------------------------------------------------------------

	//Luz ambiente y fondo
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
}