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
	double m_posX;
	double m_posY;
	double m_posZ;
	//Escalado
	double m_scaleX;
	double m_scaleY;
	double m_scaleZ;
	//Rotacion
	double m_yaw;
	double m_pitch;
	double m_roll;

//Aqui se establecen las declaraciones de metodos y variables
public:
	//Constructor, destructor
    CCube(void);
    virtual ~CCube(void);

	//METODOS publicos
	void createCube(Ogre::SceneManager* mSceneMgr, std::string id);
	void setPosition(double x, double y, double z);
	void setScale(double x, double y, double z);
	void setRotation(double x, double y, double z);

private:

	//METODOS privados

};

//---------------------------------------------------------------------------

#endif // #ifndef __CCube_h_

//---------------------------------------------------------------------------