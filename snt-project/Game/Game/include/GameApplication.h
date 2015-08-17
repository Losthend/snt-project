#ifndef __GameApplication_h_
#define __GameApplication_h_

class PhysicsManager;
class Object;

class GameApplication
{
public:
    GameApplication(void);
    virtual ~GameApplication(void);
    void createScene1(void);
	void createScene2(void);
	void createScene3(void);
	void createScene4(void);

	int activeScene;
	Object* specialObject0;
	Object* specialObject1;
	
	void createFont(Ogre::String name, Ogre::Vector2 size, Ogre::Vector3 pos, Ogre::String material, Ogre::Vector2 repeat, bool unit);

private:
	void clearScene(void);
	void createGroundGrass(Ogre::Vector3 vSize, Ogre::Vector3 vPos, int amount);
	void createObject(Ogre::String name, Ogre::Vector3 size, Ogre::Vector3 pos, Ogre::String material, Ogre::String mesh);
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameApplication_h_

//---------------------------------------------------------------------------
