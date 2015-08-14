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
	gCanUpdateKeyboard = false;
	//*********************************************************************
	//ELIMINAR: Sustituye a la accion del boton START del menu
	//gCCegui->gameMenu->shouldBeDisplayed = false;
	//gCanUpdate = true;
	//gCanUpdateKeyboard = true;
	//gGameApp->createScene2();
	//gCCegui->gameMenu->d_root->hide();
	//*********************************************************************
	//General
	pulseQ = false;
	//Escenario 1
	actualText = 1;
	nextText = false;
	text_1_7 = false;
	text_8_9 = false;
	text_10 = false;
	text_11 = false;
	text_12_13 = false;
	//Escenario 2
	text_1_3 = false;
	text_4_5 = false;
	text_6_7 = false;
	//Escenario 3
	inExitCount = 0;
	inExit = false;
	text_1_4 = false;
	text_5 = false;
	text_6_7_8 = false;
	magic = false;
	//Escenario 4
	text_1_14 = false;
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
	//DEADBOX: Control de la muerte del jugador
	//****************************************************+
	if(gPlayer != 0 && gPlayer->m_lifeCounter <= 0 && !(gCCegui->deadBox->isActive())){
		gCanUpdate = false;
		gCCegui->dialogBox->hide();
		gCCegui->deadBox->activate();
		gCCegui->deadBox->show();
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
		//Eventos de texto
		controlText1();
		//Cambio de escenario
		if(gPlayer != 0 && gPlayer->m_sceneObject->mNode->getPosition().y < -100){
			gCCegui->alertBox->hide();
			gGameApp->createScene2();
		}
	}

	//****************************************************
	//ESCENARIO 2: Cueva
	//****************************************************
	if(gGameApp->activeScene == 2){
		//Eventos de texto
		controlText2();
		if(gPlayer != 0 && text_6_7 && actualText == 0){
			//Cambio de escenario
			gCCegui->alertBox->hide();
			gGameApp->createScene3();
		}
		//Control de muerte
		if(gPlayer->m_sceneObject->mNode->getPosition().y < -500)
			gPlayer->m_lifeCounter = 0;
	}

	//****************************************************
	//ESCENARIO 3: Templo
	//****************************************************
	if(gGameApp->activeScene == 3){
		controlText3();
		if (gCanUpdate)
			doScene3();
	}

	//****************************************************
	//ESCENARIO 4: Final
	//****************************************************
	if(gGameApp->activeScene == 4){
		controlText4();
	}
}

//****************************************************
//Acciones especiales a realizar durante el escenario 3
//****************************************************
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
				gSceneMgr->getLight("shadowLight")->setVisible(false);
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
				gCCegui->alertBox->hide();
				gGameApp->createScene4();
			}
		}
	}
}

//****************************************************
//Control de los eventos de texto en el escenario 1
//****************************************************
void EventManager::controlText1()
{
	Ogre::String string = "";
	Ogre::Real posX = gPlayer->m_sceneObject->mNode->getPosition().x;

	//INTRODUCCIÓN
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
			string = selectText1(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			if(actualText == 7){
				gCCegui->dialogBox->getChildAtIdx(2)->setText("AYUDA");
				actualText = 0;
			}
			else
				actualText++;
			nextText = false;
		}
		else if (actualText == 0 && nextText){
			gCCegui->dialogBox->getChildAtIdx(3)->setText("");
			gCCegui->dialogBox->getChildAtIdx(2)->setText("SUJETO 123");
			gCCegui->dialogBox->hide();
			text_1_7 = true;
			nextText = false;
			gCanUpdate = true;
		}
	}
	
	//Derrumbamiento, sangre y puerta
	bool block = ( ((!text_11 && !text_10)||(!text_12_13 && text_10)) && posX > 550 && posX < 750 );
	bool blood = (!text_8_9 && posX > 775 && posX < 825);
	bool door = (!text_10 && posX > 900 && posX < 950);
	if ( block || blood || door ){ 
		//Activacion del alertBox
		if( !(gCCegui->alertBox->isActive()) ){
			gCCegui->alertBox->activate();
			gCCegui->alertBox->show();
		}
		//Pulsacion de Q
		if (pulseQ){
			//Activar dialogo
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			//Primera interaccion (requisito: no haber interactuado con la puerta)
			if(block){
				if (!text_11 && !text_10){
					actualText = 11;
					text_11 = true;
				}
				//Segunda interaccion (requisito: haber interactuado con la puerta)
				else if (!text_12_13 && text_10){
					actualText = 12;
					text_12_13 = true;
				}
			}
			else if (blood){
				actualText = 8;
				text_8_9 = true;
			}
			else{
				actualText = 10;
				text_10 = true;
			}
			//Mostrar dialogo
			string = selectText1(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			//Deshabilitar alertBox y updates
			gCCegui->alertBox->hide();
			pulseQ = false;
			gCanUpdate = false;
		}
	}
	else
		gCCegui->alertBox->hide();

	//Finalización de dialogos
	if ( ( (text_10 && actualText == 10)||
			(text_11 && actualText == 11)||
			(text_8_9 && actualText == 9)||
			(text_12_13 && actualText == 13) ) && nextText){
		gCCegui->dialogBox->hide();
		gCanUpdate = true;
		nextText = false;
		pulseQ = false;
		if(actualText == 13)
			gCCegui->dialogBox->getChildAtIdx(2)->setText("SUJETO 123");
		actualText = 0;
	}
	//Avance de dialogos
	else if ( ( (text_8_9 && actualText == 8)||
				(text_12_13 && actualText == 12)) && nextText){
		actualText++;
		if(actualText == 13)
			gCCegui->dialogBox->getChildAtIdx(2)->setText("AYUDA");
		string = selectText1(actualText);
		gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
		nextText = false;
	}
}

