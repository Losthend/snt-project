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
	gSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
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
	createFont("elevatorWall2", Ogre::Vector2(100,100), Ogre::Vector3(0,50,-75), "Material/elevator_Wall", Ogre::Vector2(1,1), true);
	createFont("elevatorNote", Ogre::Vector2(40,40), Ogre::Vector3(0,50,-74), "Material/elevator_Note", Ogre::Vector2(1,1), true);

	sceneObj = gPhysics->createGroundShape("elevatorFloor", Ogre::Vector3(100, 5, 150), Ogre::Vector3(0, 0, 0), Ogre::Vector2(1,1), "Material/elevator_Floor");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("elevatorWall1", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(100*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(-50, 50, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/elevator_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 
	sceneObj = gPhysics->createPrimitiveShape("elevatorRoof", Ogre::Vector3(Ogre::Real(100*0.1), Ogre::Real(5*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(0, 100, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/elevator_Floor");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("elevatorDoor", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(100*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(49, 150, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/elevator_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	
	//PUERTA ASCENSOR
	createObject("elevatorButton", Ogre::Vector3(Ogre::Real(3*0.01), Ogre::Real(15*0.01), Ogre::Real(15*0.01)), Ogre::Vector3(51, 40, 50), "Material/elevator_Button","Cube.mesh");

	sceneObj = gPhysics->createPrimitiveShape("elevatorDoorNear", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(75*0.1), Ogre::Real(50*0.1)), Ogre::Vector3(51, 35, 50), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("elevatorDoorFar", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(75*0.1), Ogre::Real(50*0.1)), Ogre::Vector3(51, 35, -50), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createPrimitiveShape("elevatorDoorTop", Ogre::Vector3(Ogre::Real(2*0.1), Ogre::Real(175*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(51, 160, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 

	//SALA PRINCIPAL
	createFont("bunkerFont1", Ogre::Vector2(900,250), Ogre::Vector3(500,125,-75), "Material/bunker_Wall", Ogre::Vector2(8,2), true);
	createFont("bunkerFont2", Ogre::Vector2(900,250), Ogre::Vector3(500,-125,75), "Material/bunker_Wall", Ogre::Vector2(8,2), true);

	sceneObj = gPhysics->createGroundShape("bunkerFloor1", Ogre::Vector3(530, 5, 150), Ogre::Vector3(315, 0, 0), Ogre::Vector2(5,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	sceneObj = gPhysics->createGroundShape("bunkerFloor2", Ogre::Vector3(260, 5, 150), Ogre::Vector3(820, 0, 0), Ogre::Vector2(2,1), "Material/bunker_Wall");
	gObjects.push_back(new Object(1, sceneObj));
	
	sceneObj = gPhysics->createPrimitiveShape("bunkerWall1", Ogre::Vector3(5*0.1, 250*0.1, 150*0.1), Ogre::Vector3(950, 125, 0), 0, "Cube.mesh");
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
	transform.setOrigin(btVector3(0, 150, 0));
	gPlayer->m_sceneObject->mRigidBody->setCenterOfMassTransform(transform);

	//****************************************************
	//ESCENARIO 2
	//****************************************************
	SceneObject* sceneObj;

	//Punto de caida
	sceneObj = gPhysics->createConvexHullShape("stone", Ogre::Real(100), Ogre::Vector3(5, 25, 0), 0, "stone.mesh");
	sceneObj->mEntity->setMaterialName("Material/stone2");
	gObjects.push_back(new Object(1, sceneObj));

	//ZONA 1: sala del oso
	createFont("undergroundFarWall1", Ogre::Vector2(1000,250), Ogre::Vector3(450,125,-75), "Material/underground_Wall", Ogre::Vector2(1,1), true);
	createFont("undergroundNearWall1", Ogre::Vector2(1000,500), Ogre::Vector3(450,-250,75), "Material/underground_Wall", Ogre::Vector2(1,2), true);

	sceneObj = gPhysics->createGroundShape("undergroundFloor1", Ogre::Vector3(900, 5, 150), Ogre::Vector3(500, 0, 0), Ogre::Vector2(5,1), "Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	sceneObj = gPhysics->createPrimitiveShape("undergroundLeft", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(250*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(-50, 125, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj)); 

	//ZONA 1: techo
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

	sceneObj = gPhysics->createPrimitiveShape("undergroundCenter", Ogre::Vector3(Ogre::Real(5*0.1), Ogre::Real(300*0.1), Ogre::Real(150*0.1)), Ogre::Vector3(950, -150, 0), 0, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	gObjects.push_back(new Object(1, sceneObj));

	createObject("undergroundWall1", Ogre::Vector3(Ogre::Real(5*0.01), Ogre::Real(550*0.01), Ogre::Real(550*0.01)), Ogre::Vector3(950, -25, -350), "Material/underground_Wall","Cube.mesh");

	//ZONA 2: techo
	//sceneObj = gPhysics->createPrimitiveShape("undergroundRoof", Ogre::Vector3(1000*0.1, 5*0.1, 150*0.1), Ogre::Vector3(1400, 250, 0), 0, "Cube.mesh");
	//sceneObj->mEntity->setMaterialName("Material/underground_Wall");
	//gObjects.push_back(new Object(1, sceneObj)); 

	//Zona 3: Precipicio


	//DECORACION
	createFont("undergroundEntrance", Ogre::Vector2(200,100), Ogre::Vector3(400,30,-74), "Material/underground_Entrance", Ogre::Vector2(1,1), true);
	
	

	sceneObj = gPhysics->createPrimitiveShape("stone1", Ogre::Vector3(100*0.1, 5*0.1, 150*0.1), Ogre::Vector3(800, 10, 0), 60, "Cube.mesh");
	sceneObj->mEntity->setMaterialName("Material/bunker_Wall");
	gObjects.push_back(new Object(2, sceneObj)); 

	//ENEMIGO
	/*
	sceneObj = gPhysics->createConvexHullShape("bear", Ogre::Real(40), Ogre::Vector3(200, 50, 0), 10, "bear.mesh");
	//Mirar inicialmente hacia el eje X+
	transform;
	sceneObj->mRigidBody->getMotionState()->getWorldTransform(transform);
	transform.setRotation(btQuaternion(0,1,0,1));
	sceneObj->mRigidBody->setMotionState(new btDefaultMotionState(transform));
	//Guardar en global
	gObjects.push_back(new Object(4, sceneObj));
	*/

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

	//LUZ 0 
	Ogre::Light *light = gSceneMgr->createLight("shadowLight0");     
	light->setType(Ogre::Light::LT_SPOTLIGHT);
	light->setPosition(0, 249, 0);
	light->setDirection(0,-1,0);
	light->setSpotlightRange(Ogre::Degree(30), Ogre::Degree(50));	

	//LUZ 1
	Ogre::Vector3 lightPos(1100, -280, -600);
	createObject("cubeLeft1", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x-2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeRight1", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x+2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBack1", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y, lightPos.z-10), "Material/underground_Wall","Cube.mesh");
	createObject("cubeTop1", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y+10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBottom1", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y-10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	Ogre::Light *light1 = gSceneMgr->createLight("shadowLight1");     
	light1->setType(Ogre::Light::LT_SPOTLIGHT);
	light1->setPosition(lightPos);
	light1->setDirection(0,0,1);
	light1->setDiffuseColour(1.0, 0.0, 0.0);     
	light1->setSpecularColour(1.0, 0.0, 0.0);   
	light1->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(100));
	light1->setCastShadows(false);

	//LUZ 2
	lightPos = Ogre::Vector3(1300, -290, -600);
	createObject("cubeLeft2", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x-2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeRight2", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x+2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBack2", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y, lightPos.z-10), "Material/underground_Wall","Cube.mesh");
	createObject("cubeTop2", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y+10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBottom2", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y-10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	Ogre::Light *light2 = gSceneMgr->createLight("shadowLight2");     
	light2->setType(Ogre::Light::LT_SPOTLIGHT);
	light2->setPosition(lightPos);
	light2->setDirection(0,0,1);
	light2->setDiffuseColour(0.0, 1.0, 0.0);     
	light2->setSpecularColour(0.0, 1.0, 0.0);   
	light2->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(100));
	light2->setCastShadows(false);

	//LUZ 3
	lightPos = Ogre::Vector3(1500, -290, -600);
	createObject("cubeLeft3", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x-2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeRight3", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x+2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBack3", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y, lightPos.z-10), "Material/underground_Wall","Cube.mesh");
	createObject("cubeTop3", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y+10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBottom3", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y-10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	Ogre::Light *light3 = gSceneMgr->createLight("shadowLight3");     
	light3->setType(Ogre::Light::LT_SPOTLIGHT);
	light3->setPosition(lightPos);
	light3->setDirection(0,0,1);
	light3->setDiffuseColour(0.0, 0.0, 1.0);     
	light3->setSpecularColour(0.0, 0.0, 1.0);   
	light3->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(100));
	light3->setCastShadows(false);

	//LUZ 4
	lightPos = Ogre::Vector3(1700, -290, -600);
	createObject("cubeLeft4", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x-2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeRight4", Ogre::Vector3(Ogre::Real(2*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x+2, lightPos.y, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBack4", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y, lightPos.z-10), "Material/underground_Wall","Cube.mesh");
	createObject("cubeTop4", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(5*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y+10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	createObject("cubeBottom4", Ogre::Vector3(Ogre::Real(10*0.01), Ogre::Real(10*0.01), Ogre::Real(10*0.01)), Ogre::Vector3(lightPos.x, lightPos.y-10, lightPos.z), "Material/underground_Wall","Cube.mesh");
	Ogre::Light *light4 = gSceneMgr->createLight("shadowLight4");     
	light4->setType(Ogre::Light::LT_SPOTLIGHT);
	light4->setPosition(lightPos);
	light4->setDirection(0,0,1);
	light4->setDiffuseColour(1.0, 1.0, 1.0);     
	light4->setSpecularColour(1.0, 1.0, 1.0);   
	light4->setSpotlightRange(Ogre::Degree(10), Ogre::Degree(100));
	light4->setCastShadows(false);
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