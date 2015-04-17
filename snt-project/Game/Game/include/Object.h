#ifndef __Object_h_
#define __Object_h_

class SceneObject;

class Object 
{

public:
	
	//FUNCIONES

    Object(int objType, SceneObject* sceneObject);
    virtual ~Object(void);
	int update(Ogre::Vector3 vDistance);
	void move();
	
	//VARIABLES

	//SceneObject: Node + btRigidBody
	SceneObject*		m_SceneObject;
	//Velocidad del juego (duracion de cada frame)
	double				FPS;
	//Tipo de objeto
	int					m_objType; 
	//Velocidad de movimiento
	Ogre::Real			m_moveX;
	Ogre::Real			m_moveY;
	//Vector de movimiento/direccion
	Ogre::Vector3		m_direction;

};

//---------------------------------------------------------------------------

#endif // #ifndef __Object_h_

//---------------------------------------------------------------------------