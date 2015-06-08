#include "../include/stdafx.h"
#include "../include/GameApplication.h"

#include "../include/Global.h"
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/PhysicsManager.h"
#include "../include/SceneObject.h"
#include "../include/AnimationManager.h"

//---------------------------------------------------------------------------
//Constructor del juego (Game)
//---------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
	//PhysicsManager: control de fisicas y colisiones de Bullet
	gPhysics = new PhysicsManager();
	activeScene = 0;
}
//---------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
}

//---------------------------------------------------------------------------
//ESCENARIO 1
//---------------------------------------------------------------------------
void GameApplication::createScene1(void)
{
	//****************************************************
	//Liberar espacio 
	//****************************************************
	clearScene();
	gCanUpdate = true;
	activeScene = 1;

	//****************************************************
	//JUGADOR 
	//****************************************************
	if(gPlayer == 0)
	{
		SceneObject* player = gPhysics->createConvexHullShape("Player", Ogre::Real(5), Ogre::Vector3(0, 10, 0), 1, "Sinbad.mesh");
		gPlayer = new Player(player);
		gPlayer->m_animMgr = new AnimationManager();
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

	sceneObj = gPhysics->createPrimitiveShape("elevatorWall1", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(100*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(-50, 50, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/elevator_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	sceneObj = gPhysics->createPrimitiveShape("elevatorRoof", Ogre::Vector3(Ogre::Real(100*0.1), Ogre::Real(5*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(0, 100, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/elevator_Floor");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("elevatorWall3", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(100*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(49, 150, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/elevator_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	
	//PUERTA ASCENSOR
	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor1", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(100*0.1), Ogre::Real(50*0.1)), Ogre::Vector3(51, 50, 50), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor2", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(100*0.1), Ogre::Real(50*0.1)), Ogre::Vector3(51, 50, -50), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor3", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(30*0.1), Ogre::Real(50*0.1)), Ogre::Vector3(51, 85, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	createObject("elevatorButton", Ogre::Vector3(Ogre::Real(3*0.01), Ogre::Real(15*0.01), Ogre::Real(15*0.01)), Ogre::Vector3(51, 40, 50), "Material/elevator_Button","Cube.mesh");
	
	//SALA PRINCIPAL
	createFont("bunkerFont1", Ogre::Vector2(900,250), Ogre::Vector3(500,125,-75), "Material/bunker_Wall", Ogre::Vector2(8,2));
	createFont("bunkerFont2", Ogre::Vector2(900,250), Ogre::Vector3(500,-125,75), "Material/bunker_Wall", Ogre::Vector2(8,2));

	sceneObj = gPhysics->createGroundShape("bunkerFloor1", Ogre::Vector3(530, 5, 150), Ogre::Vector3(315, 0, 0), Ogre::Vector2(5,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerFloor2", Ogre::Vector3(260, 5, 150), Ogre::Vector3(820, 0, 0), Ogre::Vector2(2,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	
	sceneObj = gPhysics->createPrimitiveShape("bunkerWall1", Ogre::Vector3(5*0.1, 250*0.1, 150*0.1), Ogre::Vector3(950, 125, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	sceneObj = gPhysics->createPrimitiveShape("bunkerWall2", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(150*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(51, 175, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	sceneObj = gPhysics->createPrimitiveShape("bunkerRoof", Ogre::Vector3(900*0.1, 5*0.1, 150*0.1), Ogre::Vector3(500, 250, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	//AGUJERO
	sceneObj = gPhysics->createGroundShape("bunkerHoleRight", Ogre::Vector3(20, 5, 50), Ogre::Vector3(685, 0, 0), Ogre::Vector2(1,1), "Material/bunker_holeRight");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerHoleLeft", Ogre::Vector3(20, 5, 50), Ogre::Vector3(588, 0, 0), Ogre::Vector2(1,1), "Material/bunker_holeLeft");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerHoleTop", Ogre::Vector3(130, 5, 50), Ogre::Vector3(635, 0, -50), Ogre::Vector2(1,1), "Material/bunker_holeTop");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerHoleBottom", Ogre::Vector3(130, 5, 50), Ogre::Vector3(635, 0, 50), Ogre::Vector2(1,1), "Material/bunker_holeBottom");
	gObjects.push_back(new Object(1, sceneObj));	

	//BLOQUEO
	sceneObj = gPhysics->createPrimitiveShape("stone1", Ogre::Vector3(100*0.1, 5*0.1, 150*0.1), Ogre::Vector3(635, 10, 0), 60, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone2", Ogre::Real(40), Ogre::Vector3(635, 50, -50), 40, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone3", Ogre::Real(23), Ogre::Vector3(610, 50, -25), 23, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone5", Ogre::Real(50), Ogre::Vector3(635, 50, 0), 50, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone6", Ogre::Real(48), Ogre::Vector3(640, 50, 25), 48, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone7", Ogre::Real(31), Ogre::Vector3(615, 50, 50), 31, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 

	//DECORACION
	createFont("bunkerFlag1", Ogre::Vector2(75,150), Ogre::Vector3(200,125,-74), "Material/bunker_flag1", Ogre::Vector2(1,1));
	createFont("bunkerFlag2", Ogre::Vector2(75,150), Ogre::Vector3(400,125,-74), "Material/bunker_flag2", Ogre::Vector2(1,1));
	createFont("bunkerDoor2", Ogre::Vector2(50,75), Ogre::Vector3(750,35,-74), "Material/bunker_Door2", Ogre::Vector2(1,1));
	createFont("bunker_Poster", Ogre::Vector2(40,50), Ogre::Vector3(860,50,-74), "Material/bunker_Poster", Ogre::Vector2(1,1));
	createFont("bunker_BulletHole", Ogre::Vector2(50,50), Ogre::Vector3(80,50,-74), "Material/bunker_BulletHole", Ogre::Vector2(1,1));
	createFont("bunker_wallHole", Ogre::Vector2(150,150), Ogre::Vector3(635,150,-74), "Material/bunker_wallHole", Ogre::Vector2(1,1));
	
	createObject("bunkerDoor1", Ogre::Vector3(Ogre::Real(7*0.01), Ogre::Real(75*0.01), Ogre::Real(50*0.01)), Ogre::Vector3(950, 35, 0), "Material/bunker_Door1","Cube.mesh");

	sceneObj = gPhysics->createPrimitiveShape("monsterSkeleton", Ogre::Vector3(75, 75, 75), Ogre::Vector3(75, 0, -50), 0, "monster_skeleton.mesh");
	gObjects.push_back(new Object(1, sceneObj)); 

	//LUCES
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	Ogre::Light* light = gSceneMgr->createLight("MainLight");
	light->setPosition(20,80,50);
}

//---------------------------------------------------------------------------
//ESCENARIO 2
//---------------------------------------------------------------------------
void GameApplication::createScene2(void)
{
	//****************************************************
	//Liberar espacio 
	//****************************************************
	clearScene();
	gCanUpdate = true;
	activeScene = 2;

	//****************************************************
	//JUGADOR 
	//****************************************************
	if(gPlayer == 0)
	{
		SceneObject* player = gPhysics->createConvexHullShape("Player", Ogre::Real(5), Ogre::Vector3(0, 10, 0), 1, "Sinbad.mesh");
		gPlayer = new Player(player);
		gPlayer->m_animMgr = new AnimationManager();
	}

	//Posicion del jugador
	btTransform transform = gPlayer->m_sceneObject->mRigidBody->getCenterOfMassTransform();
	transform.setOrigin(btVector3(0, 150, 0));
	gPlayer->m_sceneObject->mRigidBody->setCenterOfMassTransform(transform);

	//****************************************************
	//ESCENARIO 2
	//****************************************************
	SceneObject* sceneObj;

	//SALA PRINCIPAL
	sceneObj = gPhysics->createGroundShape("elevatorFloor", Ogre::Vector3(100, 5, 150), Ogre::Vector3(0, 0, 0), Ogre::Vector2(1,1), "Material/elevator_Floor");
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
}

//---------------------------------------------------------------------------
//Liberar espacio
//---------------------------------------------------------------------------
void GameApplication::clearScene(void)
{
	for(unsigned x = gObjects.size(); x > 0 ; x--)
	{
		gSceneMgr->destroyEntity(gObjects[x-1]->m_sceneObject->mEntity->getName());
		gSceneMgr->destroySceneNode(gObjects[x-1]->m_sceneObject->mNode->getName());
		gPhysics->mWorld->removeRigidBody(gObjects[x-1]->m_sceneObject->mRigidBody);
		gObjects.erase(gObjects.begin()+x-1);
	}
	for(unsigned x = gEntitys.size(); x > 0 ; x--)
	{
		gSceneMgr->destroyEntity(gEntitys[x-1]->getName());
		gEntitys.erase(gEntitys.begin()+x-1);
	}
	for(unsigned x = gNodes.size(); x > 0 ; x--)
	{
		gSceneMgr->destroySceneNode(gNodes[x-1]->getName());
		gNodes.erase(gNodes.begin()+x-1);
	}
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
	//Guardar referencias
	gEntitys.push_back(entity);
	gNodes.push_back(node);
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
	//Guardar referencias
	gEntitys.push_back(entity);
	gNodes.push_back(node);
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