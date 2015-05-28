#ifndef __SceneObject_h_
#define __SceneObject_h_

class SceneObject 
{

public:
    SceneObject(Ogre::Entity &entity, Ogre::SceneNode &node, btRigidBody &body);
    virtual ~SceneObject(void);
	void update();

public:
	Ogre::Entity &mEntity;
	Ogre::SceneNode &mNode;
	btRigidBody &mRigidBody;

};

//---------------------------------------------------------------------------

#endif // #ifndef __SceneObject_h_

//---------------------------------------------------------------------------