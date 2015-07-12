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
			//Eventos de texto
			controlText2();
			//Cambio de escenario
			if(text_6_7 && actualText == 0)
				gGameApp->createScene3();
		}
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
		//TO-DO
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

	//INTRODUCCI�N
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

	//Finalizaci�n de dialogos
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
//Introducci�n
	case 1:
		string = "�De qu� me han servido tantos a�os de lealtad y sacrificio? De nada.";
		string = string	+ "\nDeb� habermelo pensado dos veces antes de comprometerme con el proyecto.";
		actualText = 1;
		break;
	case 2:
		string = "Ya ni siquiera tengo nombre. Tan solo soy otro n�mero, otra rata de laboratorio bajo";
		string = string	+ "\nla atenta mirada de Adolf Hitler.";
		actualText = 2;
		break;
	case 3:
		string = "Y todo para qu�, �Para ganar la guerra? Admito que la idea de cambiar el pasado";
		string = string	+ "\nes brillante y tentadora, el arma definitiva... pero, �A que precio?";
		actualText = 3;
		break;
	case 4:
		string = "Da igual, poco puedo hacer ya. Ser� mejor que me centre en la misi�n.";
		string = string	+ "\nSi logro encontrar lo que me pidieron... quiz� me dejen abandonar todo esto.";
		actualText = 4;
		break;
	case 5:
		string = "Debo encontrar el prototipo de una fuente de energ�a alternativa en la que";
		string = string	+ "\nestaban trabajando las \"SS\" antes... antes de perder toda comunicaci�n con la";
		string = string	+ "\nbase. Antes del... incidente.";
		actualText = 5;
		break;
	case 6:
		string = "Decenas de personas desaparecidas de la noche a la ma�ana. Ning�n cuerpo,";
		string = string	+ "\nninguna petici�n de ayuda... dios... deb� habermelo pensado mejor...";
		actualText = 6;
		break;
	case 7:
		string = "Puedes acercar/alejar la c�mara utilizando el SCROLL del rat�n.";
		string = string	+ "\nPara moverte a la izquierda/derecha utiliza la tecla A/D, respectivamente.";
		string = string	+ "\nPulsa \"Q\" al ver el icono con dicha letra para interactuar con el entorno.";
		actualText = 7;
		break;
//Interacci�n con la sangre
	case 8:
		string = "�Esto es sangre? Parece reciente.";
		actualText = 8;
		break;
	case 9:
		string = "No tiene sentido, el �ltimo comunicado emitido por esta base se";
		string = string	+ "\nremonta a hace casi un mes... ser� mejor andarse con cuidado.";
		actualText = 9;
		break;
//Interacci�n con la puerta
	case 10:
		string = "Esta cerrada... debo encontrar otra forma de avanzar.";
		actualText = 10;
		break;
//Primera interacci�n con el derrumbamiento
	case 11:
		string = "Parece un derrumbamiento. Ser� mejor pasar con cuidado.";
		string = string	+ "\n(Para saltar pulsa la tecla SPACE)";
		actualText = 11;
		break;
