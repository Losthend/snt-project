#include "stdafx.h"
#include "Collision.h"
#include <OgreSceneQuery.h>

//----------------------------------------------------------------------
//Comprobacion de los objetos que actualmente estan en el frustum
//----------------------------------------------------------------------
std::vector<Ogre::SceneNode*> inCameraFrustumObjects(Ogre::Camera* mCamera, Ogre::SceneManager* mSceneMgr)
{
	//Objetos necesarios
	Ogre::PlaneBoundedVolumeList volList;
	Ogre::PlaneBoundedVolume vol;
	Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery;
	Ogre::SceneQueryResult result;
	Ogre::SceneQueryResultMovableList::iterator iter;

	//Obtenemos un objeto que engloba aquello que este contenido en el frustum actual
	vol = mCamera->getCameraToViewportBoxVolume(1, 1, 0, 0, true);

	//Obtenemos la lista de objetos
	volList.push_back(vol);
	 
	//Creamos la query para consultas dentro de un volumen plano acotado
	mVolQuery = mSceneMgr->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());

	//Adaptamos la query al volumen de objetos
	mVolQuery->setVolumes(volList);

	//Realizamos la query y obtenemos el iterador
	result = mVolQuery->execute();

	//Vector para almacenar los nodos
	std::vector<Ogre::SceneNode*> sceneNodes;
	//Vaciado del vector para evitar errores
	sceneNodes.clear();

	//Obtenemos el nombre de los NODOS que se encuentran dentro del frustum visible
	for (iter = result.movables.begin(); iter != result.movables.end(); ++iter)
	{
		//Guardamos los nodos en el vector
		sceneNodes.push_back((*iter)->getParentSceneNode());

		//Funcionalidad para pruebas: gira aquellos elementos que esten en el frustum, excepto el personaje
		//Ogre::String nombre = (*iter)->getParentSceneNode()->getName();
		//if (nombre != "cubeNode1")
		//(*iter)->getParentSceneNode()->roll(Ogre::Degree(0.1));
	}

	return sceneNodes;
}

//----------------------------------------------------------------------
//Comprueba las colisiones entre los nodos del frustum y actua en consecuencia
//----------------------------------------------------------------------
void collisionManager(std::vector<Ogre::SceneNode*> sceneNodes)
{
	//Obtenemos un iterador sobre el vector principal de sceneNodes
	//std::vector<Ogre::SceneNode*>::iterator iter1 = sceneNodes.begin();

	//Tamaño del vector
	std::vector<int>::size_type sz = sceneNodes.size();

	//sceneNodes para comparar
	Ogre::SceneNode* node1;
	Ogre::SceneNode* node2;

	//Variable de deteccion de colision
	bool collision;

	//Deteccion de las colisiones entre nodos
	for (unsigned i=0; i<sz; i++)
	{
		node1 = sceneNodes[i];
		for (unsigned x=i+1; x<sz; x++)
		{
			node2 = sceneNodes[x];
			collision = AABBcollisionDetection(node1, node2);
			if (collision)
			{
				//Muestra en el log de debug los nodos detectados
				Ogre::String colisiones = node1->getName() + " - COLISIONA - " + node2->getName();
				OutputDebugString(colisiones.c_str());
				OutputDebugString("\n");
			}
		}
	}
}


//----------------------------------------------------------------------
//Deteccion de colisiones AABB entre dos nodos
//----------------------------------------------------------------------
bool AABBcollisionDetection(Ogre::SceneNode* node1, Ogre::SceneNode* node2)
{
	//Colision entre dos nodos 
	Ogre::AxisAlignedBox aabb = node1->_getWorldAABB().intersection(node2->_getWorldAABB());

	if(!aabb.isNull())
		return true;

	return false;
}
