#include "stdafx.h"
#include "GameApplication.h"

//---------------------------------------------------------------------------
//Constructor del juego (Game)
//---------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
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
	/*Ogre::Entity* mEntidadPrubas = gSceneMgr->createEntity("escenrioPruebas","bugSPARK.mesh");
	Ogre::SceneNode* nodoEscenario = gSceneMgr->getRootSceneNode()->createChildSceneNode("nodoEscenrioPruebas");
	nodoEscenario->attachObject(mEntidadPrubas);
	nodoEscenario->yaw(Ogre::Radian(150));
	nodoEscenario->setPosition(Ogre::Vector3(0.0, -80.0, 0.0));
	nodoEscenario->scale(Ogre::Vector3(10, 10, 10));*/
	
	//-----------------------------------------------------------------------------------
	//Generar los Manual Objects
	//Llegado el momento, pasar a BaseApplication o algun metodo/clase de inicializacion de "objetos manuales"
	simpleCube();
	//-----------------------------------------------------------------------------------

	//Personaje
	CCube* cube1 = new CCube();
	cube1->createCube("1");
	//Ogre::SceneNode* nodeCube1 = cube1->getAssociatedNode();
	//nodeCube1->attachObject(app.mCamera);

	//Suelo
	CCube* cube2 = new CCube();
	cube2->createCube("2");
	cube2->setScale(14, 1.0, 1.0);
	cube2->setPosition(0.0, -125.0, 0.0);

	//Pared izquierda
	CCube* cube3 = new CCube();
	cube3->createCube("3");
	cube3->setScale(1.0, 11.0, 1.0);
	cube3->setPosition(-150.0, 0.0, 0.0);

	//Pared derecha
	CCube* cube4 = new CCube();
	cube4->createCube("4");
	cube4->setScale(1.0, 11.0, 1.0);
	cube4->setPosition(150.0, 0.0, 0.0);

	//Techo
	CCube* cube5 = new CCube();
	cube5->createCube("5");
	cube5->setScale(14.0, 1.0, 1.0);
	cube5->setPosition(0.0, 125.0, 0.0);

	//Luz ambiente y fondo
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
}