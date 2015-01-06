#include "stdafx.h"
#include "Collision.h"

#include "Global.h"
#include "Player.h"
#include "Object.h"

#include "FuncionesGenerales.h"

//----------------------------------------------------------------------
//Comprobacion de los objetos que actualmente estan en el frustum
//----------------------------------------------------------------------
std::vector<Ogre::SceneNode*> inCameraFrustumObjects(void)
{
	//Objetos necesarios
	Ogre::PlaneBoundedVolumeList volList;
	Ogre::PlaneBoundedVolume vol;
	Ogre::PlaneBoundedVolumeListSceneQuery* mVolQuery;
	Ogre::SceneQueryResult result;
	Ogre::SceneQueryResultMovableList::iterator iter;

	//Obtenemos un objeto que engloba aquello que este contenido en el frustum actual
	vol = gCamera->getCameraToViewportBoxVolume(1, 1, 0, 0, true);

	//Obtenemos la lista de objetos
	volList.push_back(vol);
	 
	//Creamos la query para consultas dentro de un volumen plano acotado
	mVolQuery = gSceneMgr->createPlaneBoundedVolumeQuery(Ogre::PlaneBoundedVolumeList());

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
	}

	return sceneNodes;
}


//---------------------------------------------------------------------
//Devuelve un vector con las coordenadas AABB de los extremos de un nodo
//Parametros: el nodo que queremos saber cuanto ocupa
//---------------------------------------------------------------------
std::vector<Ogre::Vector3> getOccupiedCoords(Ogre::SceneNode* node)
{
	//Declaramos el vector a devolver
	std::vector<Ogre::Vector3> vNodeCoords;

	//Obtenemos las coordenadas de los extremos de la cara frontal del cubo, la unica con la que trabajaremos
	Ogre::AxisAlignedBox aabb = node->_getWorldAABB();
	Ogre::Vector3 RIGHT_BOTTOM = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
	Ogre::Vector3 LEFT_BOTTOM = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::Vector3 LEFT_TOP = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_TOP);
	Ogre::Vector3 RIGHT_TOP = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_TOP);
	//Añadimos un vertice del plano trasero para disponer de la profundidad maxima
	Ogre::Vector3 FAR_Z = aabb.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_TOP);

	//Conociendo esos limites, ya tenemos todas las coordenadas ocupadas por el nodo

	//Las almacenamos en el vector de cuatro en cuatro por cada nodo y en un orden concreto
	vNodeCoords.push_back(RIGHT_BOTTOM);
	vNodeCoords.push_back(LEFT_BOTTOM);
	vNodeCoords.push_back(LEFT_TOP);
	vNodeCoords.push_back(RIGHT_TOP);
	//Añadimos un vertice del plano trasero para disponer de la profundidad maxima
	vNodeCoords.push_back(FAR_Z);

	return vNodeCoords;
}

//---------------------------------------------------------------------
//Devuelve un vector con las coordenadas del contorno que OCUPARA un nodo en el siguiente movimiento
//Parametros: el nodo que queremos saber cuanto ocupa y la distancia que se desplazara en ese movimiento
//---------------------------------------------------------------------
std::vector<Ogre::Vector3> simulateOccupiedCoords(Ogre::SceneNode* node, Ogre::Vector3 vDistance)
{
	//Obtenemos las coordenadas de los extremos de la cara frontal del cubo, la unica con la que trabajaremos
	Ogre::AxisAlignedBox aabb = node->_getWorldAABB();
	Ogre::Vector3 RIGHT_BOTTOM = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_BOTTOM);
	Ogre::Vector3 LEFT_BOTTOM = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_BOTTOM);
	Ogre::Vector3 LEFT_TOP = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_LEFT_TOP);
	Ogre::Vector3 RIGHT_TOP = aabb.getCorner(Ogre::AxisAlignedBox::NEAR_RIGHT_TOP);
	//Añadimos un vertice del plano trasero para disponer de la profundidad maxima
	Ogre::Vector3 FAR_Z = aabb.getCorner(Ogre::AxisAlignedBox::FAR_RIGHT_TOP);

	//Conociendo esos limites, todas las coordenadas entre ellos estaran ocupadas por el objeto
	std::vector<Ogre::Vector3> vSimulateCoords;

	//Almacenamos las coordenadas de los extremos
	vSimulateCoords.push_back(Ogre::Vector3(RIGHT_BOTTOM.x + vDistance.x, RIGHT_BOTTOM.y + vDistance.y, 0));
	vSimulateCoords.push_back(Ogre::Vector3(LEFT_BOTTOM.x + vDistance.x, LEFT_BOTTOM.y + vDistance.y, 0));
	vSimulateCoords.push_back(Ogre::Vector3(LEFT_TOP.x + vDistance.x, LEFT_TOP.y + vDistance.y, 0));
	vSimulateCoords.push_back(Ogre::Vector3(RIGHT_TOP.x + vDistance.x, RIGHT_TOP.y + vDistance.y, 0));
	//Añadimos un vertice del plano trasero para disponer de la profundidad maxima
	//En este no hace falta simular el movimiento, la Z es la misma
	vSimulateCoords.push_back(FAR_Z);

	return vSimulateCoords;

}


