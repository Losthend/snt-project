#include "../include/stdafx.h"

#include "../include/BaseApplication.h"

#include "../include/KeyboardMouse.h"
#include "../include/RenderActions.h"
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

			//Controlador de teclado y raton + CEGUI
			KeyboardMouse* keyboardMouse = new KeyboardMouse();

			//Listeners y acciones de renderizado para los frame
			RenderActions* renderAct = new RenderActions();

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