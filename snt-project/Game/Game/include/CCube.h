#ifndef __CCube_h_
#define __CCube_h_

#include "stdafx.h"

//Clase para creacion de cubos
class CCube 
{
//Variables
	//Nodo sobre el que aplicar cualquier cambio
	Ogre::SceneNode* cubeNode;
	//Para saber si es colisionable o no
	bool m_collision;
	//Velocidad de movimiento del objeto
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

//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor, destructor
    CCube(void);
    virtual ~CCube(void);

	//METODOS publicos
	void createCube(Ogre::SceneManager* mSceneMgr, std::string id);
	void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	void setScale(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	void setRotation(Ogre::Real x, Ogre::Real y, Ogre::Real z);
	Ogre::SceneNode* getAssociatedNode(void);

private:

	//METODOS privados

};

//---------------------------------------------------------------------------

#endif // #ifndef __CCube_h_

//---------------------------------------------------------------------------