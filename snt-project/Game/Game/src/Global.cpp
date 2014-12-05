#include "stdafx.h"
#include "Global.h"

//Variables globales

Ogre::Root*				gRoot;
Ogre::SceneManager*		gSceneMgr;
Ogre::RenderWindow*		gWindow;
Ogre::Camera*			gCamera;
Ogre::Viewport*			gViewport;
OIS::InputManager*		gInputManager;
OIS::Mouse*				gMouse;
OIS::Keyboard*			gKeyboard;

//Metodos "set" de las variables globales

void makeGlobalRoot(Ogre::Root* root)
{
	gRoot = root;
}

void makeGlobalSceneMgr(Ogre::SceneManager* sceneMgr)
{
	gSceneMgr = sceneMgr;
}

void makeGlobalRenderWindow(Ogre::RenderWindow* window)
{
	gWindow = window;
}

void makeGlobalCamera(Ogre::Camera* camera)
{
	gCamera = camera;
}

void makeGlobalViewport(Ogre::Viewport* viewport)
{
	gViewport = viewport;
}

void makeGlobalInputManager(OIS::InputManager* inputManager)
{
	gInputManager = inputManager;
}

void makeGlobalMouse(OIS::Mouse* mouse)
{
	gMouse = mouse;
}

void makeGlobalKeyboard(OIS::Keyboard* keyboard)
{
	gKeyboard = keyboard;
}
