#pragma once

//Declaracion de clases Jugador y Cubo
class Player;
class Object;
class SceneObject;
class PhysicsManager;
class CCegui;
class GameApplication;

//Jugador + Objetos y entorno
extern Player* gPlayer;
extern std::vector<Object*> gObjects;
extern std::vector<Ogre::Entity*> gEntitys;
extern std::vector<Ogre::SceneNode*> gNodes;

//Declaramos las variables de configuracion para que puedan ser usadas como globales
extern Ogre::Root*				gRoot;
extern Ogre::SceneManager*		gSceneMgr;
extern Ogre::RenderWindow*		gWindow;
extern Ogre::Camera*			gCamera;
extern Ogre::Viewport*			gViewport;
extern OIS::InputManager*		gInputManager;
extern OIS::Mouse*				gMouse;
extern OIS::Keyboard*			gKeyboard;

extern bool						gShutDown;
extern bool						gCanUpdate;

extern PhysicsManager*			gPhysics;
extern CCegui*					gCCegui;
extern GameApplication*			gGameApp;