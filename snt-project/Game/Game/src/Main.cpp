#include "stdafx.h"

#include "RenderActions.h"
#include "BaseApplication.h"
#include "GameApplication.h"

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
		//Listeners y acciones de renderizado para los frame
		RenderActions* renderAct = new RenderActions();
		//Creamos el juego (personaje y escenario)
		GameApplication* gameApp = new GameApplication();
		//Iniciamos el bucle de renderizado infinito (ver "frameRenderingQueued")
		gRoot->startRendering();

		return 0;
    }
 
#ifdef __cplusplus
}
#endif