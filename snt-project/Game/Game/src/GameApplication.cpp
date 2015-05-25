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
	btCollisionShape &playerShape = mPhysics->createBoxShape(20, 20, 20);
	SceneObject* player = gPhysics->createBoxObject("Player", Ogre::Vector3(10, 10, 10), Ogre::Vector3(0, 100, 0), 1, playerShape);
	gPlayer = new Player(player);

	//Creacion de objetos
	
	//Shapes
	btCollisionShape &cubeShape = mPhysics->createBoxShape(20, 20, 20);
	btCollisionShape &planeShape = mPhysics->createBoxShape(80, 6, 20);
	btCollisionShape &plane2Shape = mPhysics->createBoxShape(1600, 1, 200);
	
	//SceneObjects: necesarios para el control ejercido por Bullet
	SceneObject* sceneObj1 = gPhysics->createBoxObject("cube1", Ogre::Vector3(10, 10, 10), Ogre::Vector3(-50, 100, 0), 1, cubeShape);
	SceneObject* sceneObj2 = gPhysics->createBoxObject("cube2", Ogre::Vector3(10, 10, 10), Ogre::Vector3(-50, 80, 0), 1, cubeShape);
    SceneObject* sceneObj3 = gPhysics->createBoxObject("cube3", Ogre::Vector3(10, 10, 10), Ogre::Vector3(50, 120, 0), 1, cubeShape);
    SceneObject* sceneObj4 = gPhysics->createBoxObject("cube4", Ogre::Vector3(10, 10, 10), Ogre::Vector3(50, 150, 0), 1, cubeShape);
    SceneObject* sceneObj5 = gPhysics->createBoxObject("plane", Ogre::Vector3(40, 3, 10), Ogre::Vector3(60, 110, 0), 1, planeShape);
	SceneObject* sceneObj6 = gPhysics->createBoxObject("floor", Ogre::Vector3(800, 1, 100), Ogre::Vector3(0, -50, 0), 0, plane2Shape);

	//Objets: necesarios para nuestras propias definiciones de propiedades de objeto
	Object* obj1 = new Object(2, sceneObj1);
	Object* obj2 = new Object(2, sceneObj2);
	Object* obj3 = new Object(2, sceneObj3);
	Object* obj4 = new Object(2, sceneObj4);
	Object* obj5 = new Object(2, sceneObj5);
	Object* obj6 = new Object(1, sceneObj6);

	//Almacenamiento del objeto completo en gObjects (vector global)
	gObjects.push_back(obj1);
	gObjects.push_back(obj2);
	gObjects.push_back(obj3);
	gObjects.push_back(obj4);
	gObjects.push_back(obj5);
	gObjects.push_back(obj6);
	
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