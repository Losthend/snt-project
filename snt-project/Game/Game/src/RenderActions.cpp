#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/RenderActions.h"
#include "../include/Global.h"
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/SceneObject.h"
#include "../include/FrameRate.h"
#include "../include/SdkTrayMgr.h"

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

	//Inicializacion del SdkTrays
	//sdkTrays = new SdkTrayMgr();

}

//------------------------------------------------------------------
//Destructor, tanto del objeto Root como del Listener asociado a la ventana
//------------------------------------------------------------------

RenderActions::~RenderActions(void)
{
	//Vaciado del vector global de SceneObject
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

    //Realiza la captura de eventos del teclado y del raton
    gKeyboard->capture();
    gMouse->capture();
	
	//---------------------------------------------------------------------
	//Control de fisica y colisiones entre objetos de bullet
	//---------------------------------------------------------------------

	gPhysics->update(0.06);
	for(int i = 0, len = gObjects.size(); i < len; i++)
		gObjects[i]->update();

	//---------------------------------------------------------------------

	gPlayer->update();

	//SdkTrays
	//sdkTrays->update(evt);

	//Finalizacion del frame
	endFrame();

    return true;
}

