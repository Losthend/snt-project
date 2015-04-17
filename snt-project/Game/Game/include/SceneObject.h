#ifndef __SceneObject_h_
#define __SceneObject_h_

class SceneObject 
{

public:
    SceneObject(Ogre::SceneNode &node, btRigidBody &body);
    virtual ~SceneObject(void);
	void update();

public:
	Ogre::SceneNode &mNode;
	btRigidBody &mRigidBody;

};

//---------------------------------------------------------------------------

#endif // #ifndef __SceneObject_h_

//---------------------------------------------------------------------------