#include "stdafx.h"
#include "KeyboardMouse.h"

#include "Global.h"
#include "Collision.h"
#include "FuncionesGenerales.h"

#include "CCube.h"
#include "CPlayer.h"

KeyboardMouse::KeyboardMouse(void)
{
	saltar = false;
	mMoveX = 125;
	mMoveY = 125;
	mGravedad = -0.98;
}
    

KeyboardMouse::~KeyboardMouse(void)
{
}


bool KeyboardMouse::keyboardControl(Ogre::SceneNode* mNodePJ, std::vector<Ogre::SceneNode*> sceneNodes, const Ogre::FrameEvent& evt)
{

	if(gKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	//Creamos un vector tridimensional de ceros
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	
	//Movimiento del personaje	
	//Arriba
	if (gKeyboard->isKeyDown(OIS::KC_W)){
		if(!individualCollisionManager(sceneNodes, mNodePJ)){
			transVector.y += mMoveY;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			
		}
		else{
			transVector.y -= 2 * mMoveY;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

	// Abajo
	if (gKeyboard->isKeyDown(OIS::KC_S)){
		if(!individualCollisionManager(sceneNodes, mNodePJ)){
			transVector.y -= mMoveY;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			transVector.y += 2 * mMoveY;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

	// izquierda
	if (gKeyboard->isKeyDown(OIS::KC_A)){
		if(!individualCollisionManager(sceneNodes, mNodePJ)){
			transVector.x -= mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			//animación de andar
			gPlayer->state=0x1;
		}
		else{
			//animación de quieto
			gPlayer->state=0x0;
			transVector.x += 2 * mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

		}
	}
	
	//Esto parece arreglarlo siempre, pero no deberia ser siempre... o si?
	/*if (!gKeyboard->isKeyDown(OIS::KC_A)){
		gPlayer->state=0x0;
	}*/

	// Derecha
	if (gKeyboard->isKeyDown(OIS::KC_D)){
		if(!individualCollisionManager(sceneNodes, mNodePJ)){
			transVector.x += mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			//animación de andar
			gPlayer->state=0x1;
		}
		else{
			//animación de quieto
			gPlayer->state=0x0;
			transVector.x -= 2 * mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

	//espacio	
	if (gKeyboard->isKeyDown(OIS::KC_SPACE)){
		saltar = true;
	}

	if(saltar){
		v = mNodePJ->getPosition();
		//Tener en cuenta que al multiplicar por el evt siempre tambien modificamos los valores de 'x' y de 'z' si no valen exactamente 0
		//Segun mis cuentas el valor del evt es aproximadamente (1/100)
		if(!individualCollisionManager(sceneNodes, mNodePJ)){
			mMoveY = mMoveY + (mGravedad /* * evt.timeSinceLastFrame*/);
			mNodePJ->translate(Ogre::Vector3(v[0], mMoveY, v[2]) * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			imprimir("aqui se invierte arriba y abajo?");
			transVector.y += 2 * mMoveY;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			saltar = false;
		}
	}
	return true;
}

