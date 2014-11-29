#include "stdafx.h"
#include <OgreSceneQuery.h>


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

//----------------------------------------------------------------------
//Comprobacion de los objetos que actualmente estan en el frustum
//----------------------------------------------------------------------
void inCameraFrustumObjects(Ogre::Camera* mCamera, Ogre::SceneManager* mSceneMgr)
{

	//Objetos necesarios
	Ogre::PlaneBoundedVolumeList volList;
	Ogre::PlaneBoundedVolume vol;
	Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery;
	Ogre::SceneQueryResult result;
	Ogre::SceneQueryResultMovableList::iterator iter;
	std::vector<std::string> vector;

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

	//Vaciamos el contenido anterior del vector 
	vector.clear();

	//Obtenemos el nombre de los NODOS que se encuentran dentro del frustum visible
	for (iter = result.movables.begin(); iter != result.movables.end(); ++iter)
	{
		//Obtenemos el nombre del objeto
		Ogre::String nombre = (*iter)->getParentSceneNode()->getName();
		//vector.push_back(nombre);
		if (nombre != "cubeNode1")
		(*iter)->getParentSceneNode()->roll(Ogre::Degree(0.1));
	}
	
	//std::vector<std::string>
}

	//http://www.ogre3d.org/docs/api/1.9/class_ogre_1_1_plane_bounded_volume_list_scene_query.html
	//http://ogre.sourcearchive.com/documentation/1.6.4.dfsg1-1/classOgre_1_1Camera_18a5f06c6bf48445d3602f2d336d80c4.html
	//http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Intermediate+Tutorial+4
	//http://www.ogre3d.org/forums/viewtopic.php?f=2&t=80323

