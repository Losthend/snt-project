#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "Player.h"
//Acceso a las variables globales
#include "Global.h"
//Acceso a Collision y a KeyboardMouse
#include "Collision.h"
//Para escribir en el log
#include "FuncionesGenerales.h"

//---------------------------------------------------------------------------
Player::Player(void)
{
	//Variable para indicar si esta colisionando o no
	m_obj = 0;
	m_objGravity = 0;

	//Velocidad por defecto
	m_moveX = 150;
	m_moveY = 300;

	//Aceleracion
	accX = 2;
	accY = 6;

	//Gravedad
	m_gravity = 0.98;

	//Variables de saltos
	m_jumpUp = false;
	m_jumpCount = 0;
	m_maxNumJump = 2; //Maximo numero de saltos

	//Vector de direccion del personaje
	m_direction = Ogre::Vector3::ZERO;

	//Estado por defecto del personaje (0=quieto)
	state = 0x0;

	//Creamos el cubo con su entidad y nodo correspondiente
	//Identificadores
	std::string entName = "entityPlayer";
	std::string nodeName = "nodePlayer";
	
	//Creacion (entidad y nodo)
	//-----------Carga de la textura del robot---------------
	//entity = gSceneMgr->createEntity(entName, "robot.mesh");
	//-----------Cubo creado de forma manual-------------------
	entity = gSceneMgr->createEntity(entName, "ManualObjectCube");
	node = gSceneMgr->getRootSceneNode()->createChildSceneNode(nodeName);
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
bool Player::keyboardControl(const Ogre::FrameEvent& evt)
{
	if(gKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	
	//Comprobamos el teclado
	keyPressed();

	//Obtenemos cuanto nos vamos a desplazar
	Ogre::Vector3 vDistance = m_direction * evt.timeSinceLastFrame;
	//Obtenemos cuanto nos vamos a desplazar con gravedad
	m_direction.y = m_direction.y - m_gravity;
	Ogre::Vector3 vDistanceGravity = m_direction * evt.timeSinceLastFrame;

	//Obtenemos las coordenadas que "ocupara" el jugador en el "siguiente movimiento"
	std::vector<Ogre::Vector3> vPlayerCoords = simulateOccupiedCoords(node, vDistance);
	//Y lo mismo pero con gravedad
	std::vector<Ogre::Vector3> vPlayerCoordsGravity = simulateOccupiedCoords(node, vDistanceGravity);

	//Comprobamos si hay colisiones
	m_obj = testCollisionAABB(vPlayerCoords);
	m_objGravity = testCollisionAABB(vPlayerCoordsGravity);

	//Segun las colisiones realizamos una accion u otra
	collisionSolution(vDistanceGravity, vDistance);

	//Tratamos el salto
	jumpSolution();

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
	//Izquierda
	if (gKeyboard->isKeyDown(OIS::KC_A))
	{		
		//correr
		if(gKeyboard->isKeyDown(OIS::KC_LSHIFT))
			m_direction.x = 2*-m_moveX;
		else
			m_direction.x = -m_moveX;
	}

	// Derecha
	if (gKeyboard->isKeyDown(OIS::KC_D))
	{
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
}

//=====================================================================================

//------------------------------------------------------------
//Metodo para el control del personaje en funcion de las colisiones
//------------------------------------------------------------
void Player::collisionSolution(Ogre::Vector3 vDistanceGravity, Ogre::Vector3 vDistance)
{
	//Si NO hay colision CON gravedad, permitimos el movimiento
	if (m_objGravity == 0)
	{	
		node->translate(vDistanceGravity, Ogre::Node::TS_LOCAL);
		gCamera->move(vDistanceGravity);	
	}
	//Si NO hay colision SIN gravedad, permitimos el movimiento (ya esta sobre el suelo)
	else if (m_obj == 0)
	{
		node->translate(vDistance, Ogre::Node::TS_LOCAL);
		gCamera->move(vDistance);	
	}

	//Con cualquier colision
	if (m_objGravity != 0 || m_obj != 0)
	{
		//El salto se desbloquea, el contador vuelve a cero y reset de la posicion en Y
		m_jumpUp = false;
		m_jumpCount = 0;
		m_direction.y = 0;
	}
}

//=====================================================================================

//------------------------------------------------------------
//Metodo para el control del salto
//------------------------------------------------------------
void Player::jumpSolution(void)
{
	//Si NO hay colision, estas saltando y has llegado al "maximo" del salto
	if (m_objGravity == 0 && m_jumpUp && m_direction.y <= 125)
	{
		if (m_jumpCount >= m_maxNumJump)
		{
			//El salto queda bloqueado
			m_jumpUp = true;
		}	
		else
		{
			//El salto queda desbloqueado
			m_jumpUp = false;
		}
	}	
}

//=====================================================================================

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