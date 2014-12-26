#ifndef __CPlayer_h_
#define __CPlayer_h_

#include "stdafx.h"

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
	bool m_collisionGravity;
	char state;

	//Velocidad de movimiento
	Ogre::Real m_moveX;
	Ogre::Real m_moveY;

	//Aceleracion
	Ogre::Real accX;
	Ogre::Real accY;

	//Otras variables
	bool m_jumpUp;
	double m_gravity;
	Ogre::Vector3 m_direction;


private:

	//METODOS
	void keyPressed(void);
	void jumpSolution(void);

	//VARIABLES privadas

	//Animaciones
	Ogre::AnimationState *pWalk;
	Ogre::AnimationState *pJump;

	//Estados	
	static const char STOP = 0x0;
	static const char WALK = 0x1;
	static const char JUMP = 0x2;
	static const char PUSH = 0x3;

};

//---------------------------------------------------------------------------

#endif // #ifndef __CPlayer_h_

//---------------------------------------------------------------------------