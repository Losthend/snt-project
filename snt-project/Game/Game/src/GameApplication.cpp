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
void GameApplication::createScene1(void)
{
	//****************************************************
	//JUGADOR 
	//****************************************************
	if(gPlayer == 0)
	{
		SceneObject* player = gPhysics->createConvexHullShape("Player", Ogre::Real(0.3), Ogre::Vector3(0, 10, 0), 2, "ninja.mesh");
		gPlayer = new Player(player);
	}

	//****************************************************
	//ESCENARIO 1: SceneObjects (Bullet) + gObjects (personal)
	//****************************************************
	SceneObject* sceneObj;

	//ASCENSOR
	createFont("elevatorWall2", Ogre::Vector2(100,100), Ogre::Vector3(0,50,-75), "Material/elevator_Wall", Ogre::Vector2(1,1));
	createFont("elevatorNote", Ogre::Vector2(40,40), Ogre::Vector3(0,50,-74), "Material/elevator_Note", Ogre::Vector2(1,1));

	sceneObj = gPhysics->createGroundShape("elevatorFloor", Ogre::Vector3(100, 5, 150), Ogre::Vector3(0, 0, 0), Ogre::Vector2(1,1), "Material/elevator_Floor");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorWall1", Ogre::Vector3(5*0.1, 100*0.1, 150*0.1), Ogre::Vector3(-50, 50, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/elevator_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	
	//PUERTA ASCENSOR
	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor1", Ogre::Vector3(2*0.1, 100*0.1, 50*0.1), Ogre::Vector3(51, 50, 50), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor2", Ogre::Vector3(2*0.1, 100*0.1, 50*0.1), Ogre::Vector3(51, 50, -50), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor3", Ogre::Vector3(2*0.1, 20*0.1, 50*0.1), Ogre::Vector3(51, 90, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	createObject("elevatorButton", Ogre::Vector3(3*0.01, 15*0.01, 15*0.01), Ogre::Vector3(51, 50, 50), "Material/elevator_Button","Cube.mesh");
	

	//SALA PRINCIPAL
	createFont("bunkerWall1", Ogre::Vector2(900,100), Ogre::Vector3(500,50,-75), "Material/bunker_Wall", Ogre::Vector2(8,1));

	sceneObj = gPhysics->createGroundShape("bunkerFloor1", Ogre::Vector3(530, 5, 150), Ogre::Vector3(315, 0, 0), Ogre::Vector2(5,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerFloor2", Ogre::Vector3(260, 5, 150), Ogre::Vector3(820, 0, 0), Ogre::Vector2(2,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	
	sceneObj = gPhysics->createPrimitiveShape("bunkerWall2", Ogre::Vector3(5*0.1, 100*0.1, 150*0.1), Ogre::Vector3(950, 50, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	
	//Agujero
	sceneObj = gPhysics->createGroundShape("bunkerHoleRight", Ogre::Vector3(20, 5, 50), Ogre::Vector3(685, 0, 0), Ogre::Vector2(1,1), "Material/bunker_holeRight");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerHoleLeft", Ogre::Vector3(20, 5, 50), Ogre::Vector3(588, 0, 0), Ogre::Vector2(1,1), "Material/bunker_holeLeft");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerHoleTop", Ogre::Vector3(130, 5, 50), Ogre::Vector3(635, 0, -50), Ogre::Vector2(1,1), "Material/bunker_holeTop");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerHoleBottom", Ogre::Vector3(130, 5, 50), Ogre::Vector3(635, 0, 50), Ogre::Vector2(1,1), "Material/bunker_holeBottom");
	gObjects.push_back(new Object(1, sceneObj));	

	//BLOQUEO
	sceneObj = gPhysics->createPrimitiveShape("stone1", Ogre::Vector3(100*0.1, 5*0.1, 150*0.1), Ogre::Vector3(635, 10, 0), 100, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone2", Ogre::Real(40), Ogre::Vector3(635, 50, -50), 80, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone3", Ogre::Real(20), Ogre::Vector3(610, 50, -25), 40, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone5", Ogre::Real(60), Ogre::Vector3(635, 50, 0), 100, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone6", Ogre::Real(50), Ogre::Vector3(640, 50, 25), 120, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone7", Ogre::Real(30), Ogre::Vector3(615, 50, 50), 60, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 

	//DECORACION
	createFont("bunkerFlag1", Ogre::Vector2(50,75), Ogre::Vector3(200,50,-74), "Material/bunker_flag1", Ogre::Vector2(1,1));
	createFont("bunkerFlag2", Ogre::Vector2(50,75), Ogre::Vector3(400,50,-74), "Material/bunker_flag2", Ogre::Vector2(1,1));

	sceneObj = gPhysics->createPrimitiveShape("monsterSkeleton", Ogre::Vector3(75, 75, 75), Ogre::Vector3(75, 0, -50), 0, "monster_skeleton.mesh");
	gObjects.push_back(new Object(1, sceneObj)); 

	//Hierba (null)
	//createGroundGrass(Ogre::Vector3(100,100,100), Ogre::Vector3(0,-50,0), 10);
	
	//Objetos (2)
	/*
	sceneObj = gPhysics->createPrimitiveShape("Cube1", Ogre::Vector3(50, 50, 50), Ogre::Vector3(-50, 80, 0), 10, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
    sceneObj = gPhysics->createPrimitiveShape("Cube2", Ogre::Vector3(50, 50, 50), Ogre::Vector3(50, 120, 0), 10, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
    sceneObj = gPhysics->createPrimitiveShape("Cube3", Ogre::Vector3(50, 50, 50), Ogre::Vector3(50, 150, 0), 10, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
    sceneObj = gPhysics->createPrimitiveShape("Cube4", Ogre::Vector3(50, 50, 50), Ogre::Vector3(-50, 100, 0), 10, "Barrel.mesh");
	gObjects.push_back(new Object(2, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("Plane1", Ogre::Vector3(100, 100, 100), Ogre::Vector3(60, 110, 0), 10, "WoodPallet.mesh");
	gObjects.push_back(new Object(2, sceneObj));
	*/

	//Decoracion (1)
	//sceneObj = gPhysics->createConvexHullShape("house1", Ogre::Real(0.3), Ogre::Vector3(-500, 150, 0), 0, "tudorhouse.mesh");
	//gObjects.push_back(new Object(1, sceneObj));

	 //Trampas (3)
	//sceneObj = gPhysics->createBoxObject("jaiqua1", Ogre::Vector3(10, 10, 10), Ogre::Vector3(-100, -50, 0), 1, "spine.mesh");
	//gObjects.push_back(new Object(1, sceneObj));

	//Enemigos (4)
	//sceneObj = gPhysics->createBoxObject("penguin1", Ogre::Vector3(10, 10, 10), Ogre::Vector3(-100, -50, 0), 1, "robot.mesh");
	//gObjects.push_back(new Object(1, sceneObj));
	
	//CONFIGURACIONES

	//Posicionamos la camara en la posicion que nos interesa (donde esta el jugador)
	Ogre::Vector3 playerPos = gPlayer->m_sceneObject->mNode->getPosition();
	gCamera->setPosition(playerPos.x, playerPos.y, 450.0);

	//Luz ambiente
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);

	//Fondo
	//gSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");
	// Examples/MorningSkyBox 
	// Examples/StormySkyBox
}

//---------------------------------------------------------------------------
//Crea planos verticales (fondos) con el material indicado
//---------------------------------------------------------------------------
void GameApplication::createFont(Ogre::String name, Ogre::Vector2 size, Ogre::Vector3 pos, Ogre::String material, Ogre::Vector2 repeat)
{
	//Plano
	Ogre::Plane plane(Ogre::Vector3::UNIT_Z, 0);
	Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().createPlane(name, 
																		  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
																		  plane, size.x, size.y, 
																		  20, 20, true, 1, repeat.x, repeat.y, Ogre::Vector3::UNIT_Y);
	//Entidad 
	Ogre::Entity *entity = gSceneMgr->createEntity(name);
	entity->setMaterialName(material);
	//Nodo
	Ogre::SceneNode *node = gSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(entity);
	node->setPosition(pos.x, pos.y, pos.z);
}

//---------------------------------------------------------------------------
//Crea un objeto sencillo que no requiere deteccion de colisiones
//--------------------------------------------------------------------------
void GameApplication::createObject(Ogre::String name, Ogre::Vector3 size, Ogre::Vector3 pos, Ogre::String material, Ogre::String mesh)
{
	Ogre::Entity *entity = gSceneMgr->createEntity(name, mesh);
	entity->setMaterialName(material);
	Ogre::SceneNode* node = gSceneMgr->getRootSceneNode()->createChildSceneNode(name);
	node->attachObject(entity);
	node->setScale(size.x, size.y, size.z);
	node->setPosition(pos.x, pos.y, pos.z);
	node->_updateBounds();
}

//---------------------------------------------------------------------------
//Añade hierba en en area a partir de una posicion origen y una cantidad
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