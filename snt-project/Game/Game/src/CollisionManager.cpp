#include "../include/stdafx.h"

#include "../include/CollisionManager.h"
#include "../include/Global.h"
#include "../include/Player.h"
#include "../include/Object.h"

CollisionManager::CollisionManager()
{
	m_collision = false;
}

CollisionManager::~CollisionManager(void)
{
}

btScalar CollisionManager::addSingleResult(btManifoldPoint& cp,const btCollisionObjectWrapper* objWrap0, int partId0, int index0,
											const btCollisionObjectWrapper* objWrap1, int partId1, int index1)
{
    if (cp.getDistance() <= 0.f)
		m_collision = true;
	else
		m_collision = false;

	return 0;
}