//---------------------------------------------------------------------
//Dadas las coordenadas simuladas de un nodo, comprueba si en ese movimiento se produce o no una colision.
//Parametro: las coordenadas simuladas del futuro emplazamiento del nodo y nombre del nodo para no compararlo consigo mismo
//---------------------------------------------------------------------
Object* testCollisionAABB(std::vector<Ogre::Vector3> vSimulatedCoords, Ogre::String nodeName)
{
	/*
		Orden de los extremos en los vectores:
		[0] RIGHT_BOTTOM
		[1] LEFT_BOTTOM
		[2] LEFT_TOP
		[3] RIGHT_TOP
		[4] FAR_Z
	*/

	//Declaramos bool de colision
	bool collision = false;

	//Objeto "Object" a devolver
	Object* obj = 0;

	//Obtenemos el tamaño del vector global "vObjects"
	std::vector<int>::size_type sz = vObjects.size();

	//OPTIMIZABLE CON: inCameraFrustumObjects
	//Comparamos colisiones con TODOS los objetos, minimo hasta encontrar alguna colision
	for (unsigned x=0; x < sz && !collision; x++)
	{
		if (nodeName != vObjects[x]->m_node->getName())
		{
			//Obtenemos los extremos del objeto
			std::vector<Ogre::Vector3> vNodeCoords = getOccupiedCoords(vObjects[x]->m_node);

			//Teorema del plano de separacion
			//Proyectamos las cajas sobre cada uno de los ejes y si alguna de las proyecciones
			//No se solapan, podremos asegurar que no existe colision entre las AABB.
			if (vSimulatedCoords[0].x > vNodeCoords[1].x &&
				vSimulatedCoords[1].x < vNodeCoords[0].x &&
				vSimulatedCoords[2].y > vNodeCoords[1].y &&
				vSimulatedCoords[1].y < vNodeCoords[2].y &&
				vSimulatedCoords[4].z <= vNodeCoords[2].z &&
				vSimulatedCoords[2].z >= vNodeCoords[4].z)
			{
				collision = true;
				obj = vObjects[x];
			}
		}
	}

	return obj;
}

//---------------------------------------------------------------------
//Dadas las coordenadas simuladas de un nodo, comprueba si en ese movimiento se produce o no una colision con el jugador
//Parametro: las coordenadas simuladas del futuro emplazamiento del nodo y nodo con el que colisionara
//---------------------------------------------------------------------
bool testCollisionWithSingleNode(std::vector<Ogre::Vector3> vSimulatedCoords, Ogre::SceneNode* node)
{
	std::vector<Ogre::Vector3> vPlayerCoords = getOccupiedCoords(node); 

	if (vSimulatedCoords[0].x > vPlayerCoords[1].x &&
		vSimulatedCoords[1].x < vPlayerCoords[0].x &&
		vSimulatedCoords[2].y > vPlayerCoords[1].y &&
		vSimulatedCoords[1].y < vPlayerCoords[2].y &&
		vSimulatedCoords[4].z <= vPlayerCoords[2].z &&
		vSimulatedCoords[2].z >= vPlayerCoords[4].z)
	{
		return true;
	}

	return false;
}


//------------------------------------------------------------
//Metodo para obtener el movimiento optimo tras una colision para acabar cerca del objeto sin colisionar
//------------------------------------------------------------
Ogre::Real collisionCorrection(Ogre::SceneNode* node1, Ogre::SceneNode* node2)
{
	//Buscas la distancia minima entre cualquier punto de un nodo (node1) y los extremos del otro (node2)
	std::vector<Ogre::Vector3> vCoords = getOccupiedCoords(node1);
	Ogre::Real RIGHT_BOTTOM = node2->_getWorldAABB().squaredDistance(vCoords[0]);
	Ogre::Real LEFT_BOTTOM = node2->_getWorldAABB().squaredDistance(vCoords[1]);
	Ogre::Real LEFT_TOP = node2->_getWorldAABB().squaredDistance(vCoords[2]);
	Ogre::Real RIGHT_TOP = node2->_getWorldAABB().squaredDistance(vCoords[3]);
		
	Ogre::Real minDistance = std::min(RIGHT_BOTTOM, std::min(LEFT_BOTTOM, std::min(LEFT_TOP, RIGHT_TOP)));

	return minDistance;
}