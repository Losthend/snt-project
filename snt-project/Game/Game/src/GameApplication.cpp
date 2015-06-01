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
	//PhysicsManager: control de fisicas y colisiones de Bullet
	gPhysics = new PhysicsManager();
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
	//****************************************************
	//JUGADOR 
	//****************************************************
	if(gPlayer == 0)
	{
		SceneObject* player = gPhysics->createBoxObject("Player", Ogre::Vector3(3, 3, 3), Ogre::Vector3(0, 100, 0), 1, "ninja.mesh");
		gPlayer = new Player(player);
	}

	//****************************************************
	//ESCENARIO: SceneObjects (Bullet) + gObjects (personal)
	//****************************************************
	SceneObject* sceneObj;

	//Suelo (1)
	sceneObj = gPhysics->createGroundObject("ground", Ogre::Vector3(1500, 5, 1500), Ogre::Vector3(0, -50, 0), "Examples/GrassFloor");
	gObjects.push_back(new Object(1, sceneObj));
	
	//Hierba (null)
	createGroundGrass(Ogre::Vector3(100,100,100), Ogre::Vector3(0,-50,0), 10);

	//Objetos (2)
	sceneObj = gPhysics->createBoxObject("cube1", Ogre::Vector3(50, 50, 50), Ogre::Vector3(-50, 80, 0), 1, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
    sceneObj = gPhysics->createBoxObject("cube2", Ogre::Vector3(50, 50, 50), Ogre::Vector3(50, 120, 0), 1, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
    sceneObj = gPhysics->createBoxObject("cube3", Ogre::Vector3(50, 50, 50), Ogre::Vector3(50, 150, 0), 1, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
    sceneObj = gPhysics->createBoxObject("cube4", Ogre::Vector3(50, 50, 50), Ogre::Vector3(-50, 100, 0), 1, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
	sceneObj = gPhysics->createBoxObject("plane1", Ogre::Vector3(100, 100, 100), Ogre::Vector3(60, 110, 0), 1, "WoodPallet.mesh");
	gObjects.push_back(new Object(2, sceneObj));

	//Decoracion (1)
	sceneObj = gPhysics->createBoxObject("house1", Ogre::Vector3(3, 3, 3), Ogre::Vector3(-500, 125, 0), 0, "tudorhouse.mesh");
	gObjects.push_back(new Object(1, sceneObj));
	
	 //Trampas (3)
	//sceneObj = gPhysics->createBoxObject("jaiqua1", Ogre::Vector3(10, 10, 10), Ogre::Vector3(-100, -50, 0), 1, "spine.mesh");
	//gObjects.push_back(new Object(1, sceneObj));

	//Enemigos (4)
	//sceneObj = gPhysics->createBoxObject("penguin1", Ogre::Vector3(10, 10, 10), Ogre::Vector3(-100, -50, 0), 1, "robot.mesh");
	//gObjects.push_back(new Object(1, sceneObj));
	
	//CONFIGURACIONES

	//Posicionamos la camara en la posicion que nos interesa (donde esta el jugador)
	Ogre::Vector3 playerPos = gPlayer->m_sceneObject->mNode.getPosition();
	gCamera->setPosition(playerPos.x, playerPos.y, 450.0);

	//Luz ambiente y fondo
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
	gSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");
	// Examples/MorningSkyBox 
	// Examples/StormySkyBox
}


//---------------------------------------------------------------------------
//Añade hierba en la posicion especificada a partir de una posicion origen
//---------------------------------------------------------------------------
void GameApplication::createGroundGrass(Ogre::Vector3 vSize, Ogre::Vector3 vPos, int amount)
{
	Ogre::Entity* grass = gSceneMgr->createEntity("grass","manualObjectGrass");
	Ogre::StaticGeometry* sg = gSceneMgr->createStaticGeometry("GrassArea");

	sg->setRegionDimensions(Ogre::Vector3(vSize.x, vSize.y, vSize.z));
	sg->setOrigin(Ogre::Vector3(vSize.x, vSize.y, vSize.z));

	for (Ogre::Real x = -vSize.x/2; x < vSize.x/2; x += (vSize.x / amount))
	{
		for (Ogre::Real z = -vSize.z/2; z < vSize.z/2; z += (vSize.z / amount))
		{
			Ogre::Real offsetX = -vSize.x / (float)amount / 2;
			Ogre::Real offsetZ = -vSize.z / (float)amount / 2;
			Ogre::Vector3 pos(
				x + Ogre::Math::RangeRandom(-offsetX, offsetX),
				vPos.y,
				z + Ogre::Math::RangeRandom(-offsetZ, offsetZ));
			Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(Ogre::Real(0.9), Ogre::Real(1.1)), 1);
			Ogre::Quaternion quat;
			quat.FromAngleAxis(
				Ogre::Degree(Ogre::Math::RangeRandom(0, 359)),
				Ogre::Vector3::UNIT_Y);
			sg->addEntity(grass, pos, quat, scale);
		}
	}
	sg->build();
}