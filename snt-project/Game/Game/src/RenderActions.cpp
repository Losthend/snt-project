#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "RenderActions.h"
//Acceso a variables globales
#include "Global.h"
//Acceso a Object y Player
#include "Object.h"
#include "Player.h"
//Para el control de FPS
#include "FrameRate.h"

#include "FuncionesGenerales.h"

//------------------------------------------------------------------
//Esta clase esta orientada a establecer los listeners, permitiendo 
//controlar las acciones a realizar durante el renderizado.
//------------------------------------------------------------------

//Constructor, registra los Listener de la ventana y del objeto root para el renderizado
RenderActions::RenderActions(void)
{
	//Registramos la aplicacion como un WindowEventListener
	Ogre::WindowEventUtilities::addWindowEventListener(gWindow, this);
	//Añadimos esta clase al FrameListener
	//Esto es necesario para que la funcion frameRenderingQueued sea llamada 
	gRoot->addFrameListener(this);
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

    //Realiza la captura de eventos del teclado y del raton
    gKeyboard->capture();
    gMouse->capture();

	//gPlayer->updateAnimation();
	
	//Llamamos al metodo de control del teclado para el jugador
	if(!gPlayer->keyboardControl())return false;	

	//Finalizacion del frame
	endFrame();

    return true;
}