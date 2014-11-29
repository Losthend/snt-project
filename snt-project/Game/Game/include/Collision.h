#include "stdafx.h"

//Definicion de metodos de deteccion de colisiones

std::vector<Ogre::SceneNode*> inCameraFrustumObjects(Ogre::Camera* mCamera, Ogre::SceneManager* mSceneMgr);
void collisionManager(std::vector<Ogre::SceneNode*> sceneNodes);
bool AABBcollisionDetection(Ogre::SceneNode* node1, Ogre::SceneNode* node2);
