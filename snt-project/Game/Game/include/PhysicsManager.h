#ifndef __PhysicsManager_h_
#define __PhysicsManager_h_

class SceneObject;

class PhysicsManager 
{
public:
    PhysicsManager();
    virtual ~PhysicsManager(void);
	void update(float ticks);

	//Otros metodos
	btRigidBody* createBody(const btTransform &transform, float mass, btCollisionShape &shape);
	btCollisionShape& createBoxShape(float x, float y, float z);

	SceneObject* createBoxObject(const char *name, const Ogre::Vector3 &size, const Ogre::Vector3 &pos, float mass, btCollisionShape &shape);

private:
	/*
		mWorld: simulara un mundo fisico con objetos en movimiento. 
				La detecci�n de colision en este modo puede tener "efecto tunel", hay alternativas.

		mCollisionDispatcher: decide como tratar las colisiones, gestion de los objetos que colisionan, etc.

		mCollisionConfig: utilizada para configurar el btCollisionDispacher. 
				Proporciona informaci�n sobre agrupaciones de almacenamiento intermedios utilizados durante las colisiones.

		mBroadphase: se encarga de la primera fase de detecci�n de colisiones, es decir, quien y con quien se choca.
					Seran incluidos en el btDispatcher que decidir� quien colisiona o no.
	*/

	btDiscreteDynamicsWorld *mWorld;
	btCollisionDispatcher *mCollisionDispatcher;

	btDefaultCollisionConfiguration mCollisionConfig;
	btDbvtBroadphase mBroadphase;
	btSequentialImpulseConstraintSolver mConstraintSolver;

};

//---------------------------------------------------------------------------

#endif // #ifndef __PhysicsManager_h_

//---------------------------------------------------------------------------