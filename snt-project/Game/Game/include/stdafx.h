#pragma once

//LIBRERIAS DE SISTEMA

#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include <time.h>
#include "windows.h"

//LIBRERIAS DE OPENGL

#include <GL/gl.h>
#include <GL/glu.h>

//LIBRERIAS DE BULLET
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"

//LIBRERIAS DE OGRE

//La libreria para Root
#include <OgreRoot.h>
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
//Querys de frustum
#include <OgreSceneQuery.h>

//CEGUI
#include "CEGUI\CEGUI.h"
#include "CEGUI\RendererModules\Ogre\Renderer.h"

//Librerias de inputs para Windows o Apple
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#  include <OIS/OISEvents.h>
#  include <OIS/OISInputManager.h>
#  include <OIS/OISKeyboard.h>
#  include <OIS/OISMouse.h>
#else
#  include <OISEvents.h>
#  include <OISInputManager.h>
#  include <OISKeyboard.h>
#  include <OISMouse.h>
#endif
 
//Otros headers que pueden incluirse

#include <OgreLogManager.h>

#ifdef OGRE_STATIC_LIB
#  define OGRE_STATIC_GL
#  if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    define OGRE_STATIC_Direct3D9
// D3D10 will only work on vista, so be careful about statically linking
#    if OGRE_USE_D3D10
#      define OGRE_STATIC_Direct3D10
#    endif
#  endif
#  define OGRE_STATIC_BSPSceneManager
#  define OGRE_STATIC_ParticleFX
#  define OGRE_STATIC_CgProgramManager
#  ifdef OGRE_USE_PCZ
#    define OGRE_STATIC_PCZSceneManager
#    define OGRE_STATIC_OctreeZone
#  else
#    define OGRE_STATIC_OctreeSceneManager
#  endif
#  include "OgreStaticPluginLoader.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <macUtils.h>
#endif