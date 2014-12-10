#pragma once
#include "stdafx.h"

//Declaracion de clases Jugador y Cubo
class CPlayer;
class CCube;

//Jugador y Objetos del entorno de acceso global
extern CPlayer* gPlayer;
extern std::vector<CCube*> cCubes;

//Declaramos las variables de configuracion para que puedan ser usadas como globales
extern Ogre::Root*				gRoot;
extern Ogre::SceneManager*		gSceneMgr;
extern Ogre::RenderWindow*		gWindow;
extern Ogre::Camera*			gCamera;
extern Ogre::Viewport*			gViewport;
extern OIS::InputManager*		gInputManager;
extern OIS::Mouse*				gMouse;
extern OIS::Keyboard*			gKeyboard;