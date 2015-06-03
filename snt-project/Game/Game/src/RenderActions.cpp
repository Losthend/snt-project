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

	//Iniciamos las fisicas, dando acceso tambien a la creacion de escenarios
	gGameApp = new GameApplication();

	//Por defecto, no se permite realizar update de objetos
	gCanUpdate = false;
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

	//Si no se puede realizar update y el menu principal no se esta mostrando
	if( !gCanUpdate && !(gCCegui->gameMenu->d_root->isActive()) )
	{
		//Mostrar el menu principal
		gCCegui->gameMenu->d_root->show();
		gCCegui->gameMenu->d_root->activate();
		gCCegui->gameMenu->onEnteringSample();
	}

    //Realiza la captura de eventos del teclado y del raton
    gKeyboard->capture();
    gMouse->capture();

	//Es necesario inyectar "timestamps" al sistema de CEGUI
    CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	
	if(gCanUpdate && gPlayer != 0)
	{
		//Control de fisica y colisiones bullet: Objeto y jugador
		gPhysics->update(float(0.1));
		for(int i = 0, len = gObjects.size(); i < len; i++)
			gObjects[i]->update();
		gPlayer->update();
	}
	//Finalizacion del frame
	endFrame();

    return true;
}

