class Object;
//Definicion de metodos de deteccion de colisiones

std::vector<Ogre::SceneNode*> inCameraFrustumObjects(void);

std::vector<Ogre::Vector3> getOccupiedCoords(Ogre::SceneNode* node);
std::vector<Ogre::Vector3> simulateOccupiedCoords(Ogre::SceneNode* node, Ogre::Vector3 vDistance);

Object* testCollisionAABB(std::vector<Ogre::Vector3> vSimulatedCoords, Ogre::String nodeName);

bool testCollisionWithSingleNode(std::vector<Ogre::Vector3> vSimulatedCoords, Ogre::SceneNode* node);

Ogre::Real getMinDistance(Ogre::SceneNode* node1, Ogre::SceneNode* node2);