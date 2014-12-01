#include "stdafx.h"
#include "BaseApplication.h"
#include "FixedFrameRate.h"
#include "KeyboardMouse.h"
#include "FuncionesGenerales.h"
#include "Collision.h"

//------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------

BaseApplication::BaseApplication(void)
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK)
{
}

//------------------------------------------------------------------
//Destructor, tanto del objeto Root como el Listener asociado a la ventana
//------------------------------------------------------------------

BaseApplication::~BaseApplication(void)
{
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

//------------------------------------------------------------------
//Inicializa variables y llama a las funciones necesarias para crear el entorno
//------------------------------------------------------------------

bool BaseApplication::go(void)
{
    #ifdef _DEBUG
		mResourcesCfg = "resources_d.cfg";
		mPluginsCfg = "plugins_d.cfg";
	#else
		mResourcesCfg = "resources.cfg";
		mPluginsCfg = "plugins.cfg";
	#endif

	//Se crea el Root
    mRoot = new Ogre::Root(mPluginsCfg);
	
	//Le decimos a Ogre donde estan los recursos a utilizar
	getResources();

	//Establecer manualmente el sistema de renderizado (tambien Direct3D9, por ejemplo)
	Ogre::RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	//Configuracion del proyecto de Ogre
	setUp();

	return true;
	}

//------------------------------------------------------------------
//Realiza la configuracion inicial de la aplicacion
//Contiene: creacion de ventana, inicializacion de recursos, 
//			creacion SceneManager, camara y OIS
//------------------------------------------------------------------

void BaseApplication::setUp(void)
{
	//Creacion de la ventana de renderizado, inicializacion del sistema de renderizado
	mWindow = mRoot->initialise(true, "Render Window");

	//Inicializacion de los recursos
	//Establecemos el nivel de mipmap
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	//Inicializamos todos los grupos de recursos
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//Creacion del SceneManager
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

	//Creacion de la camara, posicion, direccion, clipDistance
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setPosition(Ogre::Vector3(0,0,300));
	mCamera->lookAt(Ogre::Vector3(0,0,0));
	mCamera->setNearClipDistance(5);

	//A�adimos un viewport
	mViewport = mWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));

	//Ajustamos el aspect ratio al viewport
	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) /
		Ogre::Real(mViewport->getActualHeight()));
	
	//Activando los OIS
	activateOIS();

	//Registramos la aplicacion como un WindowEventListener
	windowResized(mWindow);
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	//Instanciamos mRoot como un FrameListener, pudiendo recibir fragmentos de eventos
	//Esto es necesario para que la funcion frameRenderingQueued sea llamada 
	mRoot->addFrameListener(this);
}

//------------------------------------------------------------------
//-----Para que Ogre conozca la localizacion de los recursos--------
//------------------------------------------------------------------

void BaseApplication::getResources(void)
{
	//Objeto de config, cargando las rutas del config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	//Le decimos a Ogre de donde obtener los recursos
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
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				archName, typeName, secName);
		}
	}
}

//------------------------------------------------------------------
//Genera lo necesario para la captura de eventos (teclado y raton)--
//------------------------------------------------------------------

void BaseApplication::activateOIS(void)
{
	//Configuracion incial de los OIS
	Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
	OIS::ParamList pl;
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;
 
	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
 
	mInputManager = OIS::InputManager::createInputSystem( pl );

	//Creacion de los objetos a partir de los cuales conoceremos los eventos
	mKeyboard = static_cast<OIS::Keyboard*>(mInputManager
		->createInputObject( OIS::OISKeyboard, false ));
	mMouse = static_cast<OIS::Mouse*>(mInputManager
		->createInputObject( OIS::OISMouse, false ));
}

//------------------------------------------------------------------
//Se le llamara cuando se redimensione la pantalla, sincroniza OIS--
//------------------------------------------------------------------

void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);
 
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}
 
//------------------------------------------------------------------
//---Las lineas comentadas de ESTE METODO DAN FALLO, ni idea porqu�-
//Libera los OIS en el momento que el objeto (ventana) es destruido-
//------------------------------------------------------------------

void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    if(rw == mWindow)
    {
        if(mInputManager)
        {
            mInputManager->destroyInputObject( mMouse );
            mInputManager->destroyInputObject( mKeyboard );
            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}

//------------------------------------------------------------------
//---En esta funcion se controla el bucle de renderizado y 
//---las acciones a realizar durante el mismo.
//------------------------------------------------------------------

bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	if(mWindow->isClosed())
        return false;

    //Realiza la captura de eventos del teclado y del raton
    mKeyboard->capture();
    mMouse->capture();
 
	//Llamamos a la funcion que se encarga de controlar las entradas 
    if(!processUnbufferedInput(evt)) return false;

    return true;
}

//------------------------------------------------------------------
//---Define que hacer cuando se produce algun evento entre frames---
//------------------------------------------------------------------

bool BaseApplication::processUnbufferedInput(const Ogre::FrameEvent& evt)
{
	//startFrame();
	
	//Obtenemos el personaje
	Ogre::SceneNode* mNodePj = mSceneMgr->getSceneNode("cubeNode1");

	bool saltar = false;
	Ogre::SceneNode* mNodePJ= mSceneMgr->getSceneNode("cubeNode1");
	if(!metodoDePrueba(mKeyboard, mNodePJ, evt))return false;
	
	//Objetos dentro del frustum rotan al pulsar F2
	if(mKeyboard->isKeyDown(OIS::KC_F2)) 
	{		
		std::vector<Ogre::SceneNode*> sceneNodes = inCameraFrustumObjects(mCamera, mSceneMgr);
		individualCollisionManager(sceneNodes, mNodePj);
	}

	//endFrame();
	//duration();

	return true;
}