//Segunda interacci�n con el derrumbamiento
	case 12:
		string = "Puede que haya algo que se me escape. Ser� mejor revisar este lugar.";
		actualText = 12;
		break;
	case 13:
		string = "Para mover objetos con telequinesis, realiza click derecho sobre el objeto y,";
		string = string	+ "\nsosteniendo la pulsaci�n, desplaza el rat�n hacia el lugar donde desees moverlo.";
		string = string	+ "\nRealiza click izquierdo, mientras sostienes la pulsaci�n, para lanzar el objeto.";
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
		}
	}

	//PUENTE Y ESPADA
	bool bridge = (!text_4_5 && posX > 1750 && posX < 1850);
	bool sword = (!text_6_7 && posX > 2400 && posX < 2500);
	if (bridge || sword){
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

	//Finalizaci�n de dialogos
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
		string = "�Pero que...!";
		break;
	case 2:
		string = "�C�mo demonios ha llegado un oso hasta aqu�? No se si podr� esquivarlo.";
		string = string	+ "\nSi me ve me atacar�. Puedo intentar matarlo o saltar por encima suyo y huir.";
		break;
	case 3:
		string = "Ataques: pulsa la tecla \"E\" para desenvainar/envainar tus espadas.";
		string = string	+ "\nCon las espadas fuera, realiza click izquierdo (rat�n) para realizar un ataque.";
		break;
	//Puente
	case 4:
		string = "Piedras voladoras... �Ingenier�a nazi o tecnolog�a extraterrestre?";
		string = string	+ "\nNo quiero saberlo, bastante tengo con los viajes en el tiempo.";
		break;
	case 5:
		string = "Creo que puedo utilizarlas a modo de puente, aunque no parecen muy";
		string = string	+ "\nestables... siento como si la gravedad de este lugar fuera distinta.";
		break;
	//Espada
	case 6:
		string = "Esta espada... es id�ntica a la m�a. Solo fueron otorgadas a aquellos que nos";
		string = string	+ "\npresentamos voluntarios para el proyecto.";
		break;
	case 7:
		string = "�Acaso no soy el �nico al que han enviado aqu�? Sea de quien sea, est�";
		string = string	+ "\nfuertemente clavada en la piedra... quiz� si tiro de ella...";
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
	if(!text_1_4)
	{
		if(actualText == 0 || actualText == 1){
			if (actualText == 0)
				actualText++;
			gCCegui->dialogBox->show();
			gCCegui->dialogBox->activate();
			string = selectText3(actualText);
			gCCegui->dialogBox->getChildAtIdx(3)->setText(string);
			actualText++;
			gCanUpdate = false;
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
			gCanUpdate = true;
		}
	}

	//Altar y energ�a


}


//****************************************************
//Eventos de texto del escenario 3
//****************************************************
Ogre::String EventManager::selectText3(int textNumber)
{
	Ogre::String string = "";
				//"1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31"
	switch (textNumber)
	{
	//Entrada al templo
	case 1:
		string = "�Pero que...!";
		break;
	case 2:
		string = "Un templo egipcio, vale, ahora todo tiene sentido... ��Sentido!?";
		string = string	+ "\nEnserio, �Que demonios est� pasando aqu�?�Que es este lugar?";
		break;
	case 3:
		string = "Dijeron que era un centro de investigaci�n sobre fuentes de energ�a alternativas.";
		string = string	+ "\nNadie dijo nada de piedras voladoras y teletransporte a templos misteriosos.";
		break;
	case 4:
		string = "No, algo no encaja aqu�. Lo peor de todo es que siento... siento...";
		string = string	+ "\nSiento como si ya hubiera estado aqu� antes, lo cual es imposible.";
		break;
	//Altar
	case 5:
		string = "Parece una especie de altar, aunque jurar�a que falta una pieza central.";
		break;
	//Energ�a
	case 6:
		string = "Realmente hermoso. Debe de ser la raz�n por la que me hicieron venir.";
		string = string	+ "\nFuentes de energ�a alternativa... no, esta tecnolog�a parece demasiado avanzada.";
		break;
	case 7:
		string = "Otra vez esa sensaci�n. Ese \"D�j� vu\", lo siento ahora con m�s intensidad.";
		string = string	+ "\nLas extra�as condiciones de este lugar... �Es posible que esto sea el causante?";
		string = string	+ "\nY, de ser as�, �Quiz� este artilugio emite algun tipo de radiaci�n?";
		break;
	case 8:
		string = "Demonios, �Ser� peligroso? �Me han enviado a una misi�n suicida?";
		string = string	+ "\nPor si eso fuera poco, �Como pretend�an que sacase esto de aqu�?";
		break;
	default:
		break;
	}

	return string;
}