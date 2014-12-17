#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "CPlayer.h"
//Acceso a las variables globales
#include "Global.h"
//Acceso a Collision y a KeyboardMouse
#include "Collision.h"
#include "KeyboardMouse.h"

//---------------------------------------------------------------------------
CPlayer::CPlayer(void)
{
	//Colisionable por defecto
	m_collision = true;
	//Velocidad por defecto
	m_Move = 125;
	//Control de teclado y raton para el personaje
	mKeyboardMouse = new KeyboardMouse();
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
//--Control, mediante teclado, del jugador
//---------------------------------------------------------------------------
bool CPlayer::keyboardControl(const Ogre::FrameEvent& evt)
{
	//Obtenemos los objetos visibles del frustum
	std::vector<Ogre::SceneNode*> sceneNodes = inCameraFrustumObjects();

	//Ejecutamos la funcion de control de teclado 
	//Siendo "node" el propio nodo del jugador almacenado en esta clase
	//Se hace un "return" del resultado (bool)
	return mKeyboardMouse->keyboardControl(node, sceneNodes, evt); 
}

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