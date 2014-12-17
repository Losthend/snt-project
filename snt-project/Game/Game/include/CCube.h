#ifndef __CCube_h_
#define __CCube_h_

#include "stdafx.h"

//Clase para creacion de cubos
class CCube 
{
//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor, destructor
    CCube(std::string id);
    virtual ~CCube(void);

	//METODOS publicos
	Ogre::SceneNode* getAssociatedNode(void);
	
	//Velocidad de movimiento
	Ogre::Real m_moveX;
	Ogre::Real m_moveY;

	//Booleano de colision: (true) colisionable, (false) no colisionable
	bool m_collision;

private:
	//VARIABLES

	//Declaracion de la entidad y del nodo correspondiente al cubo
	Ogre::Entity* entity;
	Ogre::SceneNode* node;
};

//---------------------------------------------------------------------------

#endif // #ifndef __CCube_h_

//---------------------------------------------------------------------------