//****************************************************
//Eventos de texto del escenario 1
//****************************************************
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
		string = "Y todo para qué, ¿Para ganar la guerra? Admito que la idea de cambiar el pasado";
		string = string	+ "\nes brillante y tentadora, el arma definitiva... pero, ¿A que precio?";
		actualText = 3;
		break;
	case 4:
		string = "Da igual, poco puedo hacer ya. Será mejor que me centre en la misión.";
		string = string	+ "\nSi logro encontrar lo que me pidieron... quizá me dejen abandonar todo esto.";
		actualText = 4;
		break;
	case 5:
		string = "Debo encontrar el prototipo de una fuente de energía alternativa en la que";
		string = string	+ "\nestaban trabajando las \"SS\" antes... antes de perder toda comunicación con la";
		string = string	+ "\nbase. Antes del... incidente.";
		actualText = 5;
		break;
	case 6:
		string = "Decenas de personas desaparecidas de la noche a la mañana. Ningún cuerpo,";
		string = string	+ "\nninguna petición de ayuda... dios... debí habermelo pensado mejor...";
		actualText = 6;
		break;
	case 7:
		string = "Puedes acercar/alejar la cámara utilizando el SCROLL del ratón.";
		string = string	+ "\nPara moverte a la izquierda/derecha utiliza la tecla A/D, respectivamente.";
		string = string	+ "\nPulsa \"Q\" al ver el icono con dicha letra para interactuar con el entorno.";
		actualText = 7;
		break;
//Interacción con la sangre
	case 8:
		string = "¿Esto es sangre? Parece reciente.";
		actualText = 8;
		break;
	case 9:
		string = "No tiene sentido, el último comunicado emitido por esta base se";
		string = string	+ "\nremonta a hace casi un mes... será mejor andarse con cuidado.";
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
		string = "Para mover objetos con telequinesis, realiza click derecho sobre el objeto y,";
		string = string	+ "\nsosteniendo la pulsación, desplaza el ratón hacia el lugar donde desees moverlo.";
		string = string	+ "\nRealiza click izquierdo, mientras sostienes la pulsación, para lanzar el objeto.";
		actualText = 13;
		break;
	default:
		break;
	}

	return string;
}

