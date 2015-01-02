#include "stdafx.h"
#include "FrameRate.h"
#include "Global.h"
#include "FuncionesGenerales.h"

//Frames por segundo
const int frameRate = 60; 
//Duraci�n de cada frame (segundos)
const double frameLength = 1.0 / (double) frameRate;

//Frecuencia con la que se actualiza el contador (ciclos por segundo)
__int64 freq; 
//Finalizacion del frame anterior (ciclos)
__int64 t0; 
//Finalizacion del frame frame actual (ciclos)
__int64 t1; 
//Tiempo de espera entre el frame actual y la duraci�n deseada
double tiempoSleep; 

//Devuelve el n�mero de frames por segundo
int getFPS() 
{
	return frameRate;
}

//Devuelve la duracion de cada frame
double getframeLength() 
{
	return frameLength;
}

//Inicializa el frame rate
void initFrameRate() 
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&t0);
}

//Duerme la ejecuci�n desde que se completa el frame hasta su duraci�n deseada (al final de cada iteraci�n del game loop)
void endFrame() 
{
	//Registramos el valor de t1 (fin del frame)
	QueryPerformanceCounter((LARGE_INTEGER*)&t1); 
	
	//Tiempo que esperar�
	tiempoSleep = frameLength - ((double)t1-(double)t0) / (double)freq; 
	if(tiempoSleep >= 0) {
		Sleep((DWORD)(tiempoSleep*1000.0)); 
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&t0); //Registramos la finalizacion del frame actual
}