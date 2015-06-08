#include "../include/stdafx.h"

#include "../include/RenderActions.h"
#include "../include/GameApplication.h"
#include "../include/EventManager.h"
#include "../include/FrameRate.h"
#include "../include/Global.h"


//------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------
RenderActions::RenderActions(void)
{
	//Registramos la aplicacion como un WindowEventListener
	Ogre::WindowEventUtilities::addWindowEventListener(gWindow, this);

	//Necesario para que la funcion frameRenderingQueued sea llamada 
	gRoot->addFrameListener(this);

	//Iniciamos las fisicas, dando acceso tambien a la creacion de escenarios
	gGameApp = new GameApplication();

	//Gestor de eventos
	eventMgr = new EventManager();
}

//------------------------------------------------------------------
//Destructor, tanto del objeto Root como del Listener asociado a la ventana
//------------------------------------------------------------------

RenderActions::~RenderActions(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(gWindow, this);
	windowClosed(gWindow);
	delete gRoot;
}

//------------------------------------------------------------------
//---En esta funcion se controla el bucle de renderizado y 
//---las acciones a realizar durante el mismo.
//------------------------------------------------------------------

bool RenderActions::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(gWindow->isClosed())
        return false;

	 if(gShutDown)
        return false;

    //Realiza la captura de eventos del teclado y del raton
    gKeyboard->capture();
    gMouse->capture();

	//Es necesario inyectar "timestamps" al sistema de CEGUI
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);

	//Tratar eventos 
	eventMgr->handleEvent();
	
	//Finalizacion del frame
	endFrame();

    return true;
}

