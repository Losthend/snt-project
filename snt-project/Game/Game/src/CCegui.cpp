#include "../include/stdafx.h"

#include "../include/CCegui.h"
#include "../include/Global.h"
#include "../include/Player.h"
#include "../include/GameMenu.h"
#include "../include/GameApplication.h"
#include "../include/EventManager.h"

CCegui::CCegui(void)
{
	//Se inicializa CEGUI
	mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

	//Se actualizan los resourceGroup (definidos en resources.cfg)
	CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
	CEGUI::Font::setDefaultResourceGroup("Fonts");
	CEGUI::Scheme::setDefaultResourceGroup("Schemes"); //Se encarga de la apariencia de los menus
	CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
	CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

	//Carga inicial de los menus
	initLoads();	
}

CCegui::~CCegui(void)
{
}

CEGUI::MouseButton CCegui::convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

//-----------------------------------------------------------------
//Carga inicial de los recursos necesarios para que funcionen los menus (loadWindows, ventanas)
//-----------------------------------------------------------------
void CCegui::initLoads()
{
	//Menu principal
    CEGUI::SchemeManager::getSingleton().createFromFile("GameMenu.scheme");
    CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");
	//CEGUI::FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("GameMenuImages/MouseCursor");
    CEGUI::Font& defaultFont = CEGUI::FontManager::getSingleton().createFromFile("Jura-13.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(&defaultFont);

	//DialogBox
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaCommonDialogs.scheme");
	CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
	
	//Cargar ventanas
	loadWindows();
}

//-----------------------------------------------------------------
//Se crean las ventanas, ocultas, para que esten disponibles posteriormente desde el root
//-----------------------------------------------------------------
void CCegui::loadWindows()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	//Creaci�n de "root window"
	CEGUI::Window *wRoot = wmgr.createWindow("DefaultWindow", "MainWindow");
	CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(wRoot);
	
	//Menu principal
	gameMenu = new GameMenu();

	//Dialog box
	gameDialogBox();

	//Pause box
	gameDeadBox();

	//End box
	gameEndBox();

	//Menu de aviso de interacci�n
	alertBox = wmgr.createWindow("Vanilla/StaticText", "MainWindow/AlertBox");
	alertBox->setSize(CEGUI::USize(CEGUI::UDim(0.03f, 0), CEGUI::UDim(0.05f, 0)));
	alertBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.5f, 0),CEGUI::UDim(0.45f, 0)));
	alertBox->setDisabled(true);
	alertBox->setText("Q");
	wRoot->addChild(alertBox);

	//Por defecto, todas las ventanas ocultas
	size_t numChild = wRoot->getChildCount();
	for (unsigned i = 0; i < numChild; i++)
		wRoot->getChildAtIdx(i)->hide();
}

//----------------------------------------------------------------
//"Dialog box" para los eventos de texto del juego
//----------------------------------------------------------------
void CCegui::gameDialogBox()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	dialogBox = wmgr.createWindow("Vanilla/FrameWindow", "MainWindow/DialogBox");
	dialogBox->setSize(CEGUI::USize(CEGUI::UDim(0.75f, 0), CEGUI::UDim(0.18f, 0)));
	dialogBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0),CEGUI::UDim(0.75f, 0)));
	dialogBox->setDisabled(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(dialogBox);
	
	CEGUI::Window* titlebar = wmgr.createWindow("Vanilla/Titlebar");
    titlebar->setSize(CEGUI::USize(CEGUI::UDim(1, 0.0), CEGUI::UDim(0.1f, 0)));
	titlebar->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0.10f, 0)));
	titlebar->setText("SUJETO 123");

	CEGUI::Window* textLabel = wmgr.createWindow("Vanilla/StaticText");
    textLabel->setSize(CEGUI::USize(CEGUI::UDim(1, 0.0), CEGUI::UDim(0.75f, 0)));
	textLabel->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, 0)));
	textLabel->setText("");

	CEGUI::Window* button1 = wmgr.createWindow("Vanilla/Button");
    button1->setSize(CEGUI::USize(CEGUI::UDim(0.3f, 0.0), CEGUI::UDim(0.2f, 0)));
	button1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.70f, 0),CEGUI::UDim(0.80f, 0)));
	button1->setText("Siguiente (ENTER)");
	//button1->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&CCegui::handleNext,this));
	
	//El orden en el que se a�aden importa! De variarlo, realizar las modificaciones correspondientes en EventManager.cpp
	//Posicion 0 (dialog box), posicion 1 (otro), titlebar, textlabel, button1 
	dialogBox->addChild(titlebar);
	dialogBox->addChild(textLabel);
	dialogBox->addChild(button1);
}

