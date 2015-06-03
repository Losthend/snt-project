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

	void Player::catchAttack();
	Ogre::Real RayCastingPlayerMouse();

	void animationManager();
	void animWalk(Ogre::AnimationState* mAnimWalk);
	bool animJump(Ogre::AnimationState* mAnimJump);
	bool animFall(Ogre::AnimationState* mAnimJump);
	void animCrouchDown(Ogre::AnimationState* mAnimCrouch);
	void animCrouchUp(Ogre::AnimationState* mAnimCrouch);

	bool fallManager();
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
	bool				m_inJump;
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