#include "stdafx.h"
#include "GameApplication.h"

#include "CCube.h"
#include "ManualObjects.h"

//---------------------------------------------------------------------------
//Constructor del juego (Game)
//---------------------------------------------------------------------------
GameApplication::GameApplication(void)
{
	try {
		//Iniciamos las variables de configuracion del proyecto
        app.go();
		//Tras esto, tendremos todas las variables necesarias preparadas para su uso
    } 		
	catch( Ogre::Exception& e ) {
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
					MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
					std::cerr << "An exception has occured: " <<
						e.getFullDescription().c_str() << std::endl;
		#endif
    }
}
//---------------------------------------------------------------------------
GameApplication::~GameApplication(void)
{
}

//---------------------------------------------------------------------------
//Establece aquello que se muestra en la escena y sus caracteristicas, incluida la iluminacion
//---------------------------------------------------------------------------

void GameApplication::createScene(void)
{
	Ogre::SceneManager* mSceneMgr = app.mSceneMgr;

	/*Ogre::Entity* mEntidadPrubas = mSceneMgr->createEntity("escenrioPruebas","bugSPARK.mesh");
	Ogre::SceneNode* nodoEscenario = mSceneMgr->getRootSceneNode()->createChildSceneNode("nodoEscenrioPruebas");
	nodoEscenario->attachObject(mEntidadPrubas);
	nodoEscenario->yaw(Ogre::Radian(150));
	nodoEscenario->setPosition(Ogre::Vector3(0.0, -80.0, 0.0));
	nodoEscenario->scale(Ogre::Vector3(10, 10, 10));*/
	
	//Generar los Manual Objects
	//Llegado el momento, pasar a BaseApplication
	simpleCube(mSceneMgr);

	//Personaje
	CCube* cube1 = new CCube();
	cube1->createCube(mSceneMgr, "1");
	//Ogre::SceneNode* nodeCube1 = cube1->getAssociatedNode();
	//nodeCube1->attachObject(app.mCamera);

	//Suelo
	CCube* cube2 = new CCube();
	cube2->createCube(mSceneMgr, "2");
	cube2->setScale(14, 1.0, 1.0);
	cube2->setPosition(0.0, -125.0, 0.0);

	//Pared izquierda
	CCube* cube3 = new CCube();
	cube3->createCube(mSceneMgr, "3");
	cube3->setScale(1.0, 11.0, 1.0);
	cube3->setPosition(-150.0, 0.0, 0.0);

	//Pared derecha
	CCube* cube4 = new CCube();
	cube4->createCube(mSceneMgr, "4");
	cube4->setScale(1.0, 11.0, 1.0);
	cube4->setPosition(150.0, 0.0, 0.0);

	//Techo
	CCube* cube5 = new CCube();
	cube5->createCube(mSceneMgr, "5");
	cube5->setScale(14.0, 1.0, 1.0);
	cube5->setPosition(0.0, 125.0, 0.0);

	//Luz ambiente y fondo
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
 
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setPosition(20,80,50);

	//Iniciamos el bucle de renderizado infinito
	app.mRoot->startRendering();
}