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

	//Nombre del nodo y de la entidad
	std::string m_entName;
	std::string m_nodeName;

	//Declaracion de la entidad y del nodo correspondiente al cubo
	Ogre::Entity* entity;
	Ogre::SceneNode* node;

	//Velocidad de movimiento
	Ogre::Real m_moveX;
	Ogre::Real m_moveY;

	//Variables de salto
	bool m_jumpUp;
	Ogre::Real m_jumpCount;
	Ogre::Real m_maxNumJump;

	//Agacharse y correr
	bool m_crouchDown;
	bool m_run;

	//Variables de arrastrar objetos
	bool m_catch;
	Object* m_catchObj;
	bool m_catchPost;

	//Otras variables
	int m_objType;
	double m_gravity;

	//Vector de movimiento/direccion
	Ogre::Vector3 m_direction;

	char state;

private:

	//METODOS
	void keyPressed(void);
	void catchSolution();
	void catchActions(Ogre::Vector3 vDistance);

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