#ifndef __Player_h_
#define __Player_h_

class Object;

//Clase para creacion de cubos
class Player
{
//Aqui se establecen las declaraciones de metodos y variables
public:

	//Constructor, destructor
    Player(void);
    virtual ~Player(void);
	//METODOS publicos
	bool keyboardControl();
	void updateAnimation();

	//VARIABLES PUBLICAS

	//Velocidad del juego (duracion de cada frame)
	double FPS;

	//Declaracion de la entidad y del nodo correspondiente al cubo
	Ogre::Entity* entity;
	Ogre::SceneNode* node;

	//Nodos de colision, si colisiona con un nodo lo almacena aqui y decide como actuar
	Object* m_obj;
	Object* m_objGravity;

	//Velocidad de movimiento
	Ogre::Real m_moveX;
	Ogre::Real m_moveY;

	//Aceleracion
	Ogre::Real accX;
	Ogre::Real accY;

	//Otras variables
	bool m_jumpUp;
	bool m_crouchDown;
	Ogre::Real m_jumpCount;
	Ogre::Real m_maxNumJump;

	double m_gravity;
	Ogre::Vector3 m_direction;

	char state;

private:

	//METODOS
	void keyPressed(void);
	void collisionSolution(Ogre::Vector3 vDistanceGravity, Ogre::Vector3 vDistance);
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

#endif // #ifndef __Player_h_

//---------------------------------------------------------------------------