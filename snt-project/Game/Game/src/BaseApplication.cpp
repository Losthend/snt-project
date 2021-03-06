#include "../include/stdafx.h"
#include "../include/BaseApplication.h"

//Para instanciar las variables de entorno globales
#include "../include/Global.h"

//------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------

BaseApplication::BaseApplication(void)
    : mRoot(0),
    mCamera(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
	#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
		m_ResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
	#else
		m_ResourcePath = "";
	#endif
}

//------------------------------------------------------------------
//Destructor
//------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	destroyScene();
}

//------------------------------------------------------------------
//Inicializa variables y llama a las funciones necesarias para crear el entorno
//------------------------------------------------------------------

bool BaseApplication::go(void)
{
   //Primero se define el origen de los recursos
	#ifdef _DEBUG
	#ifndef OGRE_STATIC_LIB
		mResourcesCfg = m_ResourcePath + "resources_d.cfg";
		mPluginsCfg = m_ResourcePath + "plugins_d.cfg";
	#else
		mResourcesCfg = "resources_d.cfg";
		mPluginsCfg = "plugins_d.cfg";
	#endif
	#else
	#ifndef OGRE_STATIC_LIB
		mResourcesCfg = m_ResourcePath + "resources.cfg";
		mPluginsCfg = m_ResourcePath + "plugins.cfg";
	#else
		mResourcesCfg = "resources.cfg";
		mPluginsCfg = "plugins.cfg";
	#endif
	#endif

	//Se realiza la configuracion, en caso de cancelacion del proceso, detiene la ejecucion
    if (!setup())
        return false;

	//-----------------------------------------------------------------------------------
	//Generar los Manual Objects
	grass();
	//-----------------------------------------------------------------------------------

	//Hacer que las variables (configuradas) del entorno sean de acceso global
	gRoot = mRoot;
	gSceneMgr = mSceneMgr;
	gWindow = mWindow;
	gCamera = mCamera;
	gViewport = mViewport;
	gInputManager = mInputManager;
	gMouse = mMouse;
	gKeyboard = mKeyboard;
	gShutDown = false;
	//-----------------------------------------------------------------------------------

	return true;
	}


//---------------------------------------------------------------------------
//Realiza llamadas a metodos para configurar todo lo necesario en la aplicacion
//---------------------------------------------------------------------------
bool BaseApplication::setup(void)
{

	//Se crea el Root
    mRoot = new Ogre::Root(mPluginsCfg);

	//Configuracion de los recursos
    setupResources();

	//Configuracion de la ventana inicial de seleccion de valores del sistema
    bool carryOn = configure();
	//En caso de cancelarse la seleccion, detiene la ejecucion del programa
    if (!carryOn) return false;

	//Creacion del sceneManager, la camara y el viewport
    chooseSceneManager();
    createCamera();
    createViewports();

    //Configuracion del mipmap level por defecto
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    //Carga de los recursos
    loadResources();

	//Crea los frameListeners
    createFrameListener();

    return true;
};

//---------------------------------------------------------------------------
//Configura los recursos necesarios para que funcione la aplicacion
//---------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Cargar las rutas de los recursos desde el archivo de configuracion
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    //Realizar todas las configuraciones descritas en el fichero
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

		#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            // OS X does not set the working directory relative to the app.
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location.
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative directories
                archName = Ogre::String(Ogre::macBundlePath() + "/" + archName);
		#endif

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}

//---------------------------------------------------------------------------
//Configuracion de la ventana inicial de seleccion de valores del sistema
//---------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    // Muestra la configuracion e inicializa el sistema
    // Se puede evitar usando root.restoreConfig() para cargar la configuracion anterior almacenada en ogre.cfg.
    if(mRoot->showConfigDialog())
    {
        //Si devuelve true, el usuario a realizado click en OK, por lo que inicializa la ventana
        mWindow = mRoot->initialise(true, "S&T");
        return true;
    }
    else
    {
        return false;
    }

	//Establecer manualmente el sistema de renderizado
	//Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	//mRoot->setRenderSystem(rs);
	//rs->setConfigOption("Full Screen", "No");
	//rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");
}

//---------------------------------------------------------------------------
//Crea el sceneManager
//---------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    //Crea un sceneManager generico
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC); 
}

//---------------------------------------------------------------------------
//Crea la camara con su configuracion inicial
//---------------------------------------------------------------------------
void BaseApplication::createCamera(void)
{
    //Crea la camara
    mCamera = mSceneMgr->createCamera("PlayerCam");

    //Posicion
    mCamera->setPosition(Ogre::Vector3(0,75,450));
    //Direccion y clipDistance
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5); 
}

//---------------------------------------------------------------------------
//Creacion del viewport
//---------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
    // Create one viewport, entire window
    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
}


//---------------------------------------------------------------------------
//Creacion de los Listeners para los recursos
//---------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

	//Con esto el raton se muestra en pantalla, con el cursor del sistema, sin estar "atrapado" por la ventana del juego.
	
	#if defined OIS_WIN32_PLATFORM
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
		pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
	#endif
	
    mInputManager = OIS::InputManager::createInputSystem(pl);

	//Controladores de teclado y raton
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));

    //Configura el area de recorte del raton
    windowResized(mWindow);
}

//---------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

//---------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}

//---------------------------------------------------------------------------
//Ajusta el area de recorte del raton
//---------------------------------------------------------------------------
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

//---------------------------------------------------------------------------
// Desactiva OIS despues de cerrar la ventana (muy importante en Linux)
//---------------------------------------------------------------------------
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    //Solo cierra la ventana que crea el OIS
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject(mMouse);
            mInputManager->destroyInputObject(mKeyboard);

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------
//--Genera un ManualObject denominado "manualObjectGrass" para generar hierba 3D
//---------------------------------------------------------------------
void BaseApplication::grass()
{
	const float width = 25;
	const float height = 30;
	Ogre::Vector3 vec(width/2, 0, 0);
	Ogre::ManualObject obj("manualObjectGrass");
 
	Ogre::Quaternion quat;
	quat.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);

	obj.begin("Examples/GrassBlades", Ogre::RenderOperation::OT_TRIANGLE_LIST);
 
	for (int i = 0; i < 3; ++i)
	{
		obj.position(-vec.x, height, -vec.z);
		obj.textureCoord(0, 0);
		obj.position(vec.x, height, vec.z);
		obj.textureCoord(1, 0);
		obj.position(-vec.x, 0, -vec.z);
		obj.textureCoord(0, 1);
		obj.position(vec.x, 0, vec.z);
		obj.textureCoord(1, 1);
		int offset = 4 * i;
		obj.triangle(offset + 0, offset + 3, offset + 1);
		obj.triangle(offset + 0, offset + 2, offset + 3);
		vec = quat * vec;
	}
	obj.end();
	obj.convertToMesh("manualObjectGrass");
}