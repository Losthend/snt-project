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
	gGameApp->createScene1();
	gCCegui->gameMenu->d_root->hide();
	//*********************************************************************
	//Escenario 1
	actualText = 1;
	nextText = false;
	text_1_7 = false;
	text_8_9 = false;
	text_10 = false;
	text_11 = false;
	text_12_13 = false;
	//Escenario 3
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
		//Puerta del ascensor
		SceneObject* specialObj2 = gGameApp->specialObject0->m_sceneObject;
		if(gCanUpdate && specialObj2 != 0 && specialObj2->mNode->getPosition().y < 125){
			specialObj2->mRigidBody->translate(btVector3(0, btScalar(0.1), 0));
		}
		//Cambio de escenario
		if(gPlayer != 0 && gPlayer->m_sceneObject->mNode->getPosition().y < -100)
			gGameApp->createScene2();
		//Eventos de texto
		controlText1();
	}

	//****************************************************
	//ESCENARIO 2: Cueva
	//****************************************************
	if(gGameApp->activeScene == 2){
		//Iluminacion
		if(gPlayer != 0){
			Ogre::Real posX = gPlayer->m_sceneObject->mNode->getPosition().x;
			if ( posX > 1000 ){
				gSceneMgr->getLight("shadowLight1")->setCastShadows(false);
				gSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
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
		//Nucleo y cambio de escenario
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
				sceneObj = gPhysics->createPrimitiveShape("templeMagicBlock1", Ogre::Vector3(Ogre::Real(25*0.1), Ogre::Real(250*0.1), Ogre::Real(300*0.1)), Ogre::Vector3(475, 125, 0), 0, "Cube.mesh", true);
				sceneObj->mEntity->setMaterialName("Material/temple_Wall");
				gObjects.push_back(new Object(1, sceneObj));
				sceneObj = gPhysics->createPrimitiveShape("templeMagicBlock2", Ogre::Vector3(Ogre::Real(300*0.1), Ogre::Real(250*0.1), Ogre::Real(1*0.1)), Ogre::Vector3(650, 125, -150), 0, "Cube.mesh", true);
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

void EventManager::controlText1()
{
	Ogre::String string = "";
	//Introducción
	if(!text_1_7)
	{
		if(actualText == 1){
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			string = selectText1(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			gCanUpdate = false;
		}
		else if (actualText > 1 && actualText <= 7 && nextText){
			if(actualText == 7)
				gCCegui->dialogBox->getChildAtIdx(2)->setText("AYUDA");
			string = selectText1(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			nextText = false;
		}
		else if (actualText == 8 && nextText){
			gCCegui->dialogBox->getChildAtIdx(3)->setText("");
			gCCegui->dialogBox->getChildAtIdx(2)->setText("SUJETO 123");
			gCCegui->dialogBox->hide();
			text_1_7 = true;
			nextText = false;
			gCanUpdate = true;
		}
	}
	//Interaccion con la sangre
	if(!text_8_9)
	{
		//To-Do
		//Posicion del jugador (para mostrar el aviso): gPlayer->m_sceneObject->mNode->getPosition().x
	}

}

Ogre::String EventManager::selectText1(int textNumber)
{
	Ogre::String string = "";
	switch (textNumber)
	{
//Introducción
	case 1:
		string = "¿De qué me han servido tantos años de lealtad y sacrificio? De nada.";
		string = string	+ "\nDebí habermelo pensado dos veces antes de comprometerme con el proyecto.";
		actualText = 1;
		break;
	case 2:
		string = "Ya ni siquiera tengo nombre. Tan solo soy otro número, otra rata de laboratorio bajo";
		string = string	+ "\nla atenta mirada de Adolf Hitler.";
		actualText = 2;
		break;
	case 3:
		string = "Y todo para qué... ¿Para ganar la guerra? ¿Para dominar el mundo?";
		string = string	+ "\n¿Viajes en el tiempo como arma definitiva? Santo Dios...";
		actualText = 3;
		break;
	case 4:
		string = "Da igual, poco puedo hacer ya. Será mejor que me centre en la misión.";
		string = string	+ "\nSi logro encontrar la respuesta... quizá, solo quizá, me dejen marchar.";
		actualText = 4;
		break;
	case 5:
		string = "Segun recuerdo, debo desvelar 3 secretos para hallar la respuesta y poder salir.";
		string = string	+ "\nUn momento... ¿Respuesta? No recuerdo que hubiera una pregunta.";
		actualText = 5;
		break;
	case 6:
		string = "Estoy confuso. Joder... ¿Que me está pasando?";
		actualText = 6;
		break;
	case 7:
		string = "(Puedes acercar/alejar la cámara utilizando el SCROLL del ratón.)";
		string = string	+ "\n(Para moverte a la izquierda/derecha utiliza la tecla A/D, respectivamente.)";
		string = string	+ "\n(Pulsa \"E\" al ver el icono con dicha letra para interactuar con el entorno.)";
		actualText = 7;
		break;
//Interacción con la sangre
	case 8:
		string = "¿Esto es sangre? Parece reciente.";
		actualText = 8;
		break;
	case 9:
		string = "No tiene sentido, el último comunicado emitido por esta base se";
		string = string	+ "\nremonta a hace casi un año... será mejor andarse con cuidado.";
		actualText = 9;
		break;
//Interacción con la puerta
	case 10:
		string = "Esta cerrada... debo encontrar otra forma de avanzar.";
		actualText = 10;
		break;
//Primera interacción con el derrumbamiento
	case 11:
		string = "Parece un derrumbamiento. Será mejor pasar con cuidado.";
		string = string	+ "\n(Para saltar pulsa la tecla SPACE)";
		actualText = 11;
		break;
//Segunda interacción con el derrumbamiento
	case 12:
		string = "Puede que haya algo que se me escape. Será mejor revisar este lugar.";
		actualText = 12;
		break;
	case 13:
		string = "(Para mover objetos con telequinesis, realiza click derecho sobre el objeto)";
		string = string	+ "\ny, sosteniendo la pulsación, desplaza el ratón hacia el lugar donde desees llevarlo.";
		actualText = 13;
		break;
	default:
		break;
	}

	return string;
}

Ogre::String EventManager::selectText2(int textNumber)
{
	Ogre::String string = "";
				//"1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31"
	switch (textNumber)
	{
	case 1:
		string = "";
	case 2:
		string = "";
		break;
	case 3:
		string = "";
		break;
	case 4:
		string = "";
		break;
	case 5:
		string = "";
		break;
	case 6:
		string = "";
		break;
	case 7:
		string = "";
		break;
	case 8:
		string = "";
		break;
	case 9:
		string = "";
		break;
	case 10:
		string = "";
		break;
	case 11:
		string = "";
		break;
	case 12:
		string = "";
		break;
	default:
		break;
	}

	return string;
}