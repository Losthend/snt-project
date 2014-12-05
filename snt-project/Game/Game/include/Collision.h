#include "stdafx.h"
#include "Global.h"

#include <OgreSceneQuery.h>

//Definicion de metodos de deteccion de colisiones

std::vector<Ogre::SceneNode*> inCameraFrustumObjects(void);
void allCollisionManager(std::vector<Ogre::SceneNode*> sceneNodes);
bool individualCollisionManager(std::vector<Ogre::SceneNode*> sceneNodes, Ogre::SceneNode* node1);
bool AABBcollisionDetection(Ogre::SceneNode* node1, Ogre::SceneNode* node2);
