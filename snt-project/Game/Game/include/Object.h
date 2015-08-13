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
	void type6();
	void type7();
	void type8();

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
	//Vida del objeto
	Ogre::Real			m_lifeCounter;

};

//---------------------------------------------------------------------------

#endif // #ifndef __Object_h_

//---------------------------------------------------------------------------