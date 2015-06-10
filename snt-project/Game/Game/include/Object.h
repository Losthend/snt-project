#ifndef __Object_h_
#define __Object_h_

class SceneObject;

class Object 
{

public:
    Object(int objType, SceneObject* sceneObject);
    virtual ~Object(void);
	void update();
	void type2();
	void type3();
	void type4();
	void type5();

	//SceneObject: Node + btRigidBody
	SceneObject*		m_sceneObject;
	//Velocidad del juego (duracion de cada frame)
	double				FPS;
	//Tipo de objeto
	int					m_objType; 
	//Velocidad de movimiento
	Ogre::Real			m_moveX;
	Ogre::Real			m_moveY;
	//Vector de movimiento/direccion
	Ogre::Vector3		m_direction;
	bool				m_lookAt;

};

//---------------------------------------------------------------------------

#endif // #ifndef __Object_h_

//---------------------------------------------------------------------------