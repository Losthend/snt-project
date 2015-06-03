#ifndef __CollisionManager_h_
#define __CollisionManager_h_

class Player;
class Object;

class CollisionManager: public btCollisionWorld::ContactResultCallback
{
public:
	CollisionManager();
	virtual ~CollisionManager(void);

	btScalar addSingleResult(btManifoldPoint& cp,const btCollisionObjectWrapper* objWrap0, int partId0, int index0,
												const btCollisionObjectWrapper* objWrap1, int partId1, int index1);

	bool	m_collision;
};

//---------------------------------------------------------------------------

#endif // #ifndef __CollisionManager_h_

//---------------------------------------------------------------------------
