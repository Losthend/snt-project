/*
	Definicion de metodos de tratamiento de colisiones. 
	Objeto de:
		- Tipo 1: estaticos
		- Tipo 2: dinamico, el jugador puede agarrarlo
		- Tipo 3: dinamico, se mueve automaticamente, te mata con colision en los lados y lo matas cayendo encima
*/

#include "../include/stdafx.h"
#include "../include/CollisionManagement.h"
#include "../include/Global.h"
#include "../include/Object.h"
#include "../include/Player.h"

//========================================================================
//Metodo de tratamiento de colision con un objeto en el eje X, segun el tipo
//========================================================================
void collisionManagementX(Object* obj)
{
	 int objType = obj->m_objType;
	 switch ( objType )
      {
		 //En caso de ser del tipo 3
         case 3:
			//Reseteamos el personaje al punto inicial			 			
			Ogre::Vector3 initPos = gPlayer->node->getInitialPosition();
			gPlayer->node->setPosition(initPos);			
			gCamera->setPosition(initPos.x, initPos.y, 450.0);
            break;
	 }
}

//========================================================================
//Metodo de tratamiento de colision con un objeto en el eje Y, segun el tipo
//========================================================================
void collisionManagementY(Object* obj)
{
	 int objType = obj->m_objType;
	 switch ( objType )
      {
		 //En caso de ser del tipo 3
         case 3:
			 //Eliminamos el objeto
			 obj->~Object();
            break;
	 }
}