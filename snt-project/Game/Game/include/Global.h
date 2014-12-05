#pragma once
#include "stdafx.h"

//Declaramos las variables para que puedan ser usadas como globales

extern Ogre::Root*				gRoot;
extern Ogre::SceneManager*		gSceneMgr;
extern Ogre::RenderWindow*		gWindow;
extern Ogre::Camera*			gCamera;
extern Ogre::Viewport*			gViewport;
extern OIS::InputManager*		gInputManager;
extern OIS::Mouse*				gMouse;
extern OIS::Keyboard*			gKeyboard;

//Declaramos las funciones a las que pasaremos el objeto para hacerlo global

void makeGlobalRoot(Ogre::Root* root);
void makeGlobalSceneMgr(Ogre::SceneManager* sceneMgr);
void makeGlobalRenderWindow(Ogre::RenderWindow* window);
void makeGlobalCamera(Ogre::Camera* camera);
void makeGlobalViewport(Ogre::Viewport* viewport);
void makeGlobalInputManager(OIS::InputManager* inputManager);
void makeGlobalMouse(OIS::Mouse* mouse);
void makeGlobalKeyboard(OIS::Keyboard* keyboard);