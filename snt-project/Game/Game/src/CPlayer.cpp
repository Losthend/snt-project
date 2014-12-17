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
	//Inicializamos variables
	
		//Posicion
		m_posX = 0.0;
		m_posY = 0.0;
		m_posZ = 0.0;
		//Rotacion
		m_pitch = 0.0;
		m_yaw = 0.0;
		m_roll = 0.0;
		//Escalar
		m_scaleX = 1.0;
		m_scaleY = 1.0;
		m_scaleZ = 1.0;
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
		//-----------Cubo creado de forma automatica---------------
		//Ogre::Entity* cube = mSceneMgr->createEntity(cubeName, "cube.mesh");
		//-----------Cubo creado de forma manual-------------------
		entity = gSceneMgr->createEntity(entName, "ninja.mesh");
		node = gSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
		node->attachObject(entity);
		
		//Asociamos las animaciones. Requier el skeleton en la carpeta de media.
		pWalk = entity->getAnimationState("Walk");
		pWalk->setLoop(true);		
		pWalk->setLength(Ogre::Real(1.0));

		//Posicionamiento y escalado (por defecto)
		node->setPosition(Ogre::Vector3(m_posX, m_posY, m_posZ));
		node->scale(Ogre::Vector3(m_scaleX, m_scaleY, m_scaleZ));
}
//---------------------------------------------------------------------------
CPlayer::~CPlayer(void)
{
	delete this;
}

//---------------------------------------------------------------------------
//--Posicionamiento
//---------------------------------------------------------------------------
void CPlayer::setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_posX = x;
	m_posY = y;
	m_posZ = z;
	node->setPosition(Ogre::Vector3(m_posX, m_posY, m_posZ));
}

//---------------------------------------------------------------------------
//--Escalado
//---------------------------------------------------------------------------
void CPlayer::setScale(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_scaleX = x;
	m_scaleY = y;
	m_scaleZ = z;
	node->scale(Ogre::Vector3(m_scaleX, m_scaleY, m_scaleZ));
}

//---------------------------------------------------------------------------
//--Rotacion
//---------------------------------------------------------------------------
void CPlayer::setRotation(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	m_pitch = x;
	m_yaw = y;
	m_roll = z;

	if (m_pitch != 0.0)
	{
		node->rotate(Ogre::Vector3::UNIT_X,Ogre::Degree(m_pitch));
	}
	if (m_yaw != 0.0)
	{
		node->rotate(Ogre::Vector3::UNIT_Y,Ogre::Degree(m_yaw));
	}
	if (m_roll != 0.0)
	{
		node->rotate(Ogre::Vector3::UNIT_Z,Ogre::Degree(m_roll));
	}
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

void CPlayer::updateAnimation(){

	if(state == WALK){
		pWalk->setEnabled(true);
		pWalk->addTime(0.0017);
	}

}