#ifndef __Object_h_
#define __Object_h_

#include "stdafx.h"

//Clase para creacion de objectos estaticos (paredes, suelo, etc)
class Object 
{
//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor, destructor
    Object(std::string id, int objType);
    virtual ~Object(void);
	
	//Tipo de objeto
	int m_objType; 

	//Velocidad de movimiento
	Ogre::Real m_moveX;
	Ogre::Real m_moveY;

	//Booleano de colision: (true) colisionable, (false) no colisionable
	bool m_collision;

	//Declaracion de la entidad y del nodo correspondiente al cubo
	Ogre::Entity* m_entity;
	Ogre::SceneNode* m_node;	
};

//---------------------------------------------------------------------------

#endif // #ifndef __Object_h_

//---------------------------------------------------------------------------