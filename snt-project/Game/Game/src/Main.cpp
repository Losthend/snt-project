#include "../include/stdafx.h"

//Instancia las clases principales, a saber: 
//Configuración de variables globales
#include "../include/BaseApplication.h"
//Listener para el renderizado de cada frame
#include "../include/RenderActions.h"
//Creación de escenario
#include "../include/GameApplication.h"
//Para el control de FPS
#include "../include/FrameRate.h"


//------------------------------------------------------------------
//--------------------------MAIN------------------------------------
//------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
		//Configuramos el entorno
		BaseApplication* baseApp = new BaseApplication();
		bool setupOK = baseApp->go();

		if (setupOK){

			//Listeners y acciones de renderizado para los frame
			RenderActions* renderAct = new RenderActions();

			//Creamos el juego (personaje y escenario)
			GameApplication* gameApp = new GameApplication();

			//Momento de ejecucion del primer frame
			initFrameRate();

			//Iniciamos el bucle de renderizado infinito (ver "frameRenderingQueued" en "RenderActions")
			baseApp->mRoot->startRendering();
		}
		return 0;
    }
 
#ifdef __cplusplus
}
#endif