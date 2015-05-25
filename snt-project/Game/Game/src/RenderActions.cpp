#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/RenderActions.h"
#include "../include/Global.h"
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/SceneObject.h"
#include "../include/FrameRate.h"
#include "../include/CCegui.h"
#include "../include/GameMenu.h"
#include "../include/GameApplication.h"

#include "../include/PhysicsManager.h"

//------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------
RenderActions::RenderActions(void)
{
	//Registramos la aplicacion como un WindowEventListener
	Ogre::WindowEventUtilities::addWindowEventListener(gWindow, this);

	//Necesario para que la funcion frameRenderingQueued sea llamada 
	gRoot->addFrameListener(this);

	//Por defecto la app no se ha activado
	isAppInit = false;
}

//------------------------------------------------------------------
//Destructor, tanto del objeto Root como del Listener asociado a la ventana
//------------------------------------------------------------------

RenderActions::~RenderActions(void)
{
	//Vaciado del vector global de objects
	for(int i = 0, len = gObjects.size(); i < len; ++i)
	{
		delete gObjects[i];
	}

	//Finalizar listeners y delete root
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

	//Crearemos el juego en el momento que el boton "start" del menu sea pulsado
	if (!isAppInit && gCCegui->gameMenu->d_startButtonClicked)
	{
		//Creamos el juego (personaje y escenario)
		GameApplication* gameApp = new GameApplication();
		//Cerramos (eliminamos) el menu
		CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->removeChild(gCCegui->gameMenu->d_root);
		//Cargamos los menus basicos
		CEGUI::Window* menu1 = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow()->getChild("QuitButton1");
		menu1->show();
		menu1->activate();		
		//La aplicacion (juego) se ha iniciado
		isAppInit = true;
	}

    //Realiza la captura de eventos del teclado y del raton
    gKeyboard->capture();
    gMouse->capture();

	//Es necesario inyectar "timestamps" al sistema de CEGUI
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	
	if(isAppInit && gPlayer != 0)
	{
		//Control de fisica y colisiones bullet: Objeto y jugador
		gPhysics->update(0.07);
		for(int i = 0, len = gObjects.size(); i < len; i++)
			gObjects[i]->m_sceneObject->update();
		gPlayer->update();
	}
	//Finalizacion del frame
	endFrame();

    return true;
}

