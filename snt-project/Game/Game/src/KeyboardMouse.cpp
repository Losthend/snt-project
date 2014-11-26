#include "stdafx.h"
#include "KeyboardMouse.h"

void metodoDePrueba(OIS::Keyboard* mKeyboard, Ogre::SceneManager* mSceneMgr, const Ogre::FrameEvent& evt)
{
	static Ogre::Real mMove = 125; //Constante de movimiento

	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	//Movimiento del personaje	
	if (mKeyboard->isKeyDown(OIS::KC_W)) //Arriba
		transVector.y += mMove;

	if (mKeyboard->isKeyDown(OIS::KC_S)) // Abajo
		transVector.y -= mMove;

	if (mKeyboard->isKeyDown(OIS::KC_A)) // izquierda
		transVector.x -= mMove;

	if (mKeyboard->isKeyDown(OIS::KC_D)) // Derecha
		transVector.x += mMove;

	mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
}

