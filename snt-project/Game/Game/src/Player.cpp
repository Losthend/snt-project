#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "Player.h"
//Acceso al objeto
#include "Object.h"
//Acceso a las variables globales
#include "Global.h"
//Acceso a Collision y a KeyboardMouse
#include "Collision.h"
//Para escribir en el log
#include "FuncionesGenerales.h"
//Para obtener los FPS
#include "FrameRate.h"

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
	//entity = gSceneMgr->createEntity(entName, "robot.mesh");
	//-----------Cubo creado de forma manual-------------------
	entity = gSceneMgr->createEntity(m_entName, "ManualObjectCube");
	node = gSceneMgr->getRootSceneNode()->createChildSceneNode(m_nodeName);
	node->attachObject(entity);

	
	//Posicionamiento y escalado (por defecto)
	node->setPosition(0.0, 0.0, 0.0);
	node->scale(1.0, 1.0, 1.0);

	/*
	//Asociamos las animaciones. Requier el skeleton en la carpeta de media.
	pWalk = entity->getAnimationState("Walk");
	pWalk->setLoop(true);		
	pWalk->setLength(Ogre::Real(1.0));
	*/
	/*
	//El robot no sabe saltar, asi que esto tiene que estar comentado o usar el ninja
	pJump = entity->getAnimationState("Jump");
	pJump->setLoop(true);		
	pJump->setLength(Ogre::Real(1.0));
	*/
}
//---------------------------------------------------------------------------
Player::~Player(void)
{
	delete this;
}

//---------------------------------------------------------------------------
//--Control, mediante teclado, del jugador
//---------------------------------------------------------------------------
bool Player::keyboardControl()
{
	if(gKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	
	//Comprobamos el teclado
	keyPressed();

	//Obtenemos el desplazamiento CON gravedad
	Ogre::Real temp = m_direction.y;
	m_direction.y = m_direction.y - m_gravity;
	Ogre::Vector3 vDistance = m_direction * FPS;
	std::vector<Ogre::Vector3> vPlayerCoords = simulateOccupiedCoords(node, vDistance);	
	Object* obj = testCollisionAABB(vPlayerCoords, m_nodeName);

	//Si colisiona con gravedad probamos sin gravedad
	if (obj != 0)
	{
		//Obtenemos el desplazamiento SIN gravedad
		m_direction.y = temp;
		vDistance = m_direction * FPS;
		vPlayerCoords = simulateOccupiedCoords(node, vDistance);
		obj = testCollisionAABB(vPlayerCoords, m_nodeName);
	}

	//Segun las colisiones realizamos una accion u otra
	collisionSolution(vDistance, obj);

	//Tratamos el salto
	jumpSolution(obj);

	//Tratamos el agarre de objetos
	if(m_catch)
		catchSolution(vDistance);

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
	//Agachado
	if(gKeyboard->isKeyDown(OIS::KC_S) && !m_jumpUp){
		m_crouchDown = true;
	}
	else{
		m_crouchDown = false;
	}

	//Izquierda
	if (gKeyboard->isKeyDown(OIS::KC_A))
	{	
		//Agachado y moviendose
		if(m_crouchDown)
			m_direction.x = -m_moveX/2;
		else
		//correr
		if(gKeyboard->isKeyDown(OIS::KC_LSHIFT))
			m_direction.x = 2*-m_moveX;
		else
			m_direction.x = -m_moveX;
	}

	// Derecha
	if (gKeyboard->isKeyDown(OIS::KC_D))
	{
		//Agachado y moviendose
		if(m_crouchDown)
			m_direction.x = m_moveX/2;
		else
		//correr
		if(gKeyboard->isKeyDown(OIS::KC_LSHIFT))
			m_direction.x = 2*m_moveX;
		else
			m_direction.x = m_moveX;
	}

	// Espacio (salto)
	if (gKeyboard->isKeyDown(OIS::KC_SPACE) && !m_jumpUp)
	{
		//Aplicamos el desplazamiento deseado en el eje Y
		m_direction.y = m_moveY;
		//Bloqueamos el salto
		m_jumpUp = true;
		//Contamos los saltos que lleva
		m_jumpCount++;
	}

	//Coger objetos
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
//Metodo para el control del personaje en funcion de las colisiones
//------------------------------------------------------------
void Player::collisionSolution(Ogre::Vector3 vDistance, Object* obj)
{
	//Si NO hay colision, permitimos el movimiento 
	if (obj == 0)
	{
		node->translate(vDistance, Ogre::Node::TS_LOCAL);
		gCamera->move(vDistance);	
	}

	//Con cualquier colision
	if (obj != 0)
	{
		//El salto se desbloquea, el contador de saltos vuelve a cero y reset de la posicion en Y
		m_jumpUp = false;
		m_jumpCount = 0;
		m_direction.y = 0;
	}
}

//=====================================================================================

//------------------------------------------------------------
//Metodo para el control del salto
//------------------------------------------------------------
void Player::jumpSolution(Object* obj)
{
	//Si NO hay colision, estas saltando y has llegado al "maximo" del salto
	if (obj == 0 && m_jumpUp && m_direction.y <= 200)
	{
		//Desbloquea el salto solo si no has superado el maximo permitido
		if (m_jumpCount < m_maxNumJump)
			m_jumpUp = false;
	}	
}

//=====================================================================================

//------------------------------------------------------------
//Metodo para el control del agarre de objetos
//------------------------------------------------------------
void Player::catchSolution(Ogre::Vector3 vDistance)
{
	//Si no has agarrado ningun objeto aun
	if (m_catchObj == 0)
	{
		//Recorres el vector de objetos
		std::vector<int>::size_type sz = vObjects.size();
		for (unsigned x=0; x < sz; x++)
		{
			//Si el objeto es de tipo 2
			Object* obj = vObjects[x];
			if (obj->m_objType == 2)
			{
				//Buscas la distancia minima entre el jugador y los extremos del objeto
				std::vector<Ogre::Vector3> vCoords = getOccupiedCoords(obj->m_node);
				Ogre::Real RIGHT_BOTTOM = gPlayer->node->_getWorldAABB().squaredDistance(vCoords[0]);
				Ogre::Real LEFT_BOTTOM = gPlayer->node->_getWorldAABB().squaredDistance(vCoords[1]);
				Ogre::Real LEFT_TOP = gPlayer->node->_getWorldAABB().squaredDistance(vCoords[2]);
				Ogre::Real RIGHT_TOP = gPlayer->node->_getWorldAABB().squaredDistance(vCoords[3]);
					
				Ogre::Real minDistance = std::min(RIGHT_BOTTOM, std::min(LEFT_BOTTOM, std::min(LEFT_TOP, RIGHT_TOP)));
				
				//Si la distancia entre el jugador y el objeto es menor de 10 unidades, lo agarras
				if (minDistance < 10)
				{
					m_catchObj = obj;
				}
			}
		}
	}
	//Si has agarrado un objeto
	else
	{
		m_catchObj->update(vDistance);
	}
}




//---------------------------------------------------------------------------
//--Animacion de movimiento del personaje
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