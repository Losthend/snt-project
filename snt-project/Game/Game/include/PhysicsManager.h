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

	SceneObject* createBoxObject(const char *name, const Ogre::Vector3 &size, const Ogre::Vector3 &pos, float mass, Ogre::String meshName);
	SceneObject* createGroundObject(Ogre::String name, Ogre::Vector3 pos, Ogre::Vector3 size, bool grass);
	void createGroundGrass(Ogre::Vector3 vSize, Ogre::Vector3 vPos);

private:
	//mWorld: simulara un mundo fisico con objetos en movimiento. 
	btDiscreteDynamicsWorld *mWorld;
	//mCollisionDispatcher: decide como tratar las colisiones, gestion de los objetos que colisionan, etc.
	btCollisionDispatcher *mCollisionDispatcher;
	//mCollisionConfig: utilizada para configurar el btCollisionDispacher. Proporciona información sobre agrupaciones de almacenamiento intermedios utilizados durante las colisiones.
	btDefaultCollisionConfiguration mCollisionConfig;
	//mBroadphase: se encarga de la primera fase de detección de colisiones, es decir, quien y con quien se choca. Seran incluidos en el btDispatcher que decidirá quien colisiona o no.
	btDbvtBroadphase mBroadphase;
	btSequentialImpulseConstraintSolver mConstraintSolver;

};

//---------------------------------------------------------------------------

#endif // #ifndef __PhysicsManager_h_

//---------------------------------------------------------------------------