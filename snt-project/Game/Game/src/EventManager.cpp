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
	gGameApp->createScene3();
	gCCegui->gameMenu->d_root->hide();
	gCCegui->menu1->show();
	gCCegui->menu1->activate();
	//*********************************************************************

	//inExit: indica si se estan realizando los eventos de salida del escenario 3
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
			if(specialObj1->mNode->getPosition().x > 650 && gPlayer->m_sceneObject->mNode->getPosition().x > 500)
			{
				gGameApp->specialObject1->~Object();
				SceneObject*sceneObj = gPhysics->createConvexHullShape("templeKey", Ogre::Real(0.2), Ogre::Vector3(650, 115, 0), 0, "geosphere4500.mesh");
				gGameApp->specialObject1 = new Object(1, sceneObj);
				gObjects.push_back(gGameApp->specialObject1);
				inExit = true;
			}
		}
		else{
			//Cambio de escenario con animacion
			Ogre::Vector3 scale = specialObj1->mNode->getScale();
			specialObj1->mNode->setScale(Ogre::Real(scale.x/1.001), Ogre::Real(scale.y/1.001), Ogre::Real(scale.z/1.001));
			Ogre::Real x = scale.x*1000;
			if( x >= 160 ){
				gSceneMgr->getSceneNode("templeOuroboros1")->roll(Ogre::Degree(Ogre::Real(0.5)));
				gSceneMgr->getSceneNode("templeOuroboros2")->yaw(Ogre::Degree(Ogre::Real(0.5)));
			}
			else if( x < 160 && x >= 150 ){
				gSceneMgr->getSceneNode("templeOuroboros1")->roll(Ogre::Degree(Ogre::Real(1)));
				gSceneMgr->getSceneNode("templeOuroboros2")->yaw(Ogre::Degree(Ogre::Real(1)));
				specialObj1->mEntity->setMaterialName("Material/temple_Energy2");
			}
			else if( x < 150 && x >= 130 ){
				gSceneMgr->getSceneNode("templeOuroboros1")->roll(Ogre::Degree(Ogre::Real(1.5)));
				gSceneMgr->getSceneNode("templeOuroboros2")->yaw(Ogre::Degree(Ogre::Real(1.5)));
				specialObj1->mEntity->setMaterialName("Material/temple_Energy3");
			}
			else if( x < 130 && x >= 110 ){
				gSceneMgr->getSceneNode("templeOuroboros1")->roll(Ogre::Degree(Ogre::Real(2)));
				gSceneMgr->getSceneNode("templeOuroboros2")->yaw(Ogre::Degree(Ogre::Real(2)));
				specialObj1->mEntity->setMaterialName("Material/temple_Energy4");
			}
			else if( x < 110 && x >= 80 ){
				gSceneMgr->getSceneNode("templeOuroboros1")->roll(Ogre::Degree(Ogre::Real(2.5)));
				gSceneMgr->getSceneNode("templeOuroboros2")->yaw(Ogre::Degree(Ogre::Real(2.5)));
				specialObj1->mEntity->setMaterialName("Material/temple_Energy5");
			}
			else if( x < 80 ){
				//Cambio de escenario
			}
		}
	}
}