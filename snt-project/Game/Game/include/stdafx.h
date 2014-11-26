#pragma once

//LIBRERIAS DE SISTEMA

#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include "windows.h"

//LIBRERIAS DE OPENGL

#include <GL/gl.h>
#include <GL/glu.h>

//LIBRERIAS DE OGRE

//La libreria para Root
#include <OgreRoot.h>
//Librerias para eventos de teclado y raton
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
//Utilidades de la ventana de Ogre
#include <OgreWindowEventUtilities.h>
//Libreria para crear objetos propios (cuadrados, circulos, etc)
#include <OgreManualObject.h>
#include <OgreException.h>
//Utilidades para la configuracion de Ogre
#include <OgreConfigFile.h>
//Librerias para la creacion de camara, viewport, SceneManager y renderWindow
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
//Libreria para entidades
#include <OgreEntity.h>
 
//Otros headers que pueden incluirse
//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//#define WIN32_LEAN_AND_MEAN
//#include "windows.h"
//#endif
