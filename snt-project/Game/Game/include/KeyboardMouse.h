#ifndef __KeyboardMouse_h_
#define __KeyboardMouse_h_

#include "stdafx.h"

class KeyboardMouse
{
	//Variables
	bool saltar;
	Ogre::Vector3 v;
	Ogre::Real mMoveX;
	Ogre::Real mMoveY;
	double mGravedad;

public:
	//constructor  y destructor
	KeyboardMouse(void);
    virtual ~KeyboardMouse(void);

	bool keyboardControl(Ogre::SceneNode* mNodePJ, std::vector<Ogre::SceneNode*> sceneNodes, const Ogre::FrameEvent& evt);

};

#endif // #ifndef __KeyboardMouse_h_