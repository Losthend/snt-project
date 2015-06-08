#include "../include/stdafx.h"
#include "../include/Global.h"

//Jugador + Objetos y entorno
Player* gPlayer;
std::vector<Object*> gObjects;
std::vector<Ogre::Entity*> gEntitys;
std::vector<Ogre::SceneNode*> gNodes;

//Variables globales (Ogre)
Ogre::Root*				gRoot;
Ogre::SceneManager*		gSceneMgr;
Ogre::RenderWindow*		gWindow;
Ogre::Camera*			gCamera;
Ogre::Viewport*			gViewport;
OIS::InputManager*		gInputManager;
OIS::Mouse*				gMouse;
OIS::Keyboard*			gKeyboard;

bool					gShutDown;
bool					gCanUpdate;

PhysicsManager*			gPhysics;
CCegui*					gCCegui;
GameApplication*		gGameApp;

