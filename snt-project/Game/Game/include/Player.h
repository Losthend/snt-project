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
	void Player::catchAttack();
	Ogre::Real RayCastingPlayerMouse();
	void animationManager();
	void animWalk(Ogre::AnimationState* mAnimWalk);
	void animJump(Ogre::AnimationState* mAnimJump);
	void animFall(Ogre::AnimationState* mAnimJump);
	void animCrouchDown(Ogre::AnimationState* mAnimCrouch);
	void animCrouchUp(Ogre::AnimationState* mAnimCrouch);
	void fallManager();
	Object* rayFromPoint(Ogre::Vector3 origin, Ogre::Vector3 direction);

	//VARIABLES

	//SceneObject: Node + btRigidBody
	SceneObject*		m_sceneObject;
	//Velocidad del juego (duracion de cada frame)
	double				FPS;
	//Velocidad de movimiento
	Ogre::Real			m_moveX;
	Ogre::Real			m_moveY;
	//Variables de salto
	bool				m_fall;
	bool				m_jump;
	Ogre::Real			m_jumpCount;
	Ogre::Real			m_maxNumJump;
	//Agacharse y correr
	bool				m_crouchDown;
	bool				m_crouchUp;
	bool				m_run;
	//Direccion: TRUE:derecha(x+) / FALSE:izquierda(x-)
	bool				m_lookAt;
	//Telequinesis
	Object* m_catchObj;
	Ogre::Real m_tkDistance;
	//Vector de movimiento/direccion
	Ogre::Vector3		m_direction;

};

//---------------------------------------------------------------------------

#endif // #ifndef __Player_h_

//---------------------------------------------------------------------------