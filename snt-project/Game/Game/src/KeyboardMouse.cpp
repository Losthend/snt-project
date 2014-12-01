#include "stdafx.h"
#include "KeyboardMouse.h"
#include "FuncionesGenerales.h"

bool saltar = false;
Ogre::Vector3 v;
static Ogre::Real mMoveX = 125;
static Ogre::Real mMoveY = 125;
float mGravedad = -.98;

bool metodoDePrueba(OIS::Keyboard* mKeyboard, Ogre::SceneNode* mNodePJ, const Ogre::FrameEvent& evt)
{

	if(mKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	
	//Constante de movimiento
	
	

	//Creamos un vector tridimensional de ceros
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	
	//Movimiento del personaje	
	//Arriba
	if (mKeyboard->isKeyDown(OIS::KC_W)){
		transVector.y += mMoveY;
		mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	// Abajo
	if (mKeyboard->isKeyDown(OIS::KC_S)){
		transVector.y -= mMoveY;
		mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	// izquierda
	if (mKeyboard->isKeyDown(OIS::KC_A)){
		transVector.x -= mMoveX;
		mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	// Derecha
	if (mKeyboard->isKeyDown(OIS::KC_D)){
		transVector.x += mMoveX;
		mNodePJ->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
	}

	//espacio	
	if (mKeyboard->isKeyDown(OIS::KC_SPACE)){
		saltar = true;
	}

	if(saltar){
		v = mNodePJ->getPosition();
		//imprimir(v[0]);
		//esto está en -120 para poner los límites a mano. En realidad habrá que comprobar si colisiona o no...
		//Tener en cuenta que al multiplicar por el evt siempre tambien modificamos los valores de 'x' y de 'z' si no valen exactamente 0
		//Segun mis cuentas el valor del evt es aproximadamente (1/100)
		if(v[1]>-120){
			mMoveY = mMoveY + (mGravedad /* * evt.timeSinceLastFrame*/);
			mNodePJ->translate(Ogre::Vector3(v[0], mMoveY, v[2]) * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			saltar = false;
			mMoveY = 125;
			mNodePJ->translate(Ogre::Vector3(v[0], -120, v[2]) * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

	//aplicar el movimiento al nodo
	//mSceneMgr->getSceneNode("cubeNode1")->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

	return true;
}

