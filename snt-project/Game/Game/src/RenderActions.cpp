#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/RenderActions.h"
//Acceso a variables globales
#include "../include/Global.h"
//Acceso a Object, Player y SceneObject
#include "../include/Object.h"
#include "../include/Player.h"
#include "../include/SceneObject.h"
//Para el control de FPS
#include "../include/FrameRate.h"
//Para mostrar mensajes por pantalla
#include "../include/FuncionesGenerales.h"
//Para el uso de fisicas
#include "../include/PhysicsManager.h"

//------------------------------------------------------------------
//Esta clase esta orientada a establecer los listeners, permitiendo 
//controlar las acciones a realizar durante el renderizado.
//Incluye control de teclado y raton
//------------------------------------------------------------------

//Constructor, registra Listeners y crea paneles interactivos
RenderActions::RenderActions(void)
{
	//Registramos la aplicacion como un WindowEventListener
	Ogre::WindowEventUtilities::addWindowEventListener(gWindow, this);

	//Necesario para que la funcion frameRenderingQueued sea llamada 
	gRoot->addFrameListener(this);

	//Creacion del frame listener para teclado/raton
    gMouse->setEventCallback(this);
    gKeyboard->setEventCallback(this);

	/*
	//-------------------------------------------------------------------------
	//PROVISIONAL: Creacion de paneles (menus) - ¿Optimizacion con clase propia?
	//-------------------------------------------------------------------------
	mInputContext.mKeyboard = gKeyboard;
    mInputContext.mMouse = gMouse;

    mTrayMgr = new OgreBites::SdkTrayManager("InterfaceName", gWindow, mInputContext, this);
    mTrayMgr->showFrameStats(OgreBites::TL_TOPLEFT);
    mTrayMgr->hideCursor();

    // Create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mDetailsPanel = mTrayMgr->createParamsPanel(OgreBites::TL_NONE, "DetailsPanel", 200, items);
    mDetailsPanel->setParamValue(9, "Bilinear");
    mDetailsPanel->setParamValue(10, "Solid");
    mDetailsPanel->hide();
	//-------------------------------------------------------------------------
	*/
}

//------------------------------------------------------------------
//Destructor, tanto del objeto Root como del Listener asociado a la ventana
//------------------------------------------------------------------

RenderActions::~RenderActions(void)
{
	//if (mTrayMgr) delete mTrayMgr;

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

	//gPlayer->updateAnimation();
	
	//---------------------------------------------------------------------
	//Control de fisica y colisiones entre objetos de bullet
	//---------------------------------------------------------------------

	gPhysics->update(0.1);
	for(int i = 0, len = gObjects.size(); i < len; i++)
		gObjects[i]->update();

	//---------------------------------------------------------------------

	/*
	//Ejecutamos las actualizaciones de gravedad de los objetos de tipo 2 o 3
	for (unsigned x=0; x < vObjects.size(); x++)
	{
		Object* obj = vObjects[x];	
		if (obj->m_objType == 2)
		{			
			obj->update(Ogre::Vector3::ZERO);
		}
		if (obj->m_objType == 3)
		{			
			obj->move();
		}
	}
	*/

	//Llamamos al metodo de control del teclado para el jugador
	//if(!gPlayer->keyboardControl())return false;	

	/*
	//-------------------------------------------------------------------------
	//PROVISIONAL: Visualizacion de datos en el menu
	//-------------------------------------------------------------------------
	mTrayMgr->frameRenderingQueued(evt);
    if (!mTrayMgr->isDialogVisible())
    {
        if (mDetailsPanel->isVisible())          // If details panel is visible, then update its contents
        {
            mDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(gCamera->getDerivedPosition().x));
            mDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(gCamera->getDerivedPosition().y));
            mDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(gCamera->getDerivedPosition().z));
            mDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().w));
            mDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().x));
            mDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().y));
            mDetailsPanel->setParamValue(7, Ogre::StringConverter::toString(gCamera->getDerivedOrientation().z));
        }
    }
	//-------------------------------------------------------------------------
	*/

	//Finalizacion del frame
	endFrame();

    return true;
}

//--------------------------------------------------------------------------
//PROVISIONAL: Controladores de teclado y raton
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
bool RenderActions::keyPressed( const OIS::KeyEvent &arg )
{
	if (arg.key == OIS::KC_ESCAPE)
    {
		gShutDown = true;
    }

    return true;
}
//---------------------------------------------------------------------------
bool RenderActions::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}
//---------------------------------------------------------------------------
bool RenderActions::mouseMoved(const OIS::MouseEvent &arg)
{
    return true;
}
//---------------------------------------------------------------------------
bool RenderActions::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
    return true;
}
//---------------------------------------------------------------------------
bool RenderActions::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	return true;
}