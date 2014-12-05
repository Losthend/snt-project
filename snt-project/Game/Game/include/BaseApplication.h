#ifndef __BaseApplication_h_
#define __BaseApplication_h_

#include "stdafx.h"
#include "Global.h"

#include "FixedFrameRate.h"
#include "KeyboardMouse.h"
#include "FuncionesGenerales.h"
#include "Collision.h"


//Se añade herencia para que puedan utilizarse posteriormente ciertos metodos
class BaseApplication : public Ogre::WindowEventListener, public Ogre::FrameListener
{
//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor, destructor y "go"
    BaseApplication(void);
    virtual ~BaseApplication(void);
    bool go(void);

	//VARIABLES
	//Objeto Root, la base del proyecto
	Ogre::Root*				mRoot;
	//El SceneManager 
	Ogre::SceneManager*		mSceneMgr;
	//La ventana de renderizado
	Ogre::RenderWindow*		mWindow;
	//La camara
	Ogre::Camera*			mCamera;
	Ogre::Viewport*			mViewport;
	//Dispositivos de entrada OIS
	OIS::InputManager*		mInputManager;
	OIS::Mouse*				mMouse;
	OIS::Keyboard*			mKeyboard;

private:

	//METODOS "principales"
	virtual void getResources(void);
	virtual void activateOIS(void);
	//Para los WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);
	//Metodo que se encarga del control de eventos
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	//Configuracion inicial del proyecto
	virtual void setUp(void);
	bool processUnbufferedInput(const Ogre::FrameEvent& evt);

	//VARIABLES
	
	//Origen de recursos y plugins, necesario debido al SDK
	Ogre::String			mResourcesCfg;
	Ogre::String			mPluginsCfg;
};

//---------------------------------------------------------------------------

#endif // #ifndef __BaseApplication_h_

//---------------------------------------------------------------------------