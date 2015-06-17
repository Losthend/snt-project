#ifndef __PhysicsManager_h_
#define __PhysicsManager_h_

class SceneObject;
class Object;

class PhysicsManager 
{
public:
    PhysicsManager();
    virtual ~PhysicsManager(void);
	void update(float ticks);

	btRigidBody* createBody(const btTransform &transform, float mass, btCollisionShape* shape);
	
	SceneObject* createPrimitiveShape(Ogre::String name, Ogre::Vector3 size, Ogre::Vector3 pos, float mass, Ogre::String meshName, bool type);
	SceneObject* createConvexHullShape(Ogre::String name, Ogre::Real size, Ogre::Vector3 pos, float mass, Ogre::String meshName);	
	SceneObject* createGroundShape(Ogre::String name, Ogre::Vector3 size, Ogre::Vector3 pos, Ogre::Vector2 repeat, Ogre::String material);
	
	void magicGenerator(Ogre::Vector3 pos);

	void getMeshInformation(const Ogre::Entity* const ent,
                        size_t &vertex_count,
                        Ogre::Vector3* &vertices,
                        size_t &index_count,
                        unsigned long* &indices,
                        const Ogre::Vector3 &position,
                        const Ogre::Quaternion &orient,
                        const Ogre::Vector3 &scale);

	Object* magicObj;

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