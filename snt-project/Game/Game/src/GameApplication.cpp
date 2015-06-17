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
	gSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
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
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	SceneObject* sceneObj;

	//ASCENSOR
	createFont("elevatorWall2", Ogre::Vector2(100,100), Ogre::Vector3(0,50,-75), "Material/elevator_Wall", Ogre::Vector2(1,1), true);
	createFont("elevatorNote", Ogre::Vector2(40,40), Ogre::Vector3(0,50,-74), "Material/elevator_Note", Ogre::Vector2(1,1), true);

	sceneObj = gPhysics->createGroundShape("elevatorFloor", Ogre::Vector3(100, 5, 150), Ogre::Vector3(0, 0, 0), Ogre::Vector2(1,1), "Material/elevator_Floor");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorWall1", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(100*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(-50, 50, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/elevator_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	sceneObj = gPhysics->createPrimitiveShape("elevatorRoof", Ogre::Vector3(Ogre::Real(98*0.1), Ogre::Real(5*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(-2, 100, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/elevator_Floor");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(100*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(49, 50, 0), 1, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/elevator_Wall");
	sceneObj->mRigidBody->setGravity(btVector3(0,0,0));
	sceneObj->mRigidBody->setLinearFactor(btVector3(0,1,0));
	sceneObj->mRigidBody->setAngularFactor(btVector3(0,0,0));
	specialObject0 = new Object(1, sceneObj);
	gObjects.push_back(specialObject0);
	
	//PUERTA ASCENSOR
	createObject("elevatorButton", Ogre::Vector3(Ogre::Real(3*0.01), Ogre::Real(15*0.01), Ogre::Real(15*0.01)), Ogre::Vector3(51, 40, 50), "Material/elevator_Button","Cube.mesh");

	sceneObj = gPhysics->createPrimitiveShape("elevatorDoorNear", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(75*0.1), Ogre::Real(50*0.1)), Ogre::Vector3(51, 35, 50), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("elevatorDoorFar", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(75*0.1), Ogre::Real(50*0.1)), Ogre::Vector3(51, 35, -50), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("elevatorDoorTop", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(175*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(51, 160, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 

	//SALA PRINCIPAL
	createFont("bunkerFont1", Ogre::Vector2(900,250), Ogre::Vector3(500,125,-75), "Material/bunker_Wall", Ogre::Vector2(8,2), true);
	createFont("bunkerFont2", Ogre::Vector2(900,250), Ogre::Vector3(500,-125,75), "Material/bunker_Wall", Ogre::Vector2(8,2), true);

	sceneObj = gPhysics->createGroundShape("bunkerFloor1", Ogre::Vector3(530, 5, 150), Ogre::Vector3(315, 0, 0), Ogre::Vector2(5,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerFloor2", Ogre::Vector3(260, 5, 150), Ogre::Vector3(820, 0, 0), Ogre::Vector2(2,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	
	sceneObj = gPhysics->createPrimitiveShape("bunkerWall1", Ogre::Vector3(5*0.1, 250*0.1, 150*0.1), Ogre::Vector3(950, 125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	
	sceneObj = gPhysics->createPrimitiveShape("bunkerRoof", Ogre::Vector3(900*0.1, 5*0.1, 150*0.1), Ogre::Vector3(500, 250, 0), 0, "Cube.mesh", true);
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
	sceneObj = gPhysics->createPrimitiveShape("stone1", Ogre::Vector3(100*0.1, 5*0.1, 150*0.1), Ogre::Vector3(635, 10, 0), 60, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone2", Ogre::Real(40), Ogre::Vector3(635, 50, -50), 40, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone3", Ogre::Real(23), Ogre::Vector3(610, 50, -35), 23, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone5", Ogre::Real(50), Ogre::Vector3(635, 50, 0), 50, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone6", Ogre::Real(48), Ogre::Vector3(640, 50, 35), 48, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 
	sceneObj = gPhysics->createConvexHullShape("stone7", Ogre::Real(31), Ogre::Vector3(615, 50, 50), 31, "stone.mesh");
	gObjects.push_back(new Object(2, sceneObj)); 

	//DECORACION
	createFont("bunkerFlag1", Ogre::Vector2(75,150), Ogre::Vector3(200,125,-74), "Material/bunker_flag1", Ogre::Vector2(1,1), true);
	createFont("bunkerFlag2", Ogre::Vector2(75,150), Ogre::Vector3(400,125,-74), "Material/bunker_flag2", Ogre::Vector2(1,1), true);
	createFont("bunkerFlag3",Ogre::Vector2(75,150), Ogre::Vector3(850,125,-74), "Material/bunker_flag3", Ogre::Vector2(1,1), true);
	createFont("bunkerDoor2", Ogre::Vector2(50,75), Ogre::Vector3(750,35,-74), "Material/bunker_Door2", Ogre::Vector2(1,1), true);
	createFont("bunkerBulletHole", Ogre::Vector2(50,50), Ogre::Vector3(80,50,-74), "Material/bunker_BulletHole", Ogre::Vector2(1,1), true);
	createFont("bunkerwallHole", Ogre::Vector2(150,150), Ogre::Vector3(635,150,-74), "Material/bunker_wallHole", Ogre::Vector2(1,1), true);
	createFont("bunkerFissure1", Ogre::Vector2(75,100), Ogre::Vector3(331,0,-74), "Material/bunker_Fissure", Ogre::Vector2(1,1), true);
	createFont("bunkerFissure2", Ogre::Vector2(75,100), Ogre::Vector3(331,1,-75), "Material/bunker_Fissure", Ogre::Vector2(1,1), false);
	createFont("bunkerBlood", Ogre::Vector2(50,50), Ogre::Vector3(745,1,-48), "Material/bunker_Blood", Ogre::Vector2(1,1), false);
	
	createObject("bunkerDoor1", Ogre::Vector3(Ogre::Real(7*0.01), Ogre::Real(75*0.01), Ogre::Real(50*0.01)), Ogre::Vector3(950, 35, 0), "Material/bunker_Door1","Cube.mesh");
	createObject("monsterSkeleton", Ogre::Vector3(Ogre::Real(75*0.1), Ogre::Real(75*0.1), Ogre::Real(75*0.1)), Ogre::Vector3(75, 0, -50), "","monster_skeleton.mesh");

	//LUCES
	Ogre::Light *light = gSceneMgr->createLight("shadowLight");     
	light->setType(Ogre::Light::LT_SPOTLIGHT);
	light->setPosition(635, 249, 0);
	light->setDirection(0,-1,0);
	light->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(50));	
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
	transform.setOrigin(btVector3(0, 250, 0));
	gPlayer->m_sceneObject->mRigidBody->setCenterOfMassTransform(transform);

	//****************************************************
	//ESCENARIO 2
	//****************************************************
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	SceneObject* sceneObj;

	//ZONA 1: Punto de caida
	createFont("undergroundEntrance", Ogre::Vector2(200,100), Ogre::Vector3(400,30,-74), "Material/underground_Entrance", Ogre::Vector2(1,1), true);
	createFont("undergroundFarWall1", Ogre::Vector2(1000,250), Ogre::Vector3(450,125,-75), "Material/underground_Wall", Ogre::Vector2(1,1), true);
	createFont("undergroundNearWall1", Ogre::Vector2(1000,500), Ogre::Vector3(450,-250,75), "Material/underground_Wall", Ogre::Vector2(1,2), true);

	sceneObj = gPhysics->createConvexHullShape("bigStone1", Ogre::Real(100), Ogre::Vector3(5, 25, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/stone2");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createGroundShape("undergroundFloor1", Ogre::Vector3(900, 5, 150), Ogre::Vector3(500, 0, 0), Ogre::Vector2(5,1), "Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("undergroundLeft", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(-50, 125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 

	//ZONA 1: techo
	sceneObj = gPhysics->createPrimitiveShape("undergroundHole", Ogre::Vector3(Ogre::Real(150*0.1), Ogre::Real(5*0.1), Ogre::Real(125*0.1)), Ogre::Vector3(25, 250, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/underground_Hole");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createConvexHullShape("stoneLeft1", Ogre::Real(200), Ogre::Vector3(200, 250, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("stoneLeft2", Ogre::Real(200), Ogre::Vector3(400, 250, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("stoneCenter", Ogre::Real(300), Ogre::Vector3(600, 250, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("stoneRight", Ogre::Real(300), Ogre::Vector3(950, 190, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 

	//ZONA 2: zona de ruinas
	createFont("undergroundFarWall2", Ogre::Vector2(900,550), Ogre::Vector3(1400,-25,-600), "Material/underground_Wall", Ogre::Vector2(1,1), true);
	createFont("undergroundNearWall2", Ogre::Vector2(900,500), Ogre::Vector3(1400,-550,75), "Material/underground_Wall", Ogre::Vector2(1,2), true);

	sceneObj = gPhysics->createGroundShape("undergroundFloor2", Ogre::Vector3(900, 5, 700), Ogre::Vector3(1400, -300, -275), Ogre::Vector2(3,1), "Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("undergroundCenter", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(300*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(950, -150, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	createObject("undergroundWall1", Ogre::Vector3(Ogre::Real(5*0.01), Ogre::Real(550*0.01), Ogre::Real(550*0.01)), Ogre::Vector3(950, -25, -350), "Material/underground_Wall","Cube.mesh");
	createObject("undergroundTemple", Ogre::Vector3(Ogre::Real(35), Ogre::Real(35), Ogre::Real(35)), Ogre::Vector3(1445, -300, -500), "","temple.mesh");
	createObject("stalactite1", Ogre::Vector3(250, 250, 250), Ogre::Vector3(1100, -270, -400), "","stalactites.mesh");
	createObject("stalactite2", Ogre::Vector3(150, 150, 150), Ogre::Vector3(1710, -260, -500), "","stalactites.mesh");

	//Zona 3: Precipicio
	createFont("undergroundFarWall3", Ogre::Vector2(1400,2000), Ogre::Vector3(2550,-300,-600), "Material/underground_Wall", Ogre::Vector2(2,2), true);

	createObject("undergroundNearWall3", Ogre::Vector3(Ogre::Real(5*0.01), Ogre::Real(900*0.01), Ogre::Real(700*0.01)), Ogre::Vector3(1850, -750, -275), "Material/underground_Wall","Cube.mesh");

	//Zona 4: Puente
	sceneObj = gPhysics->createConvexHullShape("stone1", Ogre::Real(70), Ogre::Vector3(1940, -310, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/stone2");
	gObjects.push_back(new Object(5, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("stone2", Ogre::Real(70), Ogre::Vector3(2100, -280, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/stone2");
	gObjects.push_back(new Object(5, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("stone3", Ogre::Real(70), Ogre::Vector3(2220, -350, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/stone2");
	gObjects.push_back(new Object(5, sceneObj));

	//Zona 5: Secreto
	createObject("undergroundBlockFar", Ogre::Vector3(300, 300, 300), Ogre::Vector3(2450, -300, -310), "Material/stone2","stone.mesh");
	createObject("undergroundStatue", Ogre::Vector3(20, 20, 20), Ogre::Vector3(2430, -300, -100), "","statue.mesh");

	sceneObj = gPhysics->createPrimitiveShape("undergroundGroundSecret", Ogre::Vector3(Ogre::Real(600*0.1), Ogre::Real(1000*0.1), Ogre::Real(700*0.1)), Ogre::Vector3(2570, -805, -275), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	sceneObj = gPhysics->createPrimitiveShape("undergroundBlock1", Ogre::Vector3(3000, 3000, 3000), Ogre::Vector3(2500, 0, -100), 0, "stone.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/stone2");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createConvexHullShape("undergroundBlock2", Ogre::Real(400), Ogre::Vector3(2700, -150, -100), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/stone2");
	gObjects.push_back(new Object(1, sceneObj));
	
	//ENEMIGO
	sceneObj = gPhysics->createConvexHullShape("bear", Ogre::Real(40), Ogre::Vector3(1400, -250, 0), 10, "bear.mesh");
	sceneObj->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,1,0,1));
	sceneObj->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	gObjects.push_back(new Object(4, sceneObj));

	//ESPADA
	Ogre::Entity *entitySword = gSceneMgr->createEntity("undergroundSword", "Sword.mesh");
	Ogre::SceneNode* nodeSword = gSceneMgr->getRootSceneNode()->createChildSceneNode("undergroundSword");
	nodeSword->attachObject(entitySword);
	nodeSword->setScale(7, 7, 7);
	nodeSword->setPosition(2500, -280, 15);
	nodeSword->yaw(Ogre::Degree(90));
	gEntitys.push_back(entitySword);
	gNodes.push_back(nodeSword);

	//LUZ 1 
	Ogre::Light *light = gSceneMgr->createLight("shadowLight1");     
	light->setType(Ogre::Light::LT_SPOTLIGHT);
	light->setPosition(0, 249, 0);
	light->setDirection(0,-1,0);
	light->setSpotlightRange(Ogre::Degree(30), Ogre::Degree(50));	

	//LUZ 2
	Ogre::Light *light2 = gSceneMgr->createLight("shadowLight3");     
	light2->setType(Ogre::Light::LT_SPOTLIGHT);
	light2->setPosition(1500, -290, -600);
	light2->setDirection(0,0,1);
	light2->setDiffuseColour(0.0, 0.5, 0.0);     
	light2->setSpecularColour(0.0, 0.5, 0.0);   
	light2->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(100));
	light2->setCastShadows(false);
}


//---------------------------------------------------------------------------
//ESCENARIO 3
//---------------------------------------------------------------------------
void GameApplication::createScene3(void)
{
	//****************************************************
	//Liberar espacio 
	//****************************************************
	clearScene();
	gCanUpdate = true;
	activeScene = 3;

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
	transform.setOrigin(btVector3(350, 150, 0));
	gPlayer->m_sceneObject->mRigidBody->setCenterOfMassTransform(transform);

	//****************************************************
	//ESCENARIO 3
	//****************************************************
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
	SceneObject* sceneObj;

	//ZONA CENTRAL
	createFont("templeFarWall1", Ogre::Vector2(1600,250), Ogre::Vector3(0,125,-150), "Material/temple_Wall", Ogre::Vector2(3,1), true);
	createFont("templeNearWall1", Ogre::Vector2(1000,250), Ogre::Vector3(300,-125,150), "Material/temple_Wall", Ogre::Vector2(2,1), true);
	
	createFont("templeHieroglyph1", Ogre::Vector2(145,145), Ogre::Vector3(350,125,-149), "Material/temple_Hieroglyph1", Ogre::Vector2(1,1), true);
	createFont("templeHieroglyph2", Ogre::Vector2(150,125), Ogre::Vector3(50,125,-149), "Material/temple_Hieroglyph2", Ogre::Vector2(1,1), true);
	createFont("templeHieroglyph3", Ogre::Vector2(145,145), Ogre::Vector3(-325,125,-149), "Material/temple_Hieroglyph3", Ogre::Vector2(1,1), true);
	createFont("templeHieroglyph4", Ogre::Vector2(50,50), Ogre::Vector3(-250,75,-149), "Material/temple_Hieroglyph4", Ogre::Vector2(1,1), true);

	createObject("templeColumn1", Ogre::Vector3(2, Ogre::Real(0.55), 2), Ogre::Vector3(-100, 60, -140), "Material/temple_Column","Column.mesh");
	createObject("templeColumn2", Ogre::Vector3(2, Ogre::Real(0.55), 2), Ogre::Vector3(200, 60, -140), "Material/temple_Column","Column.mesh");
	createObject("templeColumn3", Ogre::Vector3(2, Ogre::Real(0.55), 2), Ogre::Vector3(500, 60, -140), "Material/temple_Column","Column.mesh");

	sceneObj = gPhysics->createGroundShape("templeFloor", Ogre::Vector3(1000, 5, 300), Ogre::Vector3(300, 0, 0), Ogre::Vector2(2,1), "Material/temple_Floor");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("templeRoof", Ogre::Vector3(1600*0.1, 5*0.1, 250*0.1), Ogre::Vector3(0, 245, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/temple_Floor");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("templeRight", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(799, 125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/temple_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	//ZONA TRAMPAS
	createFont("templeNearWall3", Ogre::Vector2(1600,250), Ogre::Vector3(-200,-375,150), "Material/temple_Wall", Ogre::Vector2(3,1), true);

	sceneObj = gPhysics->createGroundShape("templeFloor3", Ogre::Vector3(300, 5, 300), Ogre::Vector3(-350, -250, 0), Ogre::Vector2(1,1), "Material/temple_Floor");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("templeFall1", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(-500, -125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/temple_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("templeFall2", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(-200, -125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/temple_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	//TRAMPAS
	sceneObj = gPhysics->createConvexHullShape("templeSpine1", Ogre::Real(4), Ogre::Vector3(-425, -175, -200), 100, "spine.mesh");
	sceneObj->mEntity->setMaterialName("Material/temple_Wood1");
	sceneObj->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(1,0,0,1));
	sceneObj->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	sceneObj->mRigidBody->setGravity(btVector3(0,0,0));
	sceneObj->mRigidBody->setLinearFactor(btVector3(0,0,0));
	sceneObj->mRigidBody->setAngularFactor(btVector3(0,0,1));
	gObjects.push_back(new Object(7, sceneObj));

	sceneObj = gPhysics->createConvexHullShape("templeSpine2", Ogre::Real(4), Ogre::Vector3(-275, -175, -200), 100, "spine.mesh");
	sceneObj->mEntity->setMaterialName("Material/temple_Wood2");
	sceneObj->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(1,0,0,1));
	sceneObj->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	sceneObj->mRigidBody->setGravity(btVector3(0,0,0));
	sceneObj->mRigidBody->setLinearFactor(btVector3(0,0,0));
	sceneObj->mRigidBody->setAngularFactor(btVector3(0,0,1));
	gObjects.push_back(new Object(7, sceneObj));

	//PASARELA
	sceneObj = gPhysics->createPrimitiveShape("templePlatform", Ogre::Vector3(Ogre::Real(25*0.1), Ogre::Real(25*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(-350, 0, 0), 1, "Cube.mesh", true);
	sceneObj->mRigidBody->setGravity(btVector3(0,0,0));
	sceneObj->mRigidBody->setAngularFactor(btVector3(0,0,0));
	sceneObj->mRigidBody->setLinearFactor(btVector3(0,0,0));
	sceneObj->mEntity->setMaterialName("Material/temple_Wall");
	gObjects.push_back(new Object(6, sceneObj));

	//ZONA DEL NUCLEO
	createGroundGrass(Ogre::Vector3(285,100,300), Ogre::Vector3(-650,0,0), 50);

	createFont("templeFarWall2", Ogre::Vector2(300,250), Ogre::Vector3(-350,-125,-150), "Material/temple_Wall", Ogre::Vector2(1,1), true);
	createFont("templeNearWall2", Ogre::Vector2(300,250), Ogre::Vector3(-650,-125,150), "Material/temple_Wall", Ogre::Vector2(1,1), true);
	createFont("templeCreeper1", Ogre::Vector2(300,300), Ogre::Vector3(-650,115,-149), "Material/temple_Creeper1", Ogre::Vector2(1,1), true);
	createFont("templeCreeper2", Ogre::Vector2(250,250), Ogre::Vector3(-625,-115,151), "Material/temple_Creeper2", Ogre::Vector2(1,1), true);

	sceneObj = gPhysics->createGroundShape("templeFloor2", Ogre::Vector3(300, 5, 300), Ogre::Vector3(-650, 0, 0), Ogre::Vector2(2,1), "Material/temple_Floor");
	gObjects.push_back(new Object(1, sceneObj));
	
	sceneObj = gPhysics->createPrimitiveShape("templeLeft", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(-799, 125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/temple_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("templeKey", Ogre::Vector3(2, 2, 2), Ogre::Vector3(-650, 50, 0), 1, "geosphere4500.mesh", false);
	specialObject1 = new Object(2, sceneObj);
	gObjects.push_back(specialObject1);

	//SALIDA
	createFont("templeOuroboros1", Ogre::Vector2(130,130), Ogre::Vector3(650,125,-149), "Material/temple_Ouroboros", Ogre::Vector2(1,1), true);
	createFont("templeOuroboros2", Ogre::Vector2(130,130), Ogre::Vector3(650,1,0), "Material/temple_Ouroboros", Ogre::Vector2(1,1), false);

	sceneObj = gPhysics->createConvexHullShape("templeLockSupport1", Ogre::Real(25), Ogre::Vector3(650, 0, 0), 0, "pedestal.mesh");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createConvexHullShape("templeLockSupport2", Ogre::Real(24), Ogre::Vector3(650, 230, 0), 0, "pedestal.mesh");
	sceneObj->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,0,1,0));
	sceneObj->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	gObjects.push_back(new Object(1, sceneObj));

	Ogre::Light *light = gSceneMgr->createLight("templeLight");     
	light->setType(Ogre::Light::LT_SPOTLIGHT);
	light->setPosition(795, 245, 0);
	light->setDirection(0,-1,0);
	light->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(200));
	light->setVisible(false);
}

//---------------------------------------------------------------------------
//ESCENARIO 4
//---------------------------------------------------------------------------
void GameApplication::createScene4(void)
{
	//****************************************************
	//Liberar espacio 
	//****************************************************
	clearScene();
	gCanUpdate = true;
	activeScene = 4;

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
	transform.setOrigin(btVector3(-150, 50, 0));
	gPlayer->m_sceneObject->mRigidBody->setCenterOfMassTransform(transform);

	//****************************************************
	//ESCENARIO 4
	//****************************************************
	SceneObject* sceneObj;

	//Estructura general
	createFont("endFarWall1", Ogre::Vector2(500,250), Ogre::Vector3(0,125,-150), "Material/end_FarWall", Ogre::Vector2(1,1), true);
	createFont("endNearWall1", Ogre::Vector2(500,250), Ogre::Vector3(0,-125,150), "Material/end_Floor1", Ogre::Vector2(1,1), true);
	createFont("endFalseFloor", Ogre::Vector2(125,125), Ogre::Vector3(-150,1,0), "Material/end_Floor2", Ogre::Vector2(1,1), false);

	sceneObj = gPhysics->createGroundShape("endFloor", Ogre::Vector3(500, 5, 300), Ogre::Vector3(0, 0, 0), Ogre::Vector2(1,1), "Material/end_Floor1");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("endRoof", Ogre::Vector3(500*0.1, 5*0.1, 250*0.1), Ogre::Vector3(0, 245, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/end_Floor1");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("endRight", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(250, 125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/end_Floor1");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("endLeft", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(-250, 125, 0), 0, "Cube.mesh", true);
	sceneObj->mEntity->setMaterialName("Material/end_Floor1");
	gObjects.push_back(new Object(1, sceneObj));

	//Elementos de teletransporte
	sceneObj = gPhysics->createConvexHullShape("endSupport1", Ogre::Real(10), Ogre::Vector3(-150, 0, -75), 0, "pedestal.mesh");
	sceneObj->mEntity->setMaterialName("Material/end_Floor1");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("endKey1", Ogre::Real(0.02), Ogre::Vector3(-150, 40, -75), 0, "geosphere4500.mesh");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createConvexHullShape("endSupport2", Ogre::Real(10), Ogre::Vector3(-150, 0, 75), 0, "pedestal.mesh");
	sceneObj->mEntity->setMaterialName("Material/end_Floor1");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("endKey2", Ogre::Real(0.02), Ogre::Vector3(-150, 40, 75), 0, "geosphere4500.mesh");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createConvexHullShape("endSupport3", Ogre::Real(10), Ogre::Vector3(-200, 0, 0), 0, "pedestal.mesh");
	sceneObj->mEntity->setMaterialName("Material/end_Floor1");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("endKey3", Ogre::Real(0.02), Ogre::Vector3(-200, 40, 0), 0, "geosphere4500.mesh");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createConvexHullShape("endSupport4", Ogre::Real(24), Ogre::Vector3(-150, 230, 0), 0, "pedestal.mesh");
	sceneObj->mEntity->setMaterialName("Material/end_Floor1");
	sceneObj->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,0,1,0));
	sceneObj->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createConvexHullShape("endKey4", Ogre::Real(0.05), Ogre::Vector3(-150, 140, 0), 0, "geosphere4500.mesh");
	sceneObj->mEntity->setMaterialName("Material/temple_Energy2");
	gObjects.push_back(new Object(1, sceneObj));

	//"Jefe" final
	sceneObj = gPhysics->createConvexHullShape("endBoss", Ogre::Real(0.8), Ogre::Vector3(100, 30, 0), 100, "robot.mesh");
	sceneObj->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,1,0,0));
	sceneObj->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	gObjects.push_back(new Object(8, sceneObj));

	//Decoracion	
	createObject("box1", Ogre::Vector3(Ogre::Real(50*0.01), Ogre::Real(50*0.01), Ogre::Real(50*0.01)), Ogre::Vector3(180, 25, 0), "Material/temple_Box","Cube.mesh");
	createObject("box2", Ogre::Vector3(Ogre::Real(50*0.01), Ogre::Real(50*0.01), Ogre::Real(50*0.01)), Ogre::Vector3(200, 25, -50), "Material/temple_Box","Cube.mesh");
	createObject("box3", Ogre::Vector3(Ogre::Real(50*0.01), Ogre::Real(50*0.01), Ogre::Real(50*0.01)), Ogre::Vector3(210, 25, 50), "Material/temple_Box","Cube.mesh");
	createObject("box4", Ogre::Vector3(Ogre::Real(50*0.01), Ogre::Real(50*0.01), Ogre::Real(50*0.01)), Ogre::Vector3(220, 75, 0), "Material/temple_Box","Cube.mesh");

	createObject("barrel1", Ogre::Vector3(5, 5, 5), Ogre::Vector3(125, 15, -50), "","Barrel.mesh");
	createObject("barrel2", Ogre::Vector3(5, 5, 5), Ogre::Vector3(150, 15, -65), "","Barrel.mesh");
	createObject("barrel3", Ogre::Vector3(5, 5, 5), Ogre::Vector3(145, 15, 55), "","Barrel.mesh");
	createObject("barrel4", Ogre::Vector3(5, 5, 5), Ogre::Vector3(130, 15, 75), "","Barrel.mesh");
	createObject("barrel5", Ogre::Vector3(5, 5, 5), Ogre::Vector3(150, 15, 65), "","Barrel.mesh");

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
	gSceneMgr->destroyAllLights();
	gSceneMgr->destroyAllStaticGeometry();
	specialObject0 = 0;
	specialObject1 = 0;
	gPhysics->magicObj = 0;
}

//---------------------------------------------------------------------------
//Crea planos verticales (fondos) con el material indicado
//---------------------------------------------------------------------------
void GameApplication::createFont(Ogre::String name, Ogre::Vector2 size, Ogre::Vector3 pos, Ogre::String material, Ogre::Vector2 repeat, bool unit)
{
	// unit = true, es un fondo // false, es un suelo
	Ogre::Vector3 vec1;
	Ogre::Vector3 vec2;

	if(unit){
		vec1 = Ogre::Vector3::UNIT_Z;
		vec2 = Ogre::Vector3::UNIT_Y;
	} 
	else {
		vec1 = Ogre::Vector3::UNIT_Y;
		vec2 = Ogre::Vector3::UNIT_Z;
	}

	//Plano
	Ogre::Plane plane(vec1, 0);
	Ogre::MeshPtr meshPtr = Ogre::MeshManager::getSingleton().createPlane(name, 
																		  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
																		  plane, size.x, size.y, 
																		  20, 20, true, 1, repeat.x, repeat.y, vec2);
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
	if (material != "")
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
	sg->setOrigin(Ogre::Vector3(vPos.x, vPos.y, vPos.z));

	for (Ogre::Real x = vPos.x - (vSize.x/2); x < (vPos.x + vSize.x/2); x += (vSize.x / amount))
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