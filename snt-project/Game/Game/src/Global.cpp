#include "stdafx.h"
#include "Global.h"

//Instancia del jugador
CPlayer* gPlayer;
//Vector con instancias de los CCubes declarados
std::vector<CCube*> cCubes;

//Variables globales
Ogre::Root*				gRoot;
Ogre::SceneManager*		gSceneMgr;
Ogre::RenderWindow*		gWindow;
Ogre::Camera*			gCamera;
Ogre::Viewport*			gViewport;
OIS::InputManager*		gInputManager;
OIS::Mouse*				gMouse;
OIS::Keyboard*			gKeyboard;

