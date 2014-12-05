#include "stdafx.h"
#include "GameApplication.h"
#include "BaseApplication.h"

//------------------------------------------------------------------
//---------------------------MAIN-----------------------------------
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
		baseApp->go();
		//Creamos el juego
		GameApplication* gameApp = new GameApplication();
		//Creamos el escenario
		gameApp->createScene();
		//Iniciamos el bucle de renderizado infinito
		baseApp->mRoot->startRendering();

		return 0;
    }
 
#ifdef __cplusplus
}
#endif