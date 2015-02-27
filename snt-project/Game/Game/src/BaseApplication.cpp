#include "../include/stdafx.h"
#include "../include/BaseApplication.h"

//Para instanciar las variables de entorno globales
#include "../include/Global.h"

//------------------------------------------------------------------
//Constructor
//------------------------------------------------------------------

BaseApplication::BaseApplication(void)
	: mRoot(0),
	mResourcesCfg(Ogre::StringUtil::BLANK),
	mPluginsCfg(Ogre::StringUtil::BLANK)
{
	//Configuramos el entorno
	go();
}

//------------------------------------------------------------------
//Destructor, tanto del objeto Root como el Listener asociado a la ventana
//------------------------------------------------------------------

BaseApplication::~BaseApplication(void)
{
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

	//-----------------------------------------------------------------------------------
	//Generar los Manual Objects (ELIMINAR CUANDO NO SEA NECESARIO)
	simpleCube();
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
	//-----------------------------------------------------------------------------------

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
	mCamera->setPosition(Ogre::Vector3(0,0,450));
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
	
	//Redimensionamos la ventana
	windowResized(mWindow);

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


//----------------ELIMINAR CUANDO NO SEA NECESARIO---------------------
//---------------------------------------------------------------------
//--Genera un ManualObject denominado "ManualObjectCube" que puede
//--ser utilizado como "mesh" al crear una nueva entidad.
//---------------------------------------------------------------------
void BaseApplication::simpleCube(void)
{
	//Se declara el objeto
	Ogre::ManualObject* lManualObject = mSceneMgr->createManualObject("ManualObjectCube");	
	//Se le asigna el material/textura que se desea utilizar, declarado en:
	//C:\Ogre\OgreSDK_vc9_v1-9-0\media\materials\scripts\MaterialDePrueba.material
	lManualObject->begin("MyMaterial1", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	//La creación del objeto, dando vertices, coordenadas para la textura, etc.
	float lSize = 10.0f;
	float cp = 1.0f * lSize ;
	float cm = -1.0f * lSize;

	lManualObject->position(cm, cp, cm);
	//lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,0.0f,1.0f));
	lManualObject->textureCoord(0.0, 1.0);

	lManualObject->position(cp, cp, cm);
	//lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,0.0f,1.0f));
	lManualObject->textureCoord(1.0, 1.0);

	lManualObject->position(cp, cm, cm);
	//lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,0.0f,1.0f));
	lManualObject->textureCoord(1.0, 0.0);

	lManualObject->position(cm, cm, cm);
	//lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,0.0f,1.0f));
	lManualObject->textureCoord(0.0, 0.0);

	//-----------------------------

	lManualObject->position(cm, cp, cp);
	//lManualObject->colour(Ogre::ColourValue(0.0f,1.0f,1.0f,1.0f));
	lManualObject->textureCoord(0.0, 1.0);

	lManualObject->position(cp, cp, cp);
	//lManualObject->colour(Ogre::ColourValue(1.0f,1.0f,1.0f,1.0f));
	lManualObject->textureCoord(1.0, 1.0);

	lManualObject->position(cp, cm, cp);
	//lManualObject->colour(Ogre::ColourValue(1.0f,0.0f,1.0f,1.0f));
	lManualObject->textureCoord(1.0, 0.0);

	lManualObject->position(cm, cm, cp);
	//lManualObject->colour(Ogre::ColourValue(0.0f,0.0f,1.0f,1.0f));
	lManualObject->textureCoord(0.0, 0.0);

	//-----------------------------

	// face behind / front
	lManualObject->triangle(0,1,2);
	lManualObject->triangle(2,3,0);
	lManualObject->triangle(4,6,5);
	lManualObject->triangle(6,4,7);

	// face top / down
	lManualObject->triangle(0,4,5);
	lManualObject->triangle(5,1,0);
	lManualObject->triangle(2,6,7);
	lManualObject->triangle(7,3,2);

	// face left / right
	lManualObject->triangle(0,7,4);
	lManualObject->triangle(7,0,3);
	lManualObject->triangle(1,5,6);
	lManualObject->triangle(6,2,1);		

	//-----------------------------

	lManualObject->end();
	lManualObject->convertToMesh("ManualObjectCube");
}