#include "stdafx.h"
#include "FuncionesGenerales.h"

//Implementacion de metodos de deteccion de colisiones

void imprimir(int mValue)
{
	OutputDebugString(Ogre::StringConverter::toString(mValue).c_str());
		OutputDebugString("\n");
	
}

void imprimir(int* mValue)
{
	OutputDebugString(Ogre::StringConverter::toString(mValue).c_str());
		OutputDebugString("\n");
	
}
void imprimir(Ogre::String mValue)
{
	OutputDebugString(mValue.c_str());
		OutputDebugString("\n");
	
}