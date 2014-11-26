#include "stdafx.h"
#include "GameApplication.h"

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
		//Creamos el juego (configuraciones iniciales)
		GameApplication* gameApp = new GameApplication();
		//Creamos el escenario
		gameApp->createScene();

		return 0;
    }
 
#ifdef __cplusplus
}
#endif