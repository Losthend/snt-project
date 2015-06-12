#ifndef __GameApplication_h_
#define __GameApplication_h_

class PhysicsManager;
class SceneObject;

class GameApplication
{
public:
    GameApplication(void);
    virtual ~GameApplication(void);
    void createScene1(void);
	void createScene2(void);
	void createScene3(void);

	int activeScene;
	SceneObject* specialObject1;
	SceneObject* specialObject2;

private:
	void clearScene(void);
	void createGroundGrass(Ogre::Vector3 vSize, Ogre::Vector3 vPos, int amount);
	void createFont(Ogre::String name, Ogre::Vector2 size, Ogre::Vector3 pos, Ogre::String material, Ogre::Vector2 repeat, bool unit);
	void createObject(Ogre::String name, Ogre::Vector3 size, Ogre::Vector3 pos, Ogre::String material, Ogre::String mesh);
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameApplication_h_

//---------------------------------------------------------------------------
