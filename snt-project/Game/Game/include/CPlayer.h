#ifndef __CPlayer_h_
#define __CPlayer_h_

#include "stdafx.h"

class KeyboardMouse;

//Clase para creacion de cubos
class CPlayer 
{
//Aqui se establecen las declaraciones de metodos y variables
public:

	//Constructor, destructor
    CPlayer(void);
    virtual ~CPlayer(void);

	//METODOS publicos
	void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	void setScale(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	void setRotation(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	bool keyboardControl(const Ogre::FrameEvent& evt);
	void updateAnimation();

	//VARIABLES PUBLICAS

	//Declaracion de la entidad y del nodo correspondiente al cubo
	Ogre::Entity* entity;
	Ogre::SceneNode* node;

	//Booleano de colision: (true) colisionable, (false) no colisionable
	bool m_collision;
	char state;

private:

	//VARIABLES privadas

	//Control del teclado para el jugador
	KeyboardMouse* mKeyboardMouse;

	//Velocidad de movimiento
	Ogre::Real m_Move;

	//Posicion
	Ogre::Real m_posX;
	Ogre::Real m_posY;
	Ogre::Real m_posZ;

	//Escalado
	Ogre::Real m_scaleX;
	Ogre::Real m_scaleY;
	Ogre::Real m_scaleZ;

	//Rotacion
	Ogre::Real m_yaw;
	Ogre::Real m_pitch;
	Ogre::Real m_roll;

	//Animaciones
	Ogre::AnimationState *pWalk;
	Ogre::AnimationState *pJump;

	//Estados	
	static const char WALK = 0x1;
	static const char JUMP = 0x2;

};

//---------------------------------------------------------------------------

#endif // #ifndef __CPlayer_h_

//---------------------------------------------------------------------------