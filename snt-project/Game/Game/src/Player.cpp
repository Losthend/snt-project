#include "../include/stdafx.h"

//Las declaraciones de sus variables y metodos
#include "../include/Player.h"
//Acceso al objeto
#include "../include/Object.h"
//Acceso a las variables globales
#include "../include/Global.h"
//Acceso a Collision y a KeyboardMouse
#include "../include/Collision.h"
//Para escribir en el log
#include "../include/FuncionesGenerales.h"
//Para obtener los FPS
#include "../include/FrameRate.h"
//Para el tratamiento de colisiones
#include "../include/CollisionManagement.h"

//---------------------------------------------------------------------------
Player::Player(void)
{

	//Obtenemos la duracion de cada frame
	FPS = getframeLength();

	//Velocidad por defecto
	m_moveX = 150;
	m_moveY = 300;

	//Gravedad
	m_gravity = 9.8;

	//Variables de saltos
	m_jumpUp = false;
	m_jumpCount = 0;
	m_maxNumJump = 2; //Maximo numero de saltos

	//Por defecto no estas agarrando ningun objeto
	m_catch = false;
	m_catchObj = 0;
	m_catchPost = false;

	//Por defecto, no estas agachado ni corriendo
	m_crouchDown = false;
	m_run = false;

	//Vector de direccion del personaje
	m_direction = Ogre::Vector3::ZERO;

	//Estado por defecto del personaje (0=quieto)
	state = 0x0;

	//Creamos el cubo con su entidad y nodo correspondiente
	//Identificadores
	m_entName = "entityPlayer";
	m_nodeName = "nodePlayer";
	
	//Creacion (entidad y nodo)
	//-----------Carga de la textura del robot---------------
	//entity = gSceneMgr->createEntity(m_entName, "robot.mesh");
	//-----------Cubo creado de forma manual-------------------
	entity = gSceneMgr->createEntity(m_entName, "ManualObjectCube");
	node = gSceneMgr->getRootSceneNode()->createChildSceneNode(m_nodeName);
	node->attachObject(entity);

	
	//Posicionamiento y escalado (por defecto)
	node->setPosition(0.0, 50.0, 0.0);
	node->scale(1.0, 1.0, 1.0);

	/*
	///*
	//Asociamos las animaciones. Requier el skeleton en la carpeta de media.
	pWalk = entity->getAnimationState("Walk");
	pWalk->setLoop(true);		
	pWalk->setLength(Ogre::Real(1.0));
	*/

	/*
	//El robot no salta, asi que esto tiene que estar comentado o usar el ninja
	pJump = entity->getAnimationState("Jump");
	pJump->setLoop(true);		
	pJump->setLength(Ogre::Real(1.0));
	*/
}
//---------------------------------------------------------------------------
Player::~Player(void)
{
}

