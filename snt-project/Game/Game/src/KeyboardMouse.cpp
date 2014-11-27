#include "stdafx.h"
#include "KeyboardMouse.h"
#include "FuncionesGenerales.h"

bool saltar = false;
Ogre::Vector3 v;

bool metodoDePrueba(OIS::Keyboard* mKeyboard, Ogre::SceneManager* mSceneMgr, const Ogre::FrameEvent& evt)
{

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	
	//Constante de movimiento
	static Ogre::Real mMoveX = 125;
	static Ogre::Real mMoveY = 500;
	float mGravedad = -2000;
	Ogre::Real mActualY = 0, mActualX = 0, mActualZ = 0;

	//Creamos un vector tridimensional de ceros
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	
	//Movimiento del personaje	
	//Arriba
	if (mKeyboard->isKeyDown(OIS::KC_W)){
		transVector.y += mMoveY;
		mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	// Abajo
	if (mKeyboard->isKeyDown(OIS::KC_S)){
		transVector.y -= mMoveY;
		mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	// izquierda
	if (mKeyboard->isKeyDown(OIS::KC_A)){
		transVector.x -= mMoveX;
		mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	// Derecha
	if (mKeyboard->isKeyDown(OIS::KC_D)){
		transVector.x += mMoveX;
		mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	//espacio
	
	if (mKeyboard->isKeyDown(OIS::KC_SPACE)){
		v = mSceneMgr->getSceneNode("cubeNode1")->getPosition();
		mActualX = v[0];
		mActualY = v[1];
		mActualZ = v[2];
		saltar = true;	
	}

	if(saltar){
		imprimir(1);
		if(v[1] >= -100){	
			v = mSceneMgr->getSceneNode("cubeNode1")->getPosition();
			mMoveY = mMoveY + (mGravedad * evt.timeSinceLastFrame);		
			mSceneMgr->getSceneNode("cubeNode1")->translate(Ogre::Vector3(mActualX, mMoveY*evt.timeSinceLastFrame, mActualZ) * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			//transVector.y += mMoveY * evt.timeSinceLastFrame;
		}
		else{
			mSceneMgr->getSceneNode("cubeNode1")->translate(Ogre::Vector3(mActualX, 0, mActualZ) * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			saltar = false;
			mMoveY = 500;
		}
	}

	//aplicar el movimiento al nodo
	//mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	return true;
}

