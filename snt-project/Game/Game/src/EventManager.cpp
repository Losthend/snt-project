#include "../include/stdafx.h"

#include "../include/EventManager.h"
#include "../include/Global.h"
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/CCegui.h"
#include "../include/GameMenu.h"
#include "../include/SceneObject.h"
#include "../include/PhysicsManager.h"
#include "../include/GameApplication.h"

EventManager::EventManager(void)
{
	//Al comenzar, por defecto, no se permite actualizar fisicas y jugador
	gCanUpdate = false;

	//*********************************************************************
	//ELIMINAR: Sustituye a la accion del boton START del menu
	gCCegui->gameMenu->shouldBeDisplayed = false;
	gCanUpdate = true;
	gGameApp->createScene4();
	gCCegui->gameMenu->d_root->hide();
	gCCegui->menu1->show();
	gCCegui->menu1->activate();
	//*********************************************************************

	//inExit: indica si se estan realizando los eventos de salida del escenario 3
	inExitCount = 0;
	inExit = false;
}

EventManager::~EventManager(void)
{
}

void EventManager::handleEvent()
{
	//****************************************************
	//Menu principal: Si no se puede realizar update, el menu principal debe mostrarse y no se esta mostrando
	//****************************************************
	if( !gCanUpdate && gCCegui->gameMenu->shouldBeDisplayed && !(gCCegui->gameMenu->d_root->isActive()) )
	{
		//Mostrar el menu principal
		gCCegui->gameMenu->d_root->show();
		gCCegui->gameMenu->d_root->activate();
		gCCegui->gameMenu->onEnteringSample();
	}

	//****************************************************
	//UPDATE: fisicas y movimiento del jugador
	//****************************************************
	if(gCanUpdate && gPlayer != 0)
	{
		//Control de fisica y colisiones bullet: Objeto y jugador
		gPhysics->update(float(0.017));
		for(int i = 0, len = gObjects.size(); i < len; i++)
			gObjects[i]->update();
		gPlayer->update();
	}

	//****************************************************
	//EVENTOS DE ESCENARIOS
	//****************************************************

	//****************************************************
	//ESCENARIO 1: Bunker
	//****************************************************
	if(gGameApp->activeScene == 1){
		//Cambio de escenario
		if(gPlayer != 0 && gPlayer->m_sceneObject->mNode->getPosition().y < -100)
			gGameApp->createScene2();
	}

	//****************************************************
	//ESCENARIO 2: Cueva
	//****************************************************
	if(gGameApp->activeScene == 2){
		//Iluminacion
		if(gPlayer != 0){
			Ogre::Real posX = gPlayer->m_sceneObject->mNode->getPosition().x;
			if ( posX < 950 && !gSceneMgr->getLight("shadowLight1")->getCastShadows()){
				gSceneMgr->getLight("shadowLight1")->setCastShadows(true);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(false);
			}
			else if (posX > 950 && posX < 1400 && !gSceneMgr->getLight("shadowLight2")->getCastShadows()){
				gSceneMgr->getLight("shadowLight1")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(true);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(false);
			}
			else if (posX > 1500 && posX < 1700 && !gSceneMgr->getLight("shadowLight3")->getCastShadows()){
				gSceneMgr->getLight("shadowLight1")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(true);
			}

		//Cambio de escenario
		if(gPlayer->m_sceneObject->mNode->getPosition().y < -600)
			gGameApp->createScene3();
		}
	}

	//****************************************************
	//ESCENARIO 3: Templo
	//****************************************************
	if(gGameApp->activeScene == 3){
		doScene3();
	}

	//****************************************************
	//ESCENARIO 4: Final
	//****************************************************
	if(gGameApp->activeScene == 4){
		//TO-DO
	}
}

