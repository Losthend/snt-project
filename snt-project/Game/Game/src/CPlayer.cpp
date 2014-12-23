#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "CPlayer.h"
//Acceso a las variables globales
#include "Global.h"
//Acceso a Collision
#include "Collision.h"
//Para escribir en el log
#include "FuncionesGenerales.h"

//---------------------------------------------------------------------------
CPlayer::CPlayer(void)
{
	//Keyboard - Variables
	correr = false;
	saltar = false;
	mMoveX = 125;
	mMoveY = 125;
	mGravedad = -0.098;

	//Colisionable por defecto
	m_collision = true;
	//Velocidad por defecto
	m_moveX = 125;
	m_moveY = 125;

	//estado pordefecto del personaje (0=quieto)
	state = 0x0;

	//Creamos el cubo con su entidad y nodo correspondiente
	//Identificadores
	std::string entName = "entityPlayer";
	std::string nodeName = "nodePlayer";
	
	//Creacion (entidad y nodo)
	//-----------Carga de la textura del robot---------------
	//entity = gSceneMgr->createEntity(entName, "robot.mesh");
	//-----------Cubo creado de forma manual-------------------
	entity = gSceneMgr->createEntity(entName, "ManualObjectCube");
	node = gSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
	node->attachObject(entity);
	
	//Posicionamiento y escalado (por defecto)
	node->setPosition(0.0, 0.0, 0.0);
	node->scale(1.0, 1.0, 1.0);

	node->showBoundingBox(true);

	/*
	//Asociamos las animaciones. Requier el skeleton en la carpeta de media.
	pWalk = entity->getAnimationState("Walk");
	pWalk->setLoop(true);		
	pWalk->setLength(Ogre::Real(1.0));
	*/
	/*
	//El robot no sabe saltar, asi que esto tiene que estar comentado o usar el ninja
	pJump = entity->getAnimationState("Jump");
	pJump->setLoop(true);		
	pJump->setLength(Ogre::Real(1.0));
	*/
}
//---------------------------------------------------------------------------
CPlayer::~CPlayer(void)
{
	delete this;
}

//---------------------------------------------------------------------------
//--Animacion de movimiento del personaje
//---------------------------------------------------------------------------
/*
void CPlayer::updateAnimation(){

	if(state == WALK){
		pWalk->setEnabled(true);
		pWalk->addTime(0.0017);
	}
	//if(state == JUMP){
	//	pJump->setEnabled(true);
	//	pJump->addTime(0.0017);
	//}
}
*/

//---------------------------------------------------------------------------
//--Control, mediante teclado, del jugador
//---------------------------------------------------------------------------
bool CPlayer::keyboardControl(const Ogre::FrameEvent& evt)
{
	//Obtenemos los objetos visibles del frustum
	std::vector<Ogre::SceneNode*> sceneNodes = inCameraFrustumObjects();

	if(gKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	//Creamos un vector tridimensional de ceros
	Ogre::Vector3 transVector0 = Ogre::Vector3::ZERO;
	Ogre::Vector3 v1 = Ogre::Vector3::ZERO;
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;

	if(gKeyboard->isKeyDown(OIS::KC_LSHIFT)) correr = true;
	else correr = false;

	if(gKeyboard->isKeyDown(OIS::KC_A)){	
		if(correr) transVector.x -= m_moveX;
		transVector.x -= m_moveX;		
		node->translate(transVector * evt.timeSinceLastFrame);
		if(individualCollisionManager(sceneNodes, node))
		{
			node->setPosition(transVector0);
		}
	}
	if(gKeyboard->isKeyDown(OIS::KC_D)){
		if(correr) transVector.x += m_moveX;
		transVector.x += m_moveX;
		node->translate(transVector * evt.timeSinceLastFrame);
		if(individualCollisionManager(sceneNodes, node))
		{
			node->setPosition(transVector0);
		}
	}
	if(gKeyboard->isKeyDown(OIS::KC_SPACE)){		
		saltar = true;
	}

	if(saltar){
		gPlayer->state = 0x2;
		v = node->getPosition();
		//Tener en cuenta que al multiplicar por el evt siempre tambien modificamos los valores de 'x' y de 'z' si no valen exactamente 0
		//Segun mis cuentas el valor del evt es aproximadamente (1/100)
		if(!individualCollisionManager(sceneNodes, node)){
			mMoveY = mMoveY + (mGravedad  * evt.timeSinceLastFrame);
			node->translate(Ogre::Vector3(v[0], mMoveY, v[2]) * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			imprimir("aqui se invierte arriba y abajo?");
			transVector.y += 2 * mMoveY;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			saltar = false;
		}
	}

	/*if(saltar){
		v1 = node->getPosition();
		m_jump = m_jump + mGravedad * evt.timeSinceLastFrame;		
		node->translate(v1[0], m_jump * evt.timeSinceLastFrame, 0.0);
		if(individualCollisionManager(sceneNodes, node))
		{
			saltar = false;
			node->setPosition(transVector0);
		}

	}*/
	
	/*
	// izquierda
	if (gKeyboard->isKeyDown(OIS::KC_A)){
		transVector.x -= mMoveX;
		node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		if(!individualCollisionManager(sceneNodes, node)){
			
			//animación de andar
			gPlayer->state=0x1;
		}
		else{
			//animación de quieto
			gPlayer->state=0x0;
			transVector.x += 2 * mMoveX;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

		}
	}
	
	//Esto parece arreglarlo siempre, pero no deberia ser siempre... o si?
	if (!gKeyboard->isKeyDown(OIS::KC_A)){
		gPlayer->state=0x0;
	}

	// Derecha
	if (gKeyboard->isKeyDown(OIS::KC_D)){
		if(!individualCollisionManager(sceneNodes, node)){
			transVector.x += mMoveX;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			//animación de andar
			gPlayer->state=0x1;
		}
		else{
			//animación de quieto
			gPlayer->state = 0x0;
			transVector.x -= 2 * mMoveX;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}*/
	return true;
}