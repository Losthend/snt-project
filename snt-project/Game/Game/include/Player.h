#ifndef __Player_h_
#define __Player_h_

class SceneObject;
class Object;

class Player
{
public:

	//FUNCIONES

    Player(SceneObject* sceneObject);
    virtual ~Player(void);
	void update();
	void catchSolution(Object* obj);
	void catchActions();
	Ogre::Real RayCastingPlayerMouse();

	//VARIABLES

	//SceneObject: Node + btRigidBody
	SceneObject*		m_sceneObject;
	//Velocidad del juego (duracion de cada frame)
	double				FPS;
	//Velocidad de movimiento
	Ogre::Real			m_moveX;
	Ogre::Real			m_moveY;
	//Variables de salto
	bool				m_jump;
	Ogre::Real			m_jumpCount;
	Ogre::Real			m_maxNumJump;
	//Agacharse y correr
	bool				m_crouchDown;
	bool				m_run;
	//Telequinesis
	Object* m_catchObj;
	Ogre::Real m_tkDistance;
	//Vector de movimiento/direccion
	Ogre::Vector3		m_direction;

};

//---------------------------------------------------------------------------

#endif // #ifndef __Player_h_

//---------------------------------------------------------------------------