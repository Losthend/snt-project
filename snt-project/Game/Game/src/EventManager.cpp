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

	//Otras variables
	platform = false;
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
	//EVENTOS DE ESCENARIOS: En proceso
	//****************************************************
	if(gGameApp->activeScene == 1){
		if(gPlayer != 0 && gPlayer->m_sceneObject->mNode->getPosition().y < -100)
			gGameApp->createScene2();
	}

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

	if(gGameApp->activeScene == 3){
		gSceneMgr->getSceneNode("templeOuroboros1")->roll(Ogre::Degree(Ogre::Real(0.1)));
		gSceneMgr->getSceneNode("templeOuroboros2")->yaw(Ogre::Degree(Ogre::Real(0.1)));

		SceneObject* specialObj1 = gGameApp->specialObject1;
		if(specialObj1 != 0)
		{
			Ogre::Vector3 posObj1 = specialObj1->mNode->getPosition();
			if (posObj1.y < 50)
				specialObj1->mRigidBody->applyCentralImpulse(btVector3(0, 50, 0));
			if (posObj1.x < -650)
				specialObj1->mRigidBody->applyCentralImpulse(btVector3(0.5, 0, 0));
			else if (posObj1.x > -650)
				specialObj1->mRigidBody->applyCentralImpulse(btVector3(-0.5, 0, 0));
		}

		SceneObject* specialObj2 = gGameApp->specialObject2;
		if(specialObj2 != 0)
		{
			Ogre::Real pos = specialObj2->mNode->getPosition().z;
			if (pos >= 0)
				platform = true;
			else if (pos <= -280)
				platform = false;

			if (platform)
				specialObj2->mRigidBody->translate(btVector3(0, 0, -3));
			else
				specialObj2->mRigidBody->translate(btVector3(0, 0, 2));
		}
	}

	//****************************************************
	//EVENTOS DE ESCENARIOS: En proceso
	//****************************************************

}