//----------------------------------------------------------------
//Menu de pausa (tras morir)
//---------------------------------------------------------------
void CCegui::gameDeadBox()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	deadBox = wmgr.createWindow("Vanilla/FrameWindow", "MainWindow/pauseBox");
	deadBox->setSize(CEGUI::USize(CEGUI::UDim(0.20f, 0), CEGUI::UDim(0.20f, 0)));
	deadBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.40f, 0),CEGUI::UDim(0.25f, 0)));
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(deadBox);

	//Boton de salir
	CEGUI::Window* button1 = wmgr.createWindow("Vanilla/Button");
    button1->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0.0), CEGUI::UDim(0.5f, 0)));
	button1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0),CEGUI::UDim(0.0f, 0)));
	button1->setText("Salir del juego");
	button1->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&CCegui::handleExit,this));

	//Boton de reiniciar pantalla
	CEGUI::Window* button2 = wmgr.createWindow("Vanilla/Button");
    button2->setSize(CEGUI::USize(CEGUI::UDim(1.0f, 0.0), CEGUI::UDim(0.5f, 0)));
	button2->setPosition(CEGUI::UVector2(CEGUI::UDim(0.0f, 0),CEGUI::UDim(0.5f, 0)));
	button2->setText("Reiniciar escenario");
	button2->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&CCegui::handleReload,this));
	
	//El orden en el que se a�aden importa! De variarlo, realizar las modificaciones correspondientes en EventManager.cpp
	//Posicion 0 (dialog box), posicion 1 (otro), posicion 2 (button1) 
	deadBox->addChild(button1);
	deadBox->addChild(button2);
}

//----------------------------------------------------------------
//Menu tras finalizar el juego
//---------------------------------------------------------------
void CCegui::gameEndBox()
{
	CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();

	endBox = wmgr.createWindow("Vanilla/FrameWindow", "MainWindow/pauseBox");
	endBox->setSize(CEGUI::USize(CEGUI::UDim(0.75f, 0), CEGUI::UDim(0.18f, 0)));
	endBox->setPosition(CEGUI::UVector2(CEGUI::UDim(0.15f, 0),CEGUI::UDim(0.75f, 0)));
	endBox->setDisabled(true);
	CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->addChild(endBox);
	
	CEGUI::Window* titlebar = wmgr.createWindow("Vanilla/Titlebar");
    titlebar->setSize(CEGUI::USize(CEGUI::UDim(1, 0.0), CEGUI::UDim(0.1f, 0)));
	titlebar->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0.10f, 0)));
	titlebar->setText("�GRACIAS POR JUGAR A S&T!");

	CEGUI::Window* textLabel = wmgr.createWindow("Vanilla/StaticText");
    textLabel->setSize(CEGUI::USize(CEGUI::UDim(1, 0.0), CEGUI::UDim(0.75f, 0)));
	textLabel->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0),CEGUI::UDim(0, 0)));
	Ogre::String string = "Proyecto final de carrera realizado por: Ra�l Fern�ndez Latorre";
	string = string	+ "\nGrado en ingenier�a inform�tica de gesti�n y sistemas de informaci�n.";
	string = string	+ "\nUniversidad del Pais Vasco (UPV/EHU)";
	textLabel->setText(string);

	CEGUI::Window* button1 = wmgr.createWindow("Vanilla/Button");
    button1->setSize(CEGUI::USize(CEGUI::UDim(0.3f, 0.0), CEGUI::UDim(0.2f, 0)));
	button1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.70f, 0),CEGUI::UDim(0.80f, 0)));
	button1->setText("Salir (ESCAPE)");
	button1->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&CCegui::handleExit,this));

	//El orden en el que se a�aden importa! De variarlo, realizar las modificaciones correspondientes en EventManager.cpp
	//Posicion 0 (dialog box), posicion 1 (otro), posicion 2 (button1) 
	endBox->addChild(titlebar);
	endBox->addChild(textLabel);
	endBox->addChild(button1);
}

bool CCegui::handleExit(const CEGUI::EventArgs &e)
{
	gShutDown = true;
    return true;
}

bool CCegui::handleReload(const CEGUI::EventArgs &e)
{
	//Recarga de escenario (solo puede morir en 2 y 3)
	if (gGameApp->activeScene == 2){
		gEventMgr->text_1_3 = false;
		gEventMgr->text_4_5 = false;
		gEventMgr->text_6_7 = false;
		gEventMgr->actualText = 0;
		gGameApp->createScene2();
	}
	else if (gGameApp->activeScene == 3){
		gEventMgr->inExitCount = 0;
		gEventMgr->inExit = false;
		gEventMgr->text_1_4 = false;
		gEventMgr->text_5 = false;
		gEventMgr->text_6_7_8 = false;
		gEventMgr->magic = false;
		gEventMgr->actualText = 0;
		gGameApp->createScene3();
	}
	//Recargar vida
	gPlayer->m_lifeCounter = 100;
	//Desactivar deadbox
	deadBox->hide();

    return true;
}

//CEGUI::OgreRenderer::bootstrapSystem().setDisplaySize(CEGUI::Sizef(width, height));