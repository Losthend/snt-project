#pragma once

//Declaracion de clases Jugador y Cubo
class Player;
class Object;

//Jugador y Objetos del entorno de acceso global
extern Player* gPlayer;
extern std::vector<Object*> vObjects;

//Declaramos las variables de configuracion para que puedan ser usadas como globales
extern Ogre::Root*				gRoot;
extern Ogre::SceneManager*		gSceneMgr;
extern Ogre::RenderWindow*		gWindow;
extern Ogre::Camera*			gCamera;
extern Ogre::Viewport*			gViewport;
extern OIS::InputManager*		gInputManager;
extern OIS::Mouse*				gMouse;
extern OIS::Keyboard*			gKeyboard;