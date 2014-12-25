#include "stdafx.h"

//Definicion de metodos de deteccion de colisiones

std::vector<Ogre::SceneNode*> inCameraFrustumObjects(void);
std::vector<Ogre::Vector3> getAllOccupiedCoords();
std::vector<Ogre::Vector3> getOccupiedCoords(Ogre::SceneNode* node);
std::vector<Ogre::Vector3> simulateOccupiedCoords(Ogre::SceneNode* node, Ogre::Vector3 vDistance);
bool testColision(std::vector<Ogre::Vector3> vAllCoords, std::vector<Ogre::Vector3> vPlayerCoords);

/*
bool AABBcollisionDetection(Ogre::SceneNode* node1, Ogre::SceneNode* node2);
bool collisionManager(void);
*/