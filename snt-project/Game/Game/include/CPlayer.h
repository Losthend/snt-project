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