//---------------------------------------------------------------------------
//--Control, mediante teclado, del jugador
//---------------------------------------------------------------------------
bool Player::keyboardControl()
{
	if(gKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	
	//========================PRE-OPERACIONES================================

	//Comprobamos el teclado
	keyPressed();

	//Calculamos la distancia a movernos en este frame, la gravedad siempre se aplica
	m_direction.y = Ogre::Real(m_direction.y - m_gravity);
	Ogre::Vector3 vDistance = m_direction * FPS;
	//La siguiente linea eliminaria el warning
	//Ogre::Vector3 vDistance(Ogre::Real(m_direction.x*FPS), Ogre::Real(m_direction.y*FPS), Ogre::Real(m_direction.z*FPS));

	//========================PRE-VARIABLES================================

	//Definimos variables para los procesos posteriores
	std::vector<Ogre::Vector3> vCoords;
	Ogre::Real minDistance;
	Ogre::Real dist1;
	Ogre::Real dist2;
	Object* objX = 0;
	Object* objY = 0;

	//========================PRE-AGARRE================================

	//Acciones a realizar cuando tienes agarrado un objeto
	if (m_catchObj != 0)
		catchActions(vDistance);


	//========================MOVIMIENTO_X================================

	//MOVIMIENTO EN EL EJE X, solo si hay movimiento en X
	if (vDistance.x != 0)
	{
		Ogre::Vector3 tempMoveX = Ogre::Vector3(vDistance.x, 0, 0);
		//Simulamos el movimiento
		vCoords = simulateOccupiedCoords(node, tempMoveX);
		objX = testCollisionAABB(vCoords, m_nodeName);

		//Si hay colision
		if (objX != 0)
		{	
			//Buscamos el movimiento minimo para acercarse lo maximo posible al objeto sin colisionar
			dist1 = getMinDistance(node, objX->m_node);
			dist2 = getMinDistance(objX->m_node, node);
			minDistance = std::min(dist1, dist2);
			if (minDistance > 1)
			{
				//Reescribimos el movimiento en X
				if (vDistance.x > 0)
					vDistance.x =  Ogre::Real(minDistance*FPS*10);
				else
					vDistance.x = Ogre::Real(-minDistance*FPS*10);
			}
			else
				vDistance.x = 0;
		}
	}

	//========================MOVIMIENTO_Y================================

	Ogre::Vector3 tempMoveY = Ogre::Vector3(0, vDistance.y, 0);
	//Simulamos el movimiento
	vCoords = simulateOccupiedCoords(node, tempMoveY);
	objY = testCollisionAABB(vCoords, m_nodeName);

	//Si hay colision
	if (objY != 0)
	{
		//Buscamos el movimiento minimo para acercarse lo maximo posible al objeto sin colisionar
		dist1 = getMinDistance(node, objY->m_node);
		dist2 = getMinDistance(objY->m_node, node);
		minDistance = std::min(dist1, dist2);
		if (minDistance > 1)
		{
			//Reescribimos el movimiento en Y
			if (vDistance.y > 0)
				vDistance.y =  Ogre::Real(minDistance*FPS*10);
			else
				vDistance.y = 0;
		}
		else
			vDistance.y = 0;
	}

	//========================MOVIMIENTO FINAL================================

	if (vDistance.x != 0 || vDistance.y != 0)
	{
		node->translate(vDistance, Ogre::Node::TS_WORLD);
		gCamera->move(vDistance);	
	}

	//========================SALTO================================

	//Si NO hay colision, estas saltando y has llegado al "maximo" del salto
	if (objX == 0 && objY == 0 && m_jumpUp && m_direction.y <= 200)
	{
		//Desbloquea el salto solo si no has superado el maximo permitido
		if (m_jumpCount < m_maxNumJump)
			m_jumpUp = false;
	}	

	//========================POST-AGARRE================================

	//Tratamiento de objeto agarrado
	if (m_catchPost)
	{
		m_catchObj->update(vDistance);
		m_catchPost = false;
	}

	//Tratamiento de objeto No agarrado
	if(m_catch && m_catchObj == 0)
		catchSolution();

	//===================COLLISIONMANAGEMENT========================

	//recogemos el tipo de objeto con el que ha colisionado y llamamos al tratamiento de la colisionen funcion del tipo	
	if(objX != 0 )
		collisionManagementX(objX);
	if(objY != 0)
		collisionManagementY(objY);	

	//========================RESETS================================

	//NOTA: Con la condicion "objX != 0 || objY != 0" se pega a las paredes y al techo (en las paredes se resbala)
	//NOTA: Con la condicion "objY != 0" se pega al techo pero no a las paredes
	//NOTA: Con la condicion "objY != 0 && m_direction.y <= 0" no se pega ni al techo ni a las paredes
	if (objY != 0 && m_direction.y <= 0)
	{
		//Desbloqueamos el salto y evitamos terminamos el efecto de la gravedad
		m_jumpUp = false;
		m_jumpCount = 0;
		m_direction.y = 0;
	}

	//Reset del movimiento en X
	m_direction.x = 0;	

	return true;
}

//=====================================================================================

//------------------------------------------------------------
//Metodo para el control de los inputs del teclado
//------------------------------------------------------------

void Player::keyPressed(void)
{
	//Agacharse (mientras no saltes ni corras ni agarres un objeto)
	if(gKeyboard->isKeyDown(OIS::KC_S) && !m_jumpUp && !m_run && m_catchObj == 0)
		m_crouchDown = true;
	else
		m_crouchDown = false;

	//Correr (mientras no estes agachado ni agarres un objeto)
	if(gKeyboard->isKeyDown(OIS::KC_LSHIFT) && !m_crouchDown && m_catchObj == 0)
		m_run = true;
	else
		m_run = false;

	//Izquierda
	if (gKeyboard->isKeyDown(OIS::KC_A))
	{	
		//Si estas agachado
		if(m_crouchDown)
			m_direction.x = -m_moveX/2;
		//Si estas corriendo
		else if(m_run)
			m_direction.x = -m_moveX*2;
		//Si estas caminando
		else
			m_direction.x = -m_moveX;
	}

	//Derecha
	if (gKeyboard->isKeyDown(OIS::KC_D))
	{
		//Si estas agachado
		if(m_crouchDown)
			m_direction.x = m_moveX/2;
		//Si estas corriendo
		else if(m_run)
			m_direction.x = m_moveX*2;
		//Si estas caminando
		else
			m_direction.x = m_moveX;
	}

	//Saltar (mientras no estes realizando un salto)
	if (gKeyboard->isKeyDown(OIS::KC_SPACE) && !m_jumpUp)
	{
		//Aplicamos el desplazamiento deseado en el eje Y
		m_direction.y = m_moveY;
		//Bloqueamos el salto
		m_jumpUp = true;
		//Contamos los saltos que lleva
		m_jumpCount++;
	}

	//Agarrar objetos
	if(gKeyboard->isKeyDown(OIS::KC_E))
	{
		m_catch = true;
	}
	else
	{
		m_catch = false;
		m_catchObj = 0;
	}
}

//=====================================================================================

//------------------------------------------------------------
//Metodo para el agarre de objetos
//------------------------------------------------------------
void Player::catchSolution()
{
	//Recorres el vector de objetos
	std::vector<int>::size_type sz = vObjects.size();
	for (unsigned x=0; x < sz; x++)
	{
		//Si el objeto es de tipo 2
		Object* obj = vObjects[x];
		if (obj->m_objType == 2)
		{
			//Obtenemos la distancia minima entre los objetos
			Ogre::Real dist1 = getMinDistance(node, obj->m_node);
			Ogre::Real dist2 = getMinDistance(obj->m_node, node);
			Ogre::Real minDistance = std::min(dist1, dist2);

			//Si la distancia entre el jugador y el objeto es menor de 20 unidades
			if (minDistance < 20)
			{
				//Comprobamos si lo intentas agarrar por los lados (no esta permitido agarrar por arriba/debajo)
				std::vector<Ogre::Vector3> vCoordsPlayer = getOccupiedCoords(node);
				std::vector<Ogre::Vector3> vCoordsObj = getOccupiedCoords(obj->m_node);
				bool onlyX = true;

				//Si player_RIGHT_TOP.y esta por debajo del obj_RIGHT_BOTTOM.y (evitar agarrarlo por debajo)
				//Si player_RIGHT_BOTTOM.y esta por encima del obj_RIGHT_TOP.y (evitar agarrarlo por arriba)
				if ( vCoordsPlayer[3].y < vCoordsObj[0].y || vCoordsPlayer[0].y > vCoordsObj[3].y )
					onlyX = false;

				//Si solo lo estas intentando agarrar por los lados, entonces lo agarras definitivamente
				if (onlyX)
					m_catchObj = obj;
			}
		}
	}
}

//------------------------------------------------------------
//Metodo para el control de objetos agarrados
//------------------------------------------------------------
void Player::catchActions(Ogre::Vector3 vDistance)
{
	//Primero comprobamos si estamos a una distancia minima del objeto
	Ogre::Real dist1 = getMinDistance(node, m_catchObj->m_node);
	Ogre::Real dist2 = getMinDistance(m_catchObj->m_node, node);
	Ogre::Real minDistance = std::min(dist1, dist2);
	//Si estamos a una distancia aceptable
	if (minDistance < 40)
	{
		std::vector<Ogre::Vector3> vCoordsPlayer = getOccupiedCoords(node);
		std::vector<Ogre::Vector3> vCoordsObj = getOccupiedCoords(m_catchObj->m_node);
		//Nos asegurarnos de que no estas por arriba ni por debajo del objeto (podria suceder)
		//Si player_RIGHT_TOP.y esta por debajo del obj_RIGHT_BOTTOM.y (agarrado por debajo)
		//Si player_RIGHT_BOTTOM.y esta por encima del obj_RIGHT_TOP.y (agarrado por arriba)
		if ( vCoordsPlayer[3].y < vCoordsObj[0].y || vCoordsPlayer[0].y > vCoordsObj[3].y )
		{
			//Soltamos el objeto
			m_catchObj = 0;
		}
		//Comprobamos si nos movemos en la direccion en la que tenemos agarrado el objeto
		//Si player_RIGHT_BOTTOM.x < obj_LEFT_BOTTOM.x, esta agarrado por la derecha, y te diriges hacia la derecha
		else if (vCoordsPlayer[0].x < vCoordsObj[1].x && vDistance.x > 0)
		{
			//Tratar el movimiento del objeto antes que el del jugador
			m_catchObj->update(vDistance);
		}
		//Si player_LEFT_BOTTOM.x > obj_RIGHT_BOTTOM.x, esta agarrado por la izquierda, y te diriges hacia la izquierda
		else if (vCoordsPlayer[1].x > vCoordsObj[0].x && vDistance.x < 0)
		{
			//Tratar el movimiento del objeto antes que el del jugador
			m_catchObj->update(vDistance);
		}
		else
		{
			//En cualquier otro caso, debemos tratar el movimiento del objeto agarrado despues del movimiento del jugador
			m_catchPost = true;
		}
	}
	else
	{
		//Soltamos el objeto
		m_catchObj = 0;
	}

}
//---------------------------------------------------------------------------
//Animacion de movimiento del personaje
//---------------------------------------------------------------------------
/*
void Player::updateAnimation(){

	if(state == WALK){
		pWalk->setEnabled(true);
		pWalk->addTime(0.0017);
	}
	//if(state == JUMP){
	//	pJump->setEnabled(true);
	//	pJump->addTime(0.0017);
	//}
}
*/

















/*
//CODIGO REFERENTE A LAS ANIMACIONES, no borrar... sirve de referencia para la adaptacion


	// izquierda
	if (gKeyboard->isKeyDown(OIS::KC_A)){
		if(!collisionManager(sceneNodes, node)){
			transVector.x -= mMoveX;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			//animación de andar
			gPlayer->state=0x1;
		}
		else{
			//animación de quieto
			gPlayer->state=0x0;
			transVector.x += 2 * mMoveX;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);

		}
	}
	
	//Esto parece arreglarlo siempre, pero no deberia ser siempre... o si?
	if (!gKeyboard->isKeyDown(OIS::KC_A)){
		gPlayer->state=0x0;
	}

	// Derecha
	if (gKeyboard->isKeyDown(OIS::KC_D)){
		if(!collisionManager(sceneNodes, node)){
			transVector.x += mMoveX;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			//animación de andar
			gPlayer->state=0x1;
		}
		else{
			//animación de quieto
			gPlayer->state = 0x0;
			transVector.x -= 2 * mMoveX;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}
*/