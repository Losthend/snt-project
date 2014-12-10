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
	void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	void setScale(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	void setRotation(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	Ogre::SceneNode* getAssociatedNode(void);

private:
	//VARIABLES

	//Declaracion de la entidad y del nodo correspondiente al cubo
	Ogre::Entity* entity;
	Ogre::SceneNode* node;

	//Booleano de colision: (true) colisionable, (false) no colisionable
	bool m_collision;

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

};

//---------------------------------------------------------------------------

#endif // #ifndef __CCube_h_

//---------------------------------------------------------------------------