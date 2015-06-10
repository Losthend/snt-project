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
	gGameApp->createScene2();
	gCCegui->gameMenu->d_root->hide();
	gCCegui->menu1->show();
	gCCegui->menu1->activate();
	//*********************************************************************
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

		//CONTROL DE ILUMINACION
		if(gPlayer != 0){
			Ogre::Real posX = gPlayer->m_sceneObject->mNode->getPosition().x;
			if ( posX < 950 && !gSceneMgr->getLight("shadowLight0")->getCastShadows()){
				gSceneMgr->getLight("shadowLight0")->setCastShadows(true);
				gSceneMgr->getLight("shadowLight1")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight4")->setCastShadows(false);
			}
			else if (posX > 1000 && posX < 1200 && !gSceneMgr->getLight("shadowLight1")->getCastShadows()){
				gSceneMgr->getLight("shadowLight0")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight1")->setCastShadows(true);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight4")->setCastShadows(false);
			}
			else if (posX > 1200 && posX < 1400 && !gSceneMgr->getLight("shadowLight2")->getCastShadows()){
				gSceneMgr->getLight("shadowLight0")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight1")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(true);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight4")->setCastShadows(false);
			}
			else if (posX > 1500 && posX < 1700 && !gSceneMgr->getLight("shadowLight3")->getCastShadows()){
				gSceneMgr->getLight("shadowLight0")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight1")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(true);
				gSceneMgr->getLight("shadowLight4")->setCastShadows(false);
			}
			else if (posX > 1700 && posX < 1900 && !gSceneMgr->getLight("shadowLight4")->getCastShadows()){
				gSceneMgr->getLight("shadowLight0")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight1")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight2")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight3")->setCastShadows(false);
				gSceneMgr->getLight("shadowLight4")->setCastShadows(true);
			}
		}
		//FIN CONTROL ILUMINACION

	}

	//****************************************************
	//EVENTOS DE ESCENARIOS: En proceso
	//****************************************************

}