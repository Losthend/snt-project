#include "../include/stdafx.h"
#include "../include/Global.h"

//Jugador (ver constructor de GameApplication.cpp)
Player* gPlayer;
//Vector con los demas objetos (ver createScene de GameApplication.cpp) 
std::vector<Object*> gObjects;
std::vector<btCollisionShape*> gCollisionShapes;

//Variables globales (ver BaseApplication.cpp)
Ogre::Root*				gRoot;
Ogre::SceneManager*		gSceneMgr;
Ogre::RenderWindow*		gWindow;
Ogre::Camera*			gCamera;
Ogre::Viewport*			gViewport;
OIS::InputManager*		gInputManager;
OIS::Mouse*				gMouse;
OIS::Keyboard*			gKeyboard;

bool					gShutDown;

PhysicsManager*			gPhysics;
CCegui*					gCCegui;

