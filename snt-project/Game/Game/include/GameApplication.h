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
};

//---------------------------------------------------------------------------

#endif // #ifndef __GameApplication_h_

//---------------------------------------------------------------------------
