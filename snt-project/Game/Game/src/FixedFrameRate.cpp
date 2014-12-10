#include "stdafx.h"

//Las declaraciones de sus variables y metodos
#include "FixedFrameRate.h"

//MAIN: añadir un startFrame() y endFrame() 

//QueryPerformanceCounter
//Proporciona el valor actual del temporizador
//QueryPerformanceFrequency
//Devuelve el contador (rendimiento) de la frecuencia actual por segundo
//Mas información: 
//http://computersciencevideos.org/Game-Engine-Development/Measuring-Time-Using-QueryPerformanceCounter()-and-QueryPerformanceFrequency()
//http://msdn.microsoft.com/en-us/library/ff650674.aspx

//Sleep

//Definimos el frame rate a 30 fps
#define FRAMERATE 30;

//El resto de variables que necesitamos son frecuencia del procesador, inicio y final del frame
//delta almacena la duracion real del frame y deltaSeconds lo pasa a segundos
//m_frameDuration calcula cuando es el tiempo "ideal" que hemos definido de duracion de frame
LARGE_INTEGER freq;
LARGE_INTEGER start;
LARGE_INTEGER end;
LARGE_INTEGER delta;
double m_frameDuration;
float deltaSeconds;

//Obtiene la frecuencia e inicia el contador
void startFrame()
{
	//Se obtiene la frecuencia
	QueryPerformanceFrequency(&freq);
	//Iniciamos el contador (en realidad coge el "tiempo" en el que se inicia)
	QueryPerformanceCounter(&start);
}

void endFrame()
{
	//Finaliza el contador (tiempo en que se finaliza)
	QueryPerformanceCounter(&end);
}

void duration()
{
	//Obtenemos la duracion del frame
	delta.QuadPart = end.QuadPart - start.QuadPart;
	//Obtenemos la duracion en segundos de ese frame
	deltaSeconds = ((float)delta.QuadPart) / freq.QuadPart;
	//Calculamos el tiempo a dormir teniendo en cuenta
	//lo que deberia haber durado (30 fps)
	m_frameDuration = 1.0/FRAMERATE;

	//Solo aplicamos el cambio cuando la duracion del frame ha sido menor a los fps estipulados
	if (m_frameDuration-deltaSeconds > 0)
	{
		Sleep((DWORD)(m_frameDuration-deltaSeconds)*1000);
	}
}

