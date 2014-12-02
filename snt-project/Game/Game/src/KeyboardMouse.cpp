#include "stdafx.h"
#include "KeyboardMouse.h"
#include "FuncionesGenerales.h"
#include "Collision.h"


bool saltar = false;
Ogre::Vector3 v;
static Ogre::Real mMoveX = 125;
double mMoveY = 125;
double mGravedad = -.98;

bool metodoDePrueba(OIS::Keyboard* mKeyboard, Ogre::SceneNode* mNodePJ, const Ogre::FrameEvent& evt, std::vector<Ogre::SceneNode*> sceneNodes)
{

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	//Creamos un vector tridimensional de ceros
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	
	//Movimiento del personaje	
	//Arriba
	if (mKeyboard->isKeyDown(OIS::KC_W)){
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
	if (mKeyboard->isKeyDown(OIS::KC_S)){
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
	if (mKeyboard->isKeyDown(OIS::KC_A)){
		if(!individualCollisionManager(sceneNodes, mNodePJ)){
			transVector.x -= mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			transVector.x += 2 * mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

	// Derecha
	if (mKeyboard->isKeyDown(OIS::KC_D)){
		if(!individualCollisionManager(sceneNodes, mNodePJ)){
			transVector.x += mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			transVector.x -= 2 * mMoveX;
			mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

	//espacio	
	if (mKeyboard->isKeyDown(OIS::KC_SPACE)){
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

