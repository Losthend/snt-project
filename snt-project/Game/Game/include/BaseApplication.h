#ifndef __BaseApplication_h_
#define __BaseApplication_h_

class BaseApplication
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
	//Configuracion inicial del proyecto
	virtual void setUp(void);

	//ELIMINAR el siguiente metodo cuando ya no sean necesario nuestros "cubos manuales"
	void simpleCube(void);

	//VARIABLES
	
	//Origen de recursos y plugins, necesario debido al SDK
	Ogre::String			mResourcesCfg;
	Ogre::String			mPluginsCfg;
};

//---------------------------------------------------------------------------

#endif // #ifndef __BaseApplication_h_

//---------------------------------------------------------------------------