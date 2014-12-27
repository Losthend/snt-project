#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "CPlayer.h"
//Acceso a las variables globales
#include "Global.h"
//Acceso a Collision y a KeyboardMouse
#include "Collision.h"
//Para escribir en el log
#include "FuncionesGenerales.h"

//---------------------------------------------------------------------------
CPlayer::CPlayer(void)
{
	//Variable para indicar si esta colisionando o no
	m_collision = false;
	m_collisionGravity = false;

	//Velocidad por defecto
	m_moveX = 150;
	m_moveY = 300;

	//Aceleracion
	accX = 2;
	accY = 6;

	//Otras variables
	m_jumpUp = false;
	m_gravity = 0.98;
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
CPlayer::~CPlayer(void)
{
	delete this;
}

//---------------------------------------------------------------------------
//--Control, mediante teclado, del jugador
//---------------------------------------------------------------------------
bool CPlayer::keyboardControl(const Ogre::FrameEvent& evt)
{
	if(gKeyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
	
	//Comprobamos el teclado
	keyPressed();

	//Primero, obtenemos cuanto nos vamos a desplazar
	Ogre::Vector3 vDistance = m_direction * evt.timeSinceLastFrame;
	//Desplazamiento con gravedad
	m_direction.y = m_direction.y - m_gravity;
	Ogre::Vector3 vDistanceGravity = m_direction * evt.timeSinceLastFrame;

	//Segundo, para cada nodo en el frustum obtenemos las coordenadas que ocupa (excluyendo las del jugador)
	std::vector<Ogre::Vector3> vAllCoords = getAllOccupiedCoords();

	//Tercero, obtenemos las coordenadas que "ocupara" el jugador en el "siguiente movimiento"
	std::vector<Ogre::Vector3> vPlayerCoords = simulateOccupiedCoords(node, vDistance);
	//Coordenadas que ocupara con gravedad
	std::vector<Ogre::Vector3> vPlayerCoordsGravity = simulateOccupiedCoords(node, vDistanceGravity);

	//Finalmente, comprobamos si alguna de las coordenadas del jugador "colisiona" con alguna de las "ocupadas"
	m_collision = testColision(vAllCoords, vPlayerCoords);
	m_collisionGravity = testColision(vAllCoords, vPlayerCoordsGravity);

	//Si NO hay colision teniendo gravedad, permitimos el movimiento
	if (!m_collisionGravity)
	{
		//Desplazas el nodo		
		node->translate(vDistanceGravity, Ogre::Node::TS_LOCAL);	
		//Desplazar la cámara
		gCamera->move(vDistanceGravity);	
	}
	else if (!m_collision)
	{
		//Si no hay colision sin gravedad, permitimos el movimiento (ya esta sobre el suelo)
		node->translate(vDistance, Ogre::Node::TS_LOCAL);
		//Desplazar la camara con el personaje
		gCamera->move(vDistance);	
	}
	
	//Tratamos el salto
	jumpSolution();

	//Reset del movimiento en X
	m_direction.x = 0;	

	return true;
}

//------------------------------------------------------------
//Metodo para el control de los inputs del teclado
//------------------------------------------------------------
void CPlayer::keyPressed(void)
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
	}
}

//------------------------------------------------------------
//Metodo para el control del salto
//REQUIERE OPTIMIZACION, esta hecho a "ojo"... y el salto no es perfecto
//------------------------------------------------------------
void CPlayer::jumpSolution(void)
{
	//Si no hay colision, estas saltando y estas subiendo a una velocidad <=125
	if (!m_collisionGravity && m_jumpUp && m_direction.y <= 125)
	{
		//Hemos llegado al "maximo" del salto, cancelamos la subida y empezamos el descenso
		m_jumpUp = false;	
		m_direction.y = 0;
	}
	else if (m_collisionGravity || m_collision)
	{
		//Si se produce una colision durante el salto, detenemos el salto
		m_jumpUp = false;
		m_direction.y = 0;
	}
}

//---------------------------------------------------------------------------
//--Animacion de movimiento del personaje
//---------------------------------------------------------------------------
/*
void CPlayer::updateAnimation(){

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
//Obtenemos los objetos visibles del frustum
	std::vector<Ogre::SceneNode*> sceneNodes = inCameraFrustumObjects();
	//Creamos un vector tridimensional de ceros
	Ogre::Vector3 transVector = Ogre::Vector3::ZERO;
	
	//Movimiento del personaje	
	//Arriba
	if (gKeyboard->isKeyDown(OIS::KC_W)){
		if(!collisionManager(sceneNodes, node)){
			transVector.y += mMoveY;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			
		}
		else{
			transVector.y -= 2 * mMoveY;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

	// Abajo
	if (gKeyboard->isKeyDown(OIS::KC_S)){
		if(!collisionManager(sceneNodes, node)){
			transVector.y -= mMoveY;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			transVector.y += 2 * mMoveY;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
	}

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

	//espacio	
	if (gKeyboard->isKeyDown(OIS::KC_SPACE)){
		saltar = true;
	}

	if(saltar){
		gPlayer->state = 0x2;
		v = node->getPosition();
		//Tener en cuenta que al multiplicar por el evt siempre tambien modificamos los valores de 'x' y de 'z' si no valen exactamente 0
		//Segun mis cuentas el valor del evt es aproximadamente (1/100)
		if(!collisionManager(sceneNodes, node)){
			mMoveY = mMoveY + (mGravedad  * evt.timeSinceLastFrame);
			node->translate(Ogre::Vector3(v[0], mMoveY, v[2]) * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
		}
		else{
			imprimir("aqui se invierte arriba y abajo?");
			transVector.y += 2 * mMoveY;
			node->translate(transVector * evt.timeSinceLastFrame, Ogre::Node::TS_LOCAL);
			saltar = false;
		}
	}
*/