//****************************************************
//Control de los eventos de texto en el escenario 2
//****************************************************
void EventManager::controlText2()
{
	Ogre::String string = "";
	Ogre::Real posX = gPlayer->m_sceneObject->mNode->getPosition().x;

	//OSO
	if(!text_1_3 && posX > 1200)
	{
		if(actualText == 0 || actualText == 1){
			if (actualText == 0)
				actualText++;
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			string = selectText2(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			gCanUpdate = false;
		}
		else if (actualText == 2 && nextText){
			string = selectText2(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			nextText = false;
		}
		else if (actualText == 3 && nextText){
			gCCegui->dialogBox->getChildAtIdx(2)->setText("AYUDA");
			string = selectText2(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			nextText = false;
		}
		else if(actualText == 4 && nextText){
			gCCegui->dialogBox->getChildAtIdx(2)->setText("SUJETO 123");
			gCCegui->dialogBox->hide();
			text_1_3 = true;
			nextText = false;
			gCanUpdate = true;
			actualText = 0;
		}
	}

	//PUENTE Y ESPADA
	bool bridge = (!text_4_5 && posX > 1750 && posX < 1850);
	bool sword = (!text_6_7 && posX > 2400 && posX < 2500);
	if ((bridge || sword) && !(gCCegui->deadBox->isActive())){
		if( !(gCCegui->alertBox->isActive()) ){
			gCCegui->alertBox->activate();
			gCCegui->alertBox->show();
		}
		if (pulseQ){
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			if(bridge){
				actualText = 4;
				text_4_5 = true;
			}
			else{
				actualText = 6;
				text_6_7 = true;
			}
			//Mostrar dialogo
			string = selectText2(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			//Deshabilitar alertBox y updates
			gCCegui->alertBox->hide();
			pulseQ = false;
			gCanUpdate = false;
		}
	}
	else
		gCCegui->alertBox->hide();

	//Finalización de dialogos
	if ( ( (text_4_5 && actualText == 5)||
			(text_6_7 && actualText == 7) ) && nextText){
		gCCegui->dialogBox->hide();
		gCanUpdate = true;
		nextText = false;
		actualText = 0;
	}
	//Avance de dialogos
	else if ( ( (text_4_5 && actualText == 4)||
				(text_6_7 && actualText == 6) ) && nextText){
		actualText++;
		string = selectText2(actualText);
		gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
		nextText = false;
	}

}
//****************************************************
//Eventos de texto del escenario 2
//****************************************************
Ogre::String EventManager::selectText2(int textNumber)
{
	Ogre::String string = "";
	switch (textNumber)
	{
	//OSO
	case 1:
		string = "¡Pero que...!";
		break;
	case 2:
		string = "¿Cómo demonios ha llegado un oso hasta aquí? No se si podré esquivarlo.";
		string = string	+ "\nSi me ve, me atacará. Puedo intentar matarlo o saltar por encima suyo y huir.";
		break;
	case 3:
		string = "Ataques: pulsa la tecla \"E\" para desenvainar/envainar tus espadas.";
		string = string	+ "\nCon las espadas fuera, realiza click izquierdo (ratón) para realizar un ataque.";
		break;
	//Puente
	case 4:
		string = "Piedras voladoras... ¿Ingeniería nazi o tecnología extraterrestre?";
		string = string	+ "\nNo quiero saberlo, bastante tengo con los viajes en el tiempo.";
		break;
	case 5:
		string = "Creo que puedo utilizarlas a modo de puente, aunque no parecen muy";
		string = string	+ "\nestables... siento como si la gravedad de este lugar fuera distinta.";
		break;
	//Espada
	case 6:
		string = "Esta espada... es idéntica a la mía. Solo fueron otorgadas a aquellos que nos";
		string = string	+ "\npresentamos voluntarios para el proyecto.";
		break;
	case 7:
		string = "¿Acaso no soy el único al que han enviado aquí? Sea de quien sea, está";
		string = string	+ "\nfuertemente clavada en la piedra... quizá si tiro de ella...";
		break;
	default:
		break;
	}

	return string;
}


//****************************************************
//Control de los eventos de texto en el escenario 3
//****************************************************
void EventManager::controlText3()
{
	Ogre::String string = "";
	Ogre::Real posX = gPlayer->m_sceneObject->mNode->getPosition().x;

	//Entrada al templo
	if(!text_1_4 )
	{
		if(actualText == 0 || actualText == 1){
			if (actualText == 0)
				actualText++;
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			string = selectText3(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			gCanUpdateKeyboard = false;
			gPlayer->m_direction.x = 0;
		}
		else if ( (actualText == 2 || actualText == 3 ||actualText == 4) && nextText){
			string = selectText3(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			nextText = false;
		}
		else if(actualText == 5 && nextText){
			gCCegui->dialogBox->hide();
			text_1_4 = true;
			nextText = false;
			gCanUpdateKeyboard = true;
			actualText = 0;
		}
	}

	//Altar y energía
	bool altar = (!text_5 && posX > 500 && posX < 600);
	bool energy = (!text_6_7_8 && posX > -600 && posX < -500);
	if ((altar || energy) && !(gCCegui->deadBox->isActive())){
		if( !(gCCegui->alertBox->isActive()) ){
			gCCegui->alertBox->activate();
			gCCegui->alertBox->show();
		}
		if (pulseQ){
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			if(altar){
				actualText = 5;
				text_5 = true;
			}
			else{
				actualText = 6;
				text_6_7_8 = true;
			}
			//Mostrar dialogo
			string = selectText3(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			//Deshabilitar alertBox y updates
			gCCegui->alertBox->hide();
			pulseQ = false;
			gCanUpdate = false;
		}
	}
	else
		gCCegui->alertBox->hide();

	//Finalización de dialogos
	if ( ( (text_6_7_8 && actualText == 8)||
			(text_5 && actualText == 5) ) && nextText){
		gCCegui->dialogBox->hide();
		gCanUpdate = true;
		nextText = false;
		actualText = 0;
	}
	//Avance de dialogos
	else if ( ( (text_6_7_8 && actualText == 6) ||
				(text_6_7_8 && actualText == 7) ) && nextText){
		actualText++;
		string = selectText3(actualText);
		gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
		nextText = false;
	}
}


//****************************************************
//Eventos de texto del escenario 3
//****************************************************
Ogre::String EventManager::selectText3(int textNumber)
{
	Ogre::String string = "";
	switch (textNumber)
	{
	//Entrada al templo
	case 1:
		string = "¡Pero que...!";
		break;
	case 2:
		string = "Un templo egipcio, vale, ahora todo tiene sentido... ¿¡Sentido!?";
		string = string	+ "\nEnserio, ¿Que demonios está pasando aquí?¿Que es este lugar?";
		break;
	case 3:
		string = "Dijeron que era un centro de investigación sobre fuentes de energía alternativas.";
		string = string	+ "\nNadie dijo nada de piedras voladoras y teletransporte a templos misteriosos.";
		break;
	case 4:
		string = "No, algo no encaja aquí. Lo peor de todo es que siento... siento...";
		string = string	+ "\nSiento como si ya hubiera estado aquí antes, lo cual es imposible.";
		break;
	//Altar
	case 5:
		string = "Parece una especie de altar, aunque juraría que falta una pieza central.";
		break;
	//Energía
	case 6:
		string = "Realmente hermoso. Debe de ser la razón por la que me hicieron venir.";
		string = string	+ "\nFuentes de energía alternativa... no, esta tecnología parece demasiado avanzada.";
		break;
	case 7:
		string = "Otra vez esa sensación. Ese \"Déjà vu\", lo siento ahora con más intensidad.";
		string = string	+ "\nLas extrañas condiciones de este lugar... ¿Es posible que esto sea el causante?";
		string = string	+ "\nY, de ser así, ¿Quizá este artilugio emite algun tipo de radiación?";
		break;
	case 8:
		string = "Demonios, ¿Será peligroso? ¿Me han enviado a una misión suicida?";
		string = string	+ "\nPor si eso fuera poco, ¿Como pretendían que sacase esto de aquí?";
		break;
	default:
		break;
	}

	return string;
}


//****************************************************
//Control de los eventos de texto en el escenario 4
//****************************************************
void EventManager::controlText4()
{
	Ogre::String string = "";
	Ogre::Real posX = gPlayer->m_sceneObject->mNode->getPosition().x;

	//Final
	if(!text_1_14 && posX > 0 && posX < 100)
	{
		//Conversación final
		if(actualText == 0 || actualText == 1){
			if (actualText == 0)
				actualText++;
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			string = selectText4(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			gCCegui->dialogBox->getChildAtIdx(2)->setText("FUHRER 1.0");
			actualText++;
			gCanUpdateKeyboard = false;
			gPlayer->m_direction.x = 0;
		}
		else if ( actualText >= 2 && actualText <= 14 && nextText){
			string = selectText4(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			//------------------------------------
			//ROBOT: 1-2-6-7-8-10-12-14
			if(actualText == 2 || actualText == 6 || actualText == 7 || actualText == 8 || actualText == 10 || actualText == 12 || actualText == 14)
				gCCegui->dialogBox->getChildAtIdx(2)->setText("FUHRER 1.0");
			else
				gCCegui->dialogBox->getChildAtIdx(2)->setText("SUJETO 123");
			//------------------------------------
			actualText++;
			nextText = false;
			}
		else if(actualText == 15 && nextText){
			gCCegui->dialogBox->getChildAtIdx(2)->setText("SUJETO 123");
			gCCegui->dialogBox->hide();
			text_1_14 = true;
			nextText = false;
			gCanUpdateKeyboard = true;
			actualText = 0;
		}			
	}
}

//****************************************************
//Eventos de texto del escenario 4
//****************************************************
Ogre::String EventManager::selectText4(int textNumber)
{
	Ogre::String string = "";

	switch (textNumber)
	{
	//ROBOT
	case 1:
		string = "I-N-I-C-I-A-N-D-O...";
		break;
	case 2:
		string = "Bienvenido, Sujeto 123. Sus avances en la misión van a ser evaluados.";
		string = string	+ "\nPor favor, especifique lo que usted cree que ha sucedido en esta base.";
		break;
	//SUJETO 123
	case 3:
		if (text_8_9){
			string = "De acuerdo... al llegar a la base descubrí restos de sangre, indicios";
			string = string	+ "\nde que no soy la única persona que ha accedido a las instalaciones.";
		}
		else{
			string = "De acuerdo... al llegar a la base no detecté anomalías.";
			string = string	+ "\nDesde el último mes nadie ha accedido a las instalaciones.";
		}
		break;
	case 4:
		if (text_8_9){
			string = "Encontré una espada, únicamente otorgada a los voluntarios";
			string = string	+ "\ndel proyecto. Junto a la sangre, creo que, antes de llegar yo,";
			string = string	+ "\notros miembros de S&T intentaron recuperar la fuente de energía.";
		}
		else{
			string = "Encontré una espada, únicamente otorgada a los voluntarios";
			string = string	+ "\ndel proyecto. Es posible que antes del incidente alguien fuera enviado";
			string = string	+ "\na estas instalaciones para inspeccionar la fuente de energía.";
		}
		break;
	case 5:
		if (text_8_9 && magic){
			string = "También descubrí algo en el templo. Avisos ocultos dirigidos a mí.";
			string = string	+ "\nLa sangre, la espada y las notas... Era yo todo este tiempo, ¿Verdad?";
			string = string	+ "\nEsos \"Déjà vu\"... Todo era una farsa... soy un experimento en un bucle infinito.";
		}
		else if (magic){
			string = "También descubrí algo en el templo. Avisos ocultos dirigidos a mí.";
			string = string	+ "\nQuien las escribió intentó advertirme de que esto era una farsa.";
			string = string	+ "\nNunca hubo posibilidad de recuperar el artefacto... es una misión sin retorno.";
		}
		else{
			string = "Encontré la fuente de energía e intenté extraerla, pero he aparecido aquí.";
			string = string	+ "\nTengo la sensación de haber fallado en mi misión... solo deseo volver a mi hogar y";
			string = string	+ "\nolvidarme del proyecto S&T. No considero que este cualificado para este trabajo.";
		}
		break;
		//ROBOT
	case 6:
		string = "P-R-O-C-E-S-A-N-D-O...";
		break;
	case 7:
		if (text_8_9 && magic){
			string = "Sujeto 123, ha completado su misión correctamente. Usted ha sido enviado a";
			string = string	+ "\neste lugar en multiples ocasiones y, gracias a sus aportaciones, se avanzará en";
			string = string	+ "\nla investigación contra la perdida de memoria sufrida en los viajes temporales.";
		}
		else {
			string = "Sujeto 123:";
			string = string + "\nNo ha logrado completar la misión de acuerdo a los parametros establecidos.";
		}
		break;
	case 8:
		string = "S&T agradece su colaboración.Por favor, preparese. ";
		string = string	+ "\nEn breves será re-enviado al comienzo de la misión.";
		break;
	//SUJETO 123
	case 9:
		string = "¡Espera, como que re-enviado!";
		break;
	//ROBOT
	case 10:
		string = "Tres...";
		break;
	//SUJETO 123
	case 11:
		if (text_8_9 && magic){
			string = "¡He completado la misión, deben dejarme marchar!";
		}
		else{
			string = "¡Que le den a la misión, quiero abandonar el proyecto!";
		}
		break;
	//ROBOT
	case 12:
		string = "Dos...";
		break;
	//SUJETO 123
	case 13:
		string = "¡NOOOOOOOOO!";
		break;
	//ROBOT
	case 14:
		string = "Uno.";
		break;
	}
	
	return string;
}











