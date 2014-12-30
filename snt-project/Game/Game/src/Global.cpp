#include "stdafx.h"
#include "Global.h"

//Jugador (ver constructor de GameApplication.cpp)
Player* gPlayer;
//Vector con los demas objetos (ver createScene de GameApplication.cpp) 
std::vector<Object*> vObjects;

//Variables globales (ver BaseApplication.cpp)
Ogre::Root*				gRoot;
Ogre::SceneManager*		gSceneMgr;
Ogre::RenderWindow*		gWindow;
Ogre::Camera*			gCamera;
Ogre::Viewport*			gViewport;
OIS::InputManager*		gInputManager;
OIS::Mouse*				gMouse;
OIS::Keyboard*			gKeyboard;

