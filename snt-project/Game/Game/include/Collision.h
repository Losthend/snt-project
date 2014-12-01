#include "stdafx.h"

//Definicion de metodos de deteccion de colisiones

std::vector<Ogre::SceneNode*> inCameraFrustumObjects(Ogre::Camera* mCamera, Ogre::SceneManager* mSceneMgr);
void allCollisionManager(std::vector<Ogre::SceneNode*> sceneNodes);
void individualCollisionManager(std::vector<Ogre::SceneNode*> sceneNodes, Ogre::SceneNode* node1);
bool AABBcollisionDetection(Ogre::SceneNode* node1, Ogre::SceneNode* node2);
