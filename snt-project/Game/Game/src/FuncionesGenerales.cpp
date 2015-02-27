#include "../include/stdafx.h"

//Las declaraciones de sus metodos
#include "../include/FuncionesGenerales.h"

//Implementacion de metodos de deteccion de colisiones

void imprimir(int mValue)
{
	OutputDebugString(Ogre::StringConverter::toString(mValue).c_str());
		OutputDebugString("\n");
	
}

/*
void imprimir(int* mValue)
{
	OutputDebugString(Ogre::StringConverter::toString(mValue).c_str());
		OutputDebugString("\n");
	
}
*/

void imprimir(Ogre::String mValue)
{
	OutputDebugString(mValue.c_str());
		OutputDebugString("\n");
	
}