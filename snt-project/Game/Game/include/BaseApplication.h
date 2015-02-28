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

	//Objeto Root, la base del proyecto
	Ogre::Root*				mRoot;

private:

	virtual bool setup();
	virtual bool configure(void);
    virtual void chooseSceneManager(void);
    virtual void createCamera(void);
    virtual void createFrameListener(void);
    virtual void destroyScene(void);
    virtual void createViewports(void);
    virtual void setupResources(void);
    virtual void loadResources(void);

    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

	//ELIMINAR el siguiente metodo cuando ya no sean necesario nuestros "cubos manuales"
	void simpleCube(void);

	//VARIABLES
	
	Ogre::SceneManager*		mSceneMgr;
	Ogre::RenderWindow*		mWindow;
	Ogre::Camera*			mCamera;
	Ogre::Viewport*			mViewport;
	OIS::InputManager*		mInputManager;
	OIS::Mouse*				mMouse;
	OIS::Keyboard*			mKeyboard;

	bool                    mShutDown;

	Ogre::String			mResourcesCfg;
	Ogre::String			mPluginsCfg;
	Ogre::String            m_ResourcePath;   

	//Overlay para Ogre 1.9
	Ogre::OverlaySystem*        mOverlaySystem;

	#ifdef OGRE_STATIC_LIB
		Ogre::StaticPluginLoader m_StaticPluginLoader;
	#endif
};

//---------------------------------------------------------------------------

#endif // #ifndef __BaseApplication_h_

//---------------------------------------------------------------------------