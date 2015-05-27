#include "../include/stdafx.h"
#include "../include/GameApplication.h"

//Acceso a las variables globales
#include "../include/Global.h"
//Acceso a Object y Player
#include "../include/Object.h"
#include "../include/Player.h"
//Para el uso de fisicas
#include "../include/PhysicsManager.h"
//Para la creacion de SceneObjects
#include "../include/SceneObject.h"

#include "OgreMeshManager.h"
#include "OgreStaticGeometry.h"

//---------------------------------------------------------------------------
//Constructor del juego (Game)
//---------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
	//Variable global del PhysicsManager para el control de fisicas y colisiones de Bullet
	mPhysics = new PhysicsManager();
	gPhysics = mPhysics;

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
	//Creacion del personaje
	SceneObject* player = gPhysics->createBoxObject("Player", Ogre::Vector3(2, 2, 2), Ogre::Vector3(0, 100, 0), 1, 1, "ninja.mesh");
	gPlayer = new Player(player);

	//Creacion de objetos
	
	//SceneObjects: necesarios para el control ejercido por Bullet
	SceneObject* sceneObj1 = gPhysics->createBoxObject("cube1", Ogre::Vector3(30, 30, 30), Ogre::Vector3(-50, 100, 0), 1, 1, "Barrel.mesh");
	SceneObject* sceneObj2 = gPhysics->createBoxObject("cube2", Ogre::Vector3(30, 30, 30), Ogre::Vector3(-50, 80, 0), 1, 1, "Barrel.mesh");
    SceneObject* sceneObj3 = gPhysics->createBoxObject("cube3", Ogre::Vector3(30, 30, 30), Ogre::Vector3(50, 120, 0), 1, 1, "Barrel.mesh");
    SceneObject* sceneObj4 = gPhysics->createBoxObject("cube4", Ogre::Vector3(30, 30, 30), Ogre::Vector3(50, 150, 0), 1, 1, "Barrel.mesh");
    SceneObject* sceneObj5 = gPhysics->createBoxObject("plane", Ogre::Vector3(40, 3, 10), Ogre::Vector3(60, 110, 0), 1, 1, "ManualObjectCube");
	SceneObject* sceneObj6 = gPhysics->createBoxObject("floor", Ogre::Vector3(800, 1, 100), Ogre::Vector3(0, -50, 0), 0, 1, "ManualObjectCube");

	//Almacenamiento del objeto completo en gObjects (vector global)
	gObjects.push_back(new Object(2, sceneObj1));
	gObjects.push_back(new Object(2, sceneObj2));
	gObjects.push_back(new Object(2, sceneObj3));
	gObjects.push_back(new Object(2, sceneObj4));
	gObjects.push_back(new Object(2, sceneObj5));
	gObjects.push_back(new Object(1, sceneObj6));
	
	//Configuraciones finales 

	//Posicionamos la camara en la posicion que nos interesa (donde esta el jugador)
	Ogre::Vector3 playerPos = gPlayer->m_sceneObject->mNode.getPosition();
	gCamera->setPosition(playerPos.x, playerPos.y, 450.0);

	//Luz ambiente y fondo
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
	//gSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
}