#ifndef __GameApplication_h_
#define __GameApplication_h_

//---------------------------------------------------------------------------
class PhysicsManager;

class GameApplication
{
public:
    GameApplication(void);
    virtual ~GameApplication(void);

    void createScene(void);
	void createGroundGrass(Ogre::Vector3 vSize, Ogre::Vector3 vPos, int amount);
	void createFont(Ogre::String name, Ogre::Vector2 size, Ogre::Vector3 pos, Ogre::String material, Ogre::Vector2 repeat);
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameApplication_h_

//---------------------------------------------------------------------------
