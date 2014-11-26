#include "stdafx.h"
#include "BaseApplication.h"
#include "FixedFrameRate.h"

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

	//Añadimos un viewport
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
//---Las lineas comentadas de ESTE METODO DAN FALLO, ni idea porqué-
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
	static Ogre::Real mMove = 125; //Constante de movimiento

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;

	//startFrame();

	//Creamos un vector tridimensional de ceros
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;

	//Movimiento del personaje
	if (mKeyboard->isKeyDown(OIS::KC_W)) //Arriba
		transVector.y += mMove;

	if (mKeyboard->isKeyDown(OIS::KC_S)) // Abajo
		transVector.y -= mMove;

	if (mKeyboard->isKeyDown(OIS::KC_A)) // izquierda
		transVector.x -= mMove;

	if (mKeyboard->isKeyDown(OIS::KC_D)) // Derecha
		transVector.x += mMove;

	mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	
	//endFrame();
	//duration();

	return true;
}




//TUTORIALES DE LA PAGINA DE OGRE EN ESPAÑOL
//http://ogrees.wikispaces.com/Tutoriales+Basicos
//PROYECCION ORTOGONAL (frustum, con heigth y width)
//http://ogre3d.org/forums/viewtopic.php?f=2&t=55105
//Como mostrar un fondo 2D en el mundo
//http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Displaying+2D+Backgrounds
//Carga de recursos
//http://ogrees.wikispaces.com/Carga+Manual+de+Recursos
//Metodos de colisiones
//http://siondream.com/blog/games/colisiones-iv-tests-de-colision/
//http://siondream.com/blog/games/colisiones-i-introduccion-requisitos-y-alternativas/
//Este enlace puede resultar bastante util
//http://www.genbetadev.com/programacion-de-videojuegos/teoria-de-colisiones-2d-conceptos-basicos
//http://www.genbetadev.com/programacion-de-videojuegos/teoria-de-colisiones-2d-quadtree
//CODIGO para colision en esferas
//http://devmag.org.za/2009/04/13/basic-collision-detection-in-2d-part-1/
//http://www.bukisa.com/articles/218690_making-a-3d-game-with-ogre-collision-detection
//Usar AABB para Ogre?
//http://yosoygames.com.ar/wp/2013/07/good-bye-axisalignedbox-hello-aabb/
//ALGORITMOS BASICOS de colisiones explicados en C++
//http://black-byte.com/tutorial/algoritmos-basicos-de-colisiones-para-videojuegos/