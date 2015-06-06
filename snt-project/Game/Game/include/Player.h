#ifndef __Player_h_
#define __Player_h_

class SceneObject;
class Object;
class AnimationManager;

class Player
{
public:

    Player(SceneObject* sceneObject);
    virtual ~Player(void);

	void update();
	void Player::catchAttack();
	Ogre::Real RayCastingPlayerMouse();
	bool fallManager();
	Object* rayFromPoint(Ogre::Vector3 origin, Ogre::Vector3 direction);

	//SceneObject: Node + btRigidBody
	SceneObject*		m_sceneObject;
	//Frame per second
	double				FPS;
	//Animaciones
	AnimationManager*	m_animMgr;
	//Velocidad de movimiento
	Ogre::Real			m_moveX;
	Ogre::Real			m_moveY;
	//Variables de salto
	bool				m_fall;
	bool				m_jump;
	bool				m_inJump;
	//Correr
	bool				m_run;
	//Espadas y ataque
	bool				m_SwordsDrawn;
	bool				m_attack;
	//Direccion: TRUE:derecha(x+) / FALSE:izquierda(x-)
	bool				m_lookAt;
	//Telequinesis
	Object* m_catchObj;
	//Vector de movimiento/direccion
	Ogre::Vector3		m_direction;

};

//---------------------------------------------------------------------------

#endif // #ifndef __Player_h_

//---------------------------------------------------------------------------