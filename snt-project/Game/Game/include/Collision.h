class Object;
//Definicion de metodos de deteccion de colisiones

std::vector<Ogre::SceneNode*> inCameraFrustumObjects(void);

std::vector<Ogre::Vector3> getOccupiedCoords(Ogre::SceneNode* node);
std::vector<Ogre::Vector3> simulateOccupiedCoords(Ogre::SceneNode* node, Ogre::Vector3 vDistance);

Object* testCollisionAABB(std::vector<Ogre::Vector3> vSimulatedCoords);

//std::vector<Ogre::Vector3> getAllOccupiedCoords();
//bool testColision(std::vector<Ogre::Vector3> vAllCoords, std::vector<Ogre::Vector3> vPlayerCoords);