void EventManager::doScene3()
{
	//NUCLEO DE ENERGIA + CAMBIO DE ESCENARIO
	SceneObject* specialObj1 = gGameApp->specialObject1->m_sceneObject;
	if(specialObj1 != 0)
	{
		if (!inExit){
			Ogre::Vector3 posObj1 = specialObj1->mNode->getPosition();
			if (posObj1.y < 50)
				specialObj1->mRigidBody->applyCentralImpulse(btVector3(0, 50, 0));
			if (posObj1.x < -650)
				specialObj1->mRigidBody->applyCentralImpulse(btVector3(0.5, 0, 0));
			else if (posObj1.x > -650)
				specialObj1->mRigidBody->applyCentralImpulse(btVector3(-0.5, 0, 0));
			//Cambio de escenario: activacion de la animacion
			if(specialObj1->mNode->getPosition().x > 650 && gPlayer->m_sceneObject->mNode->getPosition().x > 575)
			{
				gGameApp->specialObject1->~Object();
				SceneObject*sceneObj = gPhysics->createConvexHullShape("templeKey", Ogre::Real(0.2), Ogre::Vector3(650, 115, 0), 0, "geosphere4500.mesh");
				gGameApp->specialObject1 = new Object(1, sceneObj);
				gObjects.push_back(gGameApp->specialObject1);
				sceneObj = gPhysics->createPrimitiveShape("templeMagicBlock1", Ogre::Vector3(Ogre::Real(25*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(475, 125, 0), 0, "Cube.mesh");
				sceneObj->mEntity->setMaterialName("Material/temple_Wall");
				gObjects.push_back(new Object(1, sceneObj));
				sceneObj = gPhysics->createPrimitiveShape("templeMagicBlock2", Ogre::Vector3(Ogre::Real(300*0.1), Ogre::Real(250*0.1), Ogre::Real(1*0.1)), Ogre::Vector3(650, 125, -150), 0, "Cube.mesh");
				sceneObj->mEntity->setMaterialName("Material/temple_Wall");
				gObjects.push_back(new Object(1, sceneObj));
				inExit = true;
				inExitCount++;
			}
		}
		else{
			//Cambio de escenario con animacion
			Ogre::Vector3 scale = specialObj1->mNode->getScale();
			Ogre::Real x = scale.x*1000;
			if (inExitCount != 5)
				specialObj1->mNode->setScale(Ogre::Real(scale.x/1.001), Ogre::Real(scale.y/1.001), Ogre::Real(scale.z/1.001));
			else
				specialObj1->mNode->setScale(Ogre::Real(scale.x*1.05), Ogre::Real(scale.y*1.05), Ogre::Real(scale.z*1.05));

			gSceneMgr->getSceneNode("templeOuroboros1")->roll(Ogre::Degree(Ogre::Real(inExitCount*2)));
			gSceneMgr->getSceneNode("templeOuroboros2")->yaw(Ogre::Degree(Ogre::Real(inExitCount*2)));

			if(inExitCount == 1 && x > 150){
				gSceneMgr->getLight("templeLight")->setVisible(true);
				gSceneMgr->getLight("templeLight")->setDiffuseColour(0, 0, 1);
				inExitCount++;
			}
			else if(inExitCount == 2 && x < 150){
				gSceneMgr->getLight("templeLight")->setDiffuseColour(0, 1, 0);
				specialObj1->mEntity->setMaterialName("Material/temple_Energy2");
				inExitCount++;
			}
			else if( inExitCount == 3 && x < 100){
				gSceneMgr->getLight("templeLight")->setDiffuseColour(1, 0, 0);
				gSceneMgr->getSceneNode("templeFarWall1")->detachAllObjects();	
				gSceneMgr->getSceneNode("templeNearWall1")->detachAllObjects();	
				gSceneMgr->getEntity("templeRoof")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeFloor")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeRight")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeMagicBlock1")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeMagicBlock2")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeColumn2")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeColumn3")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeColumn3")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeLockSupport1")->setMaterialName("Material/temple_Energy4");
				gSceneMgr->getEntity("templeLockSupport2")->setMaterialName("Material/temple_Energy4");
				specialObj1->mEntity->setMaterialName("Material/temple_Energy3");
				inExitCount++;
			}
			else if( inExitCount == 4 && x < 75){
				gSceneMgr->getLight("templeLight")->setDiffuseColour(0.5, 0.5, 0.5);
				specialObj1->mEntity->setMaterialName("Material/temple_Energy4");
				inExitCount++;
			}
			else if( x > 2000 ){
				//Cambio de escenario
				inExitCount = 0;
				inExit = false;
				gGameApp->createScene4();
			}
